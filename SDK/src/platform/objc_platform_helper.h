/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#pragma once
#include <memory>

#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"

#include "api/video/video_sink_interface.h"
#include "api/video/video_frame.h"

#include "api/media_stream_interface.h"
#include "api/scoped_refptr.h"
#include "rtc_base/thread.h"

namespace vewtn {

std::unique_ptr<webrtc::VideoEncoderFactory> CreateObjCEncoderFactory();
std::unique_ptr<webrtc::VideoDecoderFactory> CreateObjCDecoderFactory();

rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateObjCVideoTrackSource(bool is_publisher,
                                                                                 rtc::Thread *signaling_thread,
                                                                                 rtc::Thread *worker_thread);

std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> ObjcToNativeVideoSink(void* view);
void ObjcLocalViewToNativeVideoSink(void* view);
void StartObjCVideoCapturer(bool is_front, int width, int height, int fps);
void StopObjCVideoCapturer();
void SetObjCEnableSpeaker(bool enable);

}  // namespace webrtc
