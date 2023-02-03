/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#include "platform/objc_platform_helper.h"

#import "sdk/objc/components/video_codec/RTCVideoDecoderFactoryH264.h"
#import "sdk/objc/components/video_codec/RTCVideoEncoderFactoryH264.h"
#include "sdk/objc/native/api/video_decoder_factory.h"
#include "sdk/objc/native/api/video_encoder_factory.h"
#include "sdk/objc/native/api/video_renderer.h"
#include "api/video/video_sink_interface.h"
#include "api/video/video_frame.h"
#include "api/media_stream_interface.h"
#include "api/scoped_refptr.h"
#include "rtc_base/thread.h"
#include "sdk/objc/native/api/video_capturer.h"
#include "api/video_track_source_proxy_factory.h"
#include "rtc_base/ref_counted_object.h"
#include "sdk/objc/native/src/objc_video_track_source.h"
#import "sdk/objc/components/renderer/metal/RTCMTLVideoView.h"
#import "sdk/objc/components/capturer/RTCCameraVideoCapturer.h"
#import "sdk/objc/helpers/RTCCameraPreviewView.h"
#import "base/RTCVideoCapturer.h"
#import "sdk/objc/components/audio/RTCAudioSessionConfiguration.h"
#import "sdk/objc/base/RTCLogging.h"
#import "sdk/objc/components/audio/RTCAudioSessionConfiguration.h"

namespace vewtn {

RTC_OBJC_TYPE(RTCCameraVideoCapturer) * capturer = nil;
RTCObjCVideoSourceAdapter *adapter = nil;
RTC_OBJC_TYPE(RTCMTLVideoView)* realView = nil;
RTC_OBJC_TYPE(RTCCameraPreviewView) * localVideoView = nil;

AVCaptureDevice * findDeviceForPosition(AVCaptureDevicePosition position) {
    NSArray<AVCaptureDevice *> *captureDevices = [RTC_OBJC_TYPE(RTCCameraVideoCapturer) captureDevices];
    for (AVCaptureDevice *device in captureDevices) {
        if (device.position == position) {
            return device;
        }
    }
    return captureDevices[0];
}

AVCaptureDeviceFormat * selectFormatForDevice(AVCaptureDevice * device, int targetWidth, int targetHeight) {
    NSArray<AVCaptureDeviceFormat *> *formats = [RTC_OBJC_TYPE(RTCCameraVideoCapturer) supportedFormatsForDevice:device];
    AVCaptureDeviceFormat *selectedFormat = nil;
    int currentDiff = INT_MAX;
    
    for (AVCaptureDeviceFormat *format in formats) {
        CMVideoDimensions dimension = CMVideoFormatDescriptionGetDimensions(format.formatDescription);
        FourCharCode pixelFormat = CMFormatDescriptionGetMediaSubType(format.formatDescription);
        int diff = abs(targetWidth - dimension.width) + abs(targetHeight - dimension.height);
        if (diff < currentDiff) {
            selectedFormat = format;
            currentDiff = diff;
        } else if (diff == currentDiff && pixelFormat == [capturer preferredOutputPixelFormat]) {
            selectedFormat = format;
        }
    }
    
    return selectedFormat;
}

NSInteger selectFpsForFormat(AVCaptureDeviceFormat *format, int fps) {
    Float64 maxSupportedFramerate = 0;
    for (AVFrameRateRange *fpsRange in format.videoSupportedFrameRateRanges) {
        maxSupportedFramerate = fmax(maxSupportedFramerate, fpsRange.maxFrameRate);
    }
    return fmin(maxSupportedFramerate, fps);
}

std::unique_ptr<webrtc::VideoEncoderFactory> CreateObjCEncoderFactory() {
    return webrtc::ObjCToNativeVideoEncoderFactory([[RTC_OBJC_TYPE(RTCVideoEncoderFactoryH264) alloc] init]);
}

std::unique_ptr<webrtc::VideoDecoderFactory> CreateObjCDecoderFactory() {
    return webrtc::ObjCToNativeVideoDecoderFactory([[RTC_OBJC_TYPE(RTCVideoDecoderFactoryH264) alloc] init]);
}

std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> ObjcToNativeVideoSink(void* view) {
    dispatch_sync(dispatch_get_main_queue(), ^{    // UI need at main thread
        realView = [[RTC_OBJC_TYPE(RTCMTLVideoView) alloc] initWithFrame:CGRectZero];

        UIView *uiView = (__bridge UIView*)(view);
        [uiView addSubview:realView];
        realView.frame = CGRectMake(0, 0, uiView.frame.size.width, uiView.frame.size.height);
        realView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [realView setVideoContentMode:UIViewContentModeScaleAspectFill];
        [realView setClipsToBounds:YES];
    });

    return webrtc::ObjCToNativeVideoRenderer(realView);
}

void ObjcLocalViewToNativeVideoSink(void* view) {
    dispatch_async(dispatch_get_main_queue(), ^{    // UI need at main thread
        if (localVideoView != nil) {
            // todo: release ?
        }
        localVideoView = [[RTC_OBJC_TYPE(RTCCameraPreviewView) alloc] initWithFrame:CGRectZero];
        UIView *uiView = (__bridge UIView*)(view);
        [uiView addSubview:localVideoView];
        localVideoView.frame = CGRectMake(0, 0, uiView.frame.size.width, uiView.frame.size.height);
        localVideoView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [localVideoView setContentMode:UIViewContentModeScaleAspectFill];
        [localVideoView setClipsToBounds:YES];
    });
}

rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateObjCVideoTrackSource(bool is_publisher, rtc::Thread *signaling_thread, rtc::Thread *worker_thread) {
    if (capturer == nil && is_publisher) {
        capturer = [[RTC_OBJC_TYPE(RTCCameraVideoCapturer) alloc] init];
//        capturer.delegate = adapter;
    }
    
    rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> source;
    if (is_publisher) {
        source = webrtc::ObjCToNativeVideoCapturer(capturer, signaling_thread, worker_thread);
//        capturer.delegate = adapter;
    } else {
        source = webrtc::ObjCToNativeVideoCapturer(nullptr, signaling_thread, worker_thread);
    }
    return source;
}

void StartObjCVideoCapturer(bool is_front, int width, int height, int fps) {
    if (capturer == nil) {
        capturer = [[RTC_OBJC_TYPE(RTCCameraVideoCapturer) alloc] init];
//        capturer.delegate = adapter;
    }

    if (localVideoView != nil) {
        capturer.captureSession.sessionPreset = AVCaptureSessionPresetHigh;
        localVideoView.captureSession = capturer.captureSession;
    }

    AVCaptureDevicePosition position = is_front ? AVCaptureDevicePositionFront : AVCaptureDevicePositionBack;
    AVCaptureDevice *device = findDeviceForPosition(position);
    AVCaptureDeviceFormat *format = selectFormatForDevice(device, width, height);
    if (format == nil) {
        RTCLogError(@"No valid formats for device %@", device);
        return;
    }
    
    NSInteger selectfps = selectFpsForFormat(format, fps);
    [capturer startCaptureWithDevice:device format:format fps:selectfps completionHandler:nil];
}

void StopObjCVideoCapturer() {
    if (capturer == nil) {
        return;
    }
    [capturer stopCapture];
//    capturer = nil;
}

void SetObjCEnableSpeaker(bool enable) {
    NSError *error = nil;
    AVAudioSession* sysSession = [AVAudioSession sharedInstance];
    if (enable) {
        [sysSession overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:&error];
    } else {
        [sysSession overrideOutputAudioPort:AVAudioSessionPortOverrideNone error:&error];
    }
}

}  // namespace 
