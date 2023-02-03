/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#import "VEWTNPublisher.h"
#import "VEWTNPubSubInternal.h"
#include "wtn_interface.h"

@interface VEWTNPublisher ()
@property (nonatomic, assign) vewtn::IPublisher* publisher;
@end

@implementation VEWTNPublisher
- (instancetype)init {
    if (self = [super init]) {
        _publisher = nullptr;
    }
    return self;
}

- (void)dealloc {
    
}


- (int)startPublish:(NSString*)url {
    if (_publisher != nullptr) {
        return _publisher->StartPublish([url UTF8String]);
    }
    return -1;
}
- (int)stopPublish {
    if (_publisher != nullptr) {
        return _publisher->StopPublish();
    }
    return -1;
}

- (BOOL)isPublishing {
    if (_publisher != nullptr) {
        return _publisher->IsPublishing();
    }
    return NO;
}

- (void)setLocalView:(UIView*)view {
    if (_publisher != nullptr) {
        _publisher->SetLocalView((__bridge void*)view);
    }
}
- (int)setVideoConfig:(VEWTNVideoConfig*)video_config  {
    if (_publisher != nullptr) {
        vewtn::VideoConfig videoconfig;
        videoconfig.width = video_config.width;
        videoconfig.height = video_config.height;
        videoconfig.max_fps = video_config.max_fps;
        return _publisher->SetVideoConfig(videoconfig);
    }
    return -1;
}
- (int)startVideoCapture {
    if (_publisher != nullptr) {
        return _publisher->StartVideoCapture();
    }
    return -1;
}

- (int)stopVideoCapture {
    if (_publisher != nullptr) {
        return _publisher->StopVideoCapture();
    }
    return -1;
}

- (int)switchCamera {
    if (_publisher != nullptr) {
        return _publisher->SwitchCamera();
    }
    return -1;
}

- (int)startAudioCapture {
    if (_publisher != nullptr) {
        return _publisher->StartAudioCapture();
    }
    return -1;
}

- (int)stopAudioCapture {
    if (_publisher != nullptr) {
        return _publisher->StopAudioCapture();
    }
    return -1;
}

- (int)setLocalAudioMute:(BOOL)mute {
    if (_publisher != nullptr) {
        return _publisher->SetLocalAudioMute(mute);
    }
    return -1;
}

- (int)setLocalVideoMute:(BOOL)mute {
    if (_publisher != nullptr) {
        return _publisher->SetLocalVideoMute(mute);
    }
    return -1;
}

- (BOOL)pushEncodedVideoFrame:(VEWTNEncodedVideoFrame*)encoded_frame {
    if (_publisher != nullptr) {
        vewtn::EncodedVideoFrame frame;
        frame.codec_type = (vewtn::EncodedVideoFrameType)encoded_frame.codec_type;
        frame.data = encoded_frame.data;
        frame.size = encoded_frame.size;
        frame.width = encoded_frame.width;
        frame.height = encoded_frame.height;
        frame.timestamp_ms = encoded_frame.timestamp_ms;
        frame.rotation = encoded_frame.rotation;
        frame.is_key_frame = encoded_frame.is_key_frame;
        return _publisher->PushEncodedVideoFrame(frame);
    }
    return NO;
}


- (BOOL)pushVideoFrame:(VEWTNVideoFrame*)video_frame {
    if (_publisher != nullptr) {
        vewtn::VideoFrame frame;
        frame.type = (vewtn::VideoFrameType)video_frame.type;
        frame.y_buffer = video_frame.y_buffer;
        frame.u_buffer = video_frame.u_buffer;
        frame.v_buffer = video_frame.v_buffer;
        frame.y_stride = video_frame.y_stride;
        frame.u_stride = video_frame.u_stride;
        frame.v_stride = video_frame.v_stride;
        frame.width = video_frame.width;
        frame.height = video_frame.height;
        frame.timestamp_ms = video_frame.timestamp_ms;
        frame.rotation = video_frame.rotation;
        return _publisher->PushVideoFrame(frame);
    }
    return NO;
}


- (BOOL)pushAudioFrame:(VEWTNAudioFrame*)audio_frame {
    if (_publisher != nullptr) {
        vewtn::AudioFrame frame;
        frame.type = (vewtn::AudioFrameType)audio_frame.type;
        frame.data = audio_frame.data;
        frame.sample_rate = audio_frame.sample_rate;
        frame.sample_length = audio_frame.sample_length;
        frame.channels = audio_frame.channels;
        frame.timestamp_ms = audio_frame.timestamp_ms;
        return _publisher->PushAudioFrame(frame);
    }
    return NO;
}

@end


@implementation VEWTNPublisher(internal)

- (void)setNativePublisher:(vewtn::IPublisher*)publisher {
    _publisher = publisher;
}

- (vewtn::IPublisher*)getNativePublisher {
    return _publisher;
}

@end
