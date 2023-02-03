/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#include "external_video_source.h"
#include "api/video/i420_buffer.h"

namespace vewtn {

ExternalVideoSource::ExternalVideoSource() {
    thread_checker_.Detach();
}

void ExternalVideoSource::RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) {
    RTC_DCHECK(thread_checker_.IsCurrent());
    broadcaster_.RemoveSink(sink);
}

void ExternalVideoSource::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                        const rtc::VideoSinkWants& wants) {
    RTC_DCHECK(thread_checker_.IsCurrent());
    {
        webrtc::MutexLock lock(&mutex_);
        wants_ = wants;
    }
    broadcaster_.AddOrUpdateSink(sink, wants);
}

bool ExternalVideoSource::PushVideoFrame(VideoFrame video_frame) {
    webrtc::VideoRotation rotation_internal = (webrtc::VideoRotation)video_frame.rotation;
    int width = video_frame.width;
    int height = video_frame.height;
    rtc::scoped_refptr<webrtc::I420Buffer> buffer(webrtc::I420Buffer::Copy(
            width, height, video_frame.y_buffer, video_frame.y_stride,
            video_frame.u_buffer, video_frame.u_stride,
            video_frame.v_buffer, video_frame.v_stride));

    webrtc::VideoFrame frame = webrtc::VideoFrame::Builder()
                                .set_video_frame_buffer(buffer)
                                .set_rotation(rotation_internal)
                                .set_timestamp_us(video_frame.timestamp_ms * rtc::kNumMicrosecsPerMillisec)
                                .build();
//    RTC_LOG(LS_INFO) << "[WTN]ExternalVideoSource::PushVideoFrame, width: " << width
//                     << ", height: " << height
//                     << ", timestamp_ms: " << video_frame.timestamp_ms;

    broadcaster_.OnFrame(frame);
    return true;
}

bool ExternalVideoSource::FakeYuv(int width, int height, long timestamp) {
    rtc::scoped_refptr<webrtc::I420Buffer> buffer(webrtc::I420Buffer::Create(width, height));
    webrtc::VideoRotation rotation_internal = webrtc::kVideoRotation_0;
    buffer->InitializeData();
    webrtc::VideoFrame frame = webrtc::VideoFrame::Builder()
        .set_video_frame_buffer(buffer)
        .set_rotation(rotation_internal)
        .set_timestamp_us(timestamp)
        .build();
    
    broadcaster_.OnFrame(frame);
    return true;
}

}    // end of namespace
