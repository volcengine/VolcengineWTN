/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#pragma once
#include <memory>

#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"
#include <sdk/android/native_api/audio_device_module/audio_device_android.h>
#include <sdk/android/native_api/video/video_source.h>

namespace vewtn {

std::unique_ptr<webrtc::VideoEncoderFactory> CreateAndroidEncoderFactory();
std::unique_ptr<webrtc::VideoDecoderFactory> CreateAndroidDecoderFactory();
std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> JavaToNativeVideoSink(void* remote_view);
rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateAndroidVideoTrackSource(rtc::Thread * signaling_thread);
void StartAndroidVideoCapturer(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> source, bool is_front, int width, int height, int fps);
void StopAndroidVideoCaptuer();
void SwitchAndroidVideoCaptuer();
void SetAndroidEnableSpeaker(bool enable);
bool CheckPermission();
bool CheckRecordPermission();
bool CheckCameraPermission();

}  // namespace
