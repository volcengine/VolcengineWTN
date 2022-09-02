/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
 
#import "Subscriber.h"
#include "vewtn_interface.h"
#include "SubscriberEventHandler.h"
#import "sdk/objc/base/RTCVideoRenderer.h"
#import "sdk/objc/components/capturer/RTCCameraVideoCapturer.h"
#import "sdk/objc/components/renderer/metal/RTCMTLVideoView.h"
#include "sdk/objc/native/api/video_capturer.h"
#import "sdk/objc/base/RTCLogging.h"
#include "sdk/objc/native/api/video_renderer.h"
#import "sdk/objc/components/audio/RTCAudioSessionConfiguration.h"
#import "sdk/objc/native/api/audio_device_module.h"

#import "sdk/objc/components/video_codec/RTCDefaultVideoDecoderFactory.h"
#import "sdk/objc/components/video_codec/RTCDefaultVideoEncoderFactory.h"
//#include "api/video_codecs/video_encoder_factory.h"
//#include "api/video_codecs/video_decoder_factory.h"
#include "sdk/objc/native/api/video_encoder_factory.h"
#include "sdk/objc/native/api/video_decoder_factory.h"

static Subscriber * _sharedSubscriber = nil;

static dispatch_once_t onceToken = 0;

@interface Subscriber() {
    SubscriberEventHandler * _handler;
    vewtn::ISubscriber * _subscriber;
    rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> _video_source;
    std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> _remote_sink;
    rtc::scoped_refptr<webrtc::AudioDeviceModule> _audioDeviceModule;
    
    std::unique_ptr<webrtc::VideoEncoderFactory> video_encoder_factory;
    std::unique_ptr<webrtc::VideoDecoderFactory> video_decoder_factory;
}

@property(nonatomic) __kindof UIView<RTC_OBJC_TYPE(RTCVideoRenderer)> *remoteVideoView;
@property(nonatomic) dispatch_queue_t queue;

@end

@implementation Subscriber

+ (instancetype)create:(id<SubscriberDelegate> _Nullable)delegate {
    dispatch_once(&onceToken, ^{
        _sharedSubscriber = [[Subscriber alloc] init];
        _sharedSubscriber->_handler = new SubscriberEventHandler(_sharedSubscriber, delegate);
        _sharedSubscriber->_queue = dispatch_queue_create("SubscriberThread", NULL);
        RTC_OBJC_TYPE(RTCAudioSessionConfiguration) *webRTCConfig =
        [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) webRTCConfiguration];
        webRTCConfig.categoryOptions = webRTCConfig.categoryOptions |
        AVAudioSessionCategoryOptionDefaultToSpeaker;
        [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) setWebRTCConfiguration:webRTCConfig];
        _sharedSubscriber->_audioDeviceModule = webrtc::CreateAudioDeviceModule(false);
        _sharedSubscriber->video_encoder_factory = webrtc::ObjCToNativeVideoEncoderFactory(
                                                                                           [[RTC_OBJC_TYPE(RTCDefaultVideoEncoderFactory) alloc] init]);
        _sharedSubscriber->video_decoder_factory = webrtc::ObjCToNativeVideoDecoderFactory(
                                                                                           [[RTC_OBJC_TYPE(RTCDefaultVideoDecoderFactory) alloc] init]);
        
        _sharedSubscriber->_subscriber = vewtn::CreateSubscriber(_sharedSubscriber->_handler, _sharedSubscriber->_audioDeviceModule,
                                                                 std::move(_sharedSubscriber->video_encoder_factory),
                                                                 std::move(_sharedSubscriber->video_decoder_factory));
        RTCSetMinDebugLogLevel(RTCLoggingSeverityInfo);
    });
    return _sharedSubscriber;
}

- (void)destroy {
    dispatch_async(_queue, ^{
        vewtn::DestroySubscriber(_subscriber);
        _subscriber = nullptr;
        if (_handler) {
            delete _handler;
            _handler = nullptr;
        }
        _audioDeviceModule.release();
        _remote_sink = nullptr;
        _video_source.release();
    });
    _sharedSubscriber = nil;
    onceToken = 0;
}

- (int)setRemoteView:(UIView*)view {
    if (_subscriber == nullptr || view == nil) {
        return -1;
    }
    if (_remoteVideoView == nil) {
        _remoteVideoView = [[RTC_OBJC_TYPE(RTCMTLVideoView) alloc] initWithFrame:CGRectZero];
    }
    
    [view addSubview:_remoteVideoView];
    _remoteVideoView.frame = CGRectMake(0, 0, view.frame.size.width, view.frame.size.height);
    _remoteVideoView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    // [_remoteVideoView setVideoContentMode:UIViewContentModeScaleAspectFit];  // fit render mode
    [_remoteVideoView setVideoContentMode:UIViewContentModeScaleAspectFill];
    [_remoteVideoView setClipsToBounds:YES];
    
    _remote_sink = webrtc::ObjCToNativeVideoRenderer(_remoteVideoView);
    _subscriber->SetRemoteCanvas(_remote_sink);
    return 0;
}

- (int)setEnableSpeaker:(BOOL)enable {
    if (_subscriber == nullptr) {
        return -1;
    }
    dispatch_async(_queue, ^{
        if (enable) {
            RTC_OBJC_TYPE(RTCAudioSessionConfiguration) *webRTCConfig =
            [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) webRTCConfiguration];
            webRTCConfig.categoryOptions = webRTCConfig.categoryOptions | AVAudioSessionCategoryOptionDefaultToSpeaker;
            [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) setWebRTCConfiguration:webRTCConfig];
        } else {
            RTC_OBJC_TYPE(RTCAudioSessionConfiguration) *webRTCConfig =
            [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) webRTCConfiguration];
            webRTCConfig.categoryOptions = AVAudioSessionCategoryOptionAllowBluetooth;
            [RTC_OBJC_TYPE(RTCAudioSessionConfiguration) setWebRTCConfiguration:webRTCConfig];
        }
        _audioDeviceModule->StopRecording();
        _audioDeviceModule->StopPlayout();
        _audioDeviceModule->InitPlayout();
        _audioDeviceModule->StartPlayout();
    });
    return 0;
}

- (int)startSubscribe:(NSString*)url {
    if (_subscriber == nullptr || url.length == 0) {
        return -1;
    }
    dispatch_async(_queue, ^{
        if (!_video_source){
            rtc::Thread* signaling_thread = _subscriber->GetSignalingThread();
            rtc::Thread* worker_thread = _subscriber->GetWorkerThread();
            _video_source = webrtc::ObjCToNativeVideoCapturer(nullptr, signaling_thread, worker_thread);
            _subscriber->SetVideoSource(_video_source);
        }
        _subscriber->StartSubscribe([url UTF8String]);
        _audioDeviceModule->StopRecording();
    });
    return 0;
}

- (int)stopSubscribe {
    if (_subscriber == nullptr) {
        return -1;
    }
    dispatch_async(_queue, ^{
        _subscriber->StopSubscribe();
    });
    return 0;
}

- (bool)isSubscribed {
    if (_subscriber) {
        return _subscriber->IsSubscribed();
    }
    return false;
}

@end
