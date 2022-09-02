/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
 
#import "Publisher.h"
#include "vewtn_interface.h"
#include "PublisherEventHandler.h"
#import "sdk/objc/base/RTCVideoRenderer.h"
#import "sdk/objc/components/capturer/RTCCameraVideoCapturer.h"
#import "sdk/objc/components/renderer/metal/RTCMTLVideoView.h"
#import "sdk/objc/helpers/RTCCameraPreviewView.h"
#include "sdk/objc/native/api/video_capturer.h"
#import "sdk/objc/base/RTCLogging.h"
#import "sdk/objc/helpers/RTCCameraPreviewView.h"
#import "sdk/objc/native/api/audio_device_module.h"
#import "sdk/objc/native/src/audio/audio_device_ios.h"
#import "sdk/objc/components/audio/RTCAudioSession+Private.h"
#import "sdk/objc/native/api/audio_device_module.h"

#import "sdk/objc/components/video_codec/RTCDefaultVideoDecoderFactory.h"
#import "sdk/objc/components/video_codec/RTCDefaultVideoEncoderFactory.h"
#include "sdk/objc/native/api/video_encoder_factory.h"
#include "sdk/objc/native/api/video_decoder_factory.h"

static Publisher * _sharedPublisher = nil;
static dispatch_once_t onceToken = 0;

@interface Publisher() {
    PublisherEventHandler * _handler;
    vewtn::IPublisher * _publisher;
    rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> _video_source;
    rtc::scoped_refptr<webrtc::AudioDeviceModule> _audioDeviceModule;
    VideoConfig* _videoConfig;
    std::unique_ptr<webrtc::ios_adm::AudioDeviceIOS> _audio_device;
    std::unique_ptr<webrtc::VideoEncoderFactory> video_encoder_factory;
    std::unique_ptr<webrtc::VideoDecoderFactory> video_decoder_factory;
}

@property(nonatomic) BOOL usingFrontCamera;
@property(nonatomic) BOOL isStartAudioCapture;
@property(nonatomic) RTC_OBJC_TYPE(RTCCameraVideoCapturer) * capturer;
@property(nonatomic) RTC_OBJC_TYPE(RTCCameraPreviewView) * localVideoView;
@property(nonatomic) dispatch_queue_t queue;

@end

@implementation Publisher

+ (instancetype)create:(id<PublisherDelegate> _Nullable)delegate {
    dispatch_once(&onceToken, ^{
        _sharedPublisher = [[Publisher alloc] init];
        _sharedPublisher->_usingFrontCamera = YES;
        _sharedPublisher->_queue = dispatch_queue_create("PublisherThread", NULL);
        _sharedPublisher->_handler = new PublisherEventHandler(_sharedPublisher, delegate);
        _sharedPublisher->_audioDeviceModule = webrtc::CreateAudioDeviceModule(false);
        _sharedPublisher->video_encoder_factory = webrtc::ObjCToNativeVideoEncoderFactory(
                                                                                          [[RTC_OBJC_TYPE(RTCDefaultVideoEncoderFactory) alloc] init]);
        _sharedPublisher->video_decoder_factory = webrtc::ObjCToNativeVideoDecoderFactory(
                                                                                          [[RTC_OBJC_TYPE(RTCDefaultVideoDecoderFactory) alloc] init]);
        _sharedPublisher->_publisher = vewtn::CreatePublisher(_sharedPublisher->_handler,
                                                              _sharedPublisher->_audioDeviceModule,
                                                              std::move(_sharedPublisher->video_encoder_factory),
                                                              std::move(_sharedPublisher->video_decoder_factory));
        RTCSetMinDebugLogLevel(RTCLoggingSeverityInfo);
    });
    return _sharedPublisher;
}

- (void)destroy {
    dispatch_async(_queue, ^{
        [_capturer stopCapture];
        if (_audioDeviceModule) {
            _audioDeviceModule->StopRecording();
        }
        vewtn::DestroyPublisher(_publisher);
        _publisher = nullptr;
        if (_handler) {
            delete _handler;
            _handler = nullptr;
        }
        _videoConfig = nil;
        _audioDeviceModule.release();
        _video_source.release();
    });
    _sharedPublisher = nil;
    onceToken = 0;
}

- (int)setVideoConfig:(VideoConfig * _Nonnull)videoConfig {
    if (videoConfig == nil) {
        return -1;
    }
    _videoConfig = [[VideoConfig alloc] init];
    _videoConfig = videoConfig;
    return 0;
}

- (int)setLocalView:(UIView * _Nullable)view {
    if (view == nil) {
        return -1;
    }
    _localVideoView = [[RTC_OBJC_TYPE(RTCCameraPreviewView) alloc] initWithFrame:CGRectZero];
    [view addSubview:_localVideoView];
    _localVideoView.frame = CGRectMake(0, 0, view.frame.size.width, view.frame.size.height);
    _localVideoView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [_localVideoView setContentMode:UIViewContentModeScaleAspectFill];
    [_localVideoView setClipsToBounds:YES];
    return 0;
}

- (int)startVideoCapture {
    if (_publisher == nullptr) {
        return -1;
    }
    dispatch_async(_queue, ^{
        if (_capturer == nil) {
            _capturer = [[RTC_OBJC_TYPE(RTCCameraVideoCapturer) alloc] init];
        }
        if (_localVideoView != nil) {
            self.capturer.captureSession.sessionPreset = AVCaptureSessionPresetHigh;
            _localVideoView.captureSession = _capturer.captureSession;
        }
        if (_video_source == nullptr) {
            rtc::Thread* signaling_thread = _publisher->GetSignalingThread();
            rtc::Thread* worker_thread = _publisher->GetWorkerThread();
            _video_source = webrtc::ObjCToNativeVideoCapturer(_capturer, signaling_thread, worker_thread);
            _publisher->SetVideoSource(_video_source);
        }
        
        AVCaptureDevicePosition position =
        _usingFrontCamera ? AVCaptureDevicePositionFront : AVCaptureDevicePositionBack;
        AVCaptureDevice *device = [self findDeviceForPosition:position];
        AVCaptureDeviceFormat *format = [self selectFormatForDevice:device];
        if (format == nil) {
            RTCLogError(@"No valid formats for device %@", device);
            NSAssert(NO, @"");
            return;
        }
        
        NSInteger fps = [self selectFpsForFormat:format];
        [_capturer startCaptureWithDevice:device format:format fps:fps completionHandler:nil];
    });
    return 0;
}

- (int)stopVideoCapture {
    if (_capturer == nil) {
        return -1;
    }
    dispatch_async(_queue, ^{
        [_capturer stopCapture];
    });
    return 0;
}

- (int)switchCamera {
    if (_capturer == nil) {
        return -1;
    }
    dispatch_async(_queue, ^{
        _usingFrontCamera = !_usingFrontCamera;
        [self startVideoCapture];
    });
    return 0;
}

- (int)startAudioCapture {
    if (_capturer == nil) {
        return -1;
    }
    _isStartAudioCapture = TRUE;
    dispatch_async(_queue, ^{
        _audioDeviceModule->InitRecording();
        _audioDeviceModule->StartRecording();
    });
    return 0;
}

- (int)stopAudioCapture {
    if (_capturer == nil) {
        return -1;
    }
    _isStartAudioCapture = FALSE;
    dispatch_async(_queue, ^{
        _audioDeviceModule->StopRecording();
    });
    return 0;
}

- (int)startPublish:(NSString*)url {
    if (_publisher == nullptr || url.length == 0) {
        return -1;
    }
    dispatch_async(_queue, ^{
        if (_video_source == nullptr) {
            rtc::Thread* signaling_thread = _publisher->GetSignalingThread();
            rtc::Thread* worker_thread = _publisher->GetWorkerThread();
            if (_capturer == nil) {
                _capturer = [[RTC_OBJC_TYPE(RTCCameraVideoCapturer) alloc] init];
            }
            _video_source = webrtc::ObjCToNativeVideoCapturer(_capturer, signaling_thread, worker_thread);
            _publisher->SetVideoSource(_video_source);
        }
        if (!_isStartAudioCapture) {
            _audioDeviceModule->StopRecording();  // default stop audio record
        }
        _publisher->StartPublish([url UTF8String]);
    });
    return 0;
}

- (int)stopPublish {
    if (_publisher == nullptr) {
        return -1;
    }
    dispatch_async(_queue, ^{
        _publisher->StopPublish();
    });
    return 0;
}

- (bool)isPublishing {
    if (_publisher) {
        return _publisher->IsPublishing();
    }
    return false;
}

#pragma mark - Private

- (AVCaptureDevice *)findDeviceForPosition:(AVCaptureDevicePosition)position {
    NSArray<AVCaptureDevice *> *captureDevices =
    [RTC_OBJC_TYPE(RTCCameraVideoCapturer) captureDevices];
    for (AVCaptureDevice *device in captureDevices) {
        if (device.position == position) {
            return device;
        }
    }
    return captureDevices[0];
}

- (AVCaptureDeviceFormat *)selectFormatForDevice:(AVCaptureDevice *)device {
    NSArray<AVCaptureDeviceFormat *> *formats =
    [RTC_OBJC_TYPE(RTCCameraVideoCapturer) supportedFormatsForDevice:device];
    int targetWidth = [_videoConfig width];
    int targetHeight = [_videoConfig height];
    AVCaptureDeviceFormat *selectedFormat = nil;
    int currentDiff = INT_MAX;
    
    for (AVCaptureDeviceFormat *format in formats) {
        CMVideoDimensions dimension = CMVideoFormatDescriptionGetDimensions(format.formatDescription);
        FourCharCode pixelFormat = CMFormatDescriptionGetMediaSubType(format.formatDescription);
        int diff = abs(targetWidth - dimension.width) + abs(targetHeight - dimension.height);
        if (diff < currentDiff) {
            selectedFormat = format;
            currentDiff = diff;
        } else if (diff == currentDiff && pixelFormat == [_capturer preferredOutputPixelFormat]) {
            selectedFormat = format;
        }
    }
    
    return selectedFormat;
}

- (NSInteger)selectFpsForFormat:(AVCaptureDeviceFormat *)format {
    Float64 maxSupportedFramerate = 0;
    for (AVFrameRateRange *fpsRange in format.videoSupportedFrameRateRanges) {
        maxSupportedFramerate = fmax(maxSupportedFramerate, fpsRange.maxFrameRate);
    }
    return fmin(maxSupportedFramerate, [_videoConfig fps]);
}

@end

@implementation VideoConfig

-(id)init {
    if (self = [super init])  {
        self.width = 960;
        self.height = 640;
        self.fps = 15;
    }
    return self;
}

@end

