/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "video_sink_proxy.h"
#include <algorithm>
#include <chrono>
#include "rtc_base/logging.h"
#include <iostream>

namespace vewtn {

const int32_t RENDER_FROZEN_TIME = 500;
const int32_t RENDER_FROZEN_TIME_200 = 200;
const int32_t CHECK_DURATION = 2000;

VideoSinkProxy::VideoSinkProxy(
        std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> video_sink,
        VideoSinkObserver *observer, IRtcEventHandler *event_handler) :
        video_sink_(std::move(video_sink)),
        observer_(observer),
        event_handler_(event_handler) {
    stall_statistics_data_.reset(new StallStatisticsData);
}

VideoSinkProxy::~VideoSinkProxy() {
    observer_ = nullptr;
}

void VideoSinkProxy::OnFrame(const webrtc::VideoFrame &frame) {
    if (!is_first_frame_rendered_ && observer_) {
        is_first_frame_rendered_ = true;
        observer_->OnFirstVideoFrameRendered();
    }
    StallStatistic();

    if (event_handler_) {
        VideoFrame video_rame;
        video_rame.type = VideoFrameType::YUV_I420;
//        webrtc::VideoFrameBuffer::Type t = frame.video_frame_buffer()->type();
//        if (!frame.video_frame_buffer()->GetI420()) {
//            return;
//            auto i420 = frame.video_frame_buffer()->ToI420()->GetI420();
//            video_rame.y_buffer = i420->DataY();
//            video_rame.u_buffer = i420->DataU();
//            video_rame.v_buffer = i420->DataV();
//            video_rame.y_stride = i420->StrideY();
//            video_rame.u_stride = i420->StrideU();
//            video_rame.v_stride = i420->StrideV();
//        }
        video_rame.y_buffer = frame.video_frame_buffer()->GetI420()->DataY();
        video_rame.u_buffer = frame.video_frame_buffer()->GetI420()->DataU();
        video_rame.v_buffer = frame.video_frame_buffer()->GetI420()->DataV();
        video_rame.y_stride = frame.video_frame_buffer()->GetI420()->StrideY();
        video_rame.u_stride = frame.video_frame_buffer()->GetI420()->StrideU();
        video_rame.v_stride = frame.video_frame_buffer()->GetI420()->StrideV();
        video_rame.height = frame.height();
        video_rame.width = frame.width();
        video_rame.rotation = frame.rotation();
        video_rame.timestamp_ms = frame.render_time_ms();
        event_handler_->OnVideoFrame(stream_id_.c_str(), video_rame);
        return;
    }

    if (video_sink_) {
        video_sink_->OnFrame(frame);
    }
}

void VideoSinkProxy::UpdateStreamId(const std::string& stream_id) {
    stream_id_ = stream_id;
}

void VideoSinkProxy::OnDiscardedFrame() {
    if (video_sink_) {
        video_sink_->OnDiscardedFrame();
    }
}

void VideoSinkProxy::OnConstraintsChanged(
                                          const webrtc::VideoTrackSourceConstraints &constraints) {
    if (video_sink_) {
        video_sink_->OnConstraintsChanged(constraints);
    }
}

void VideoSinkProxy::StallStatistic() {
    auto now = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                                          std::chrono::system_clock::now().time_since_epoch()).count());
    if (!recv_first_video_frame_) {
        recv_first_video_frame_ = true;
        last_video_frame_timestamp_ = now;
        last_video_frame_timestamp200_ = now;
        last_check_timestamp_ = now;
        // render_frame_ = 1;
        stall_statistics_data_->render_frame_rate_ = 1;
        video_frozen_ = false;
        video_frozen200_ = false;
        return;
    }
    
    stall_statistics_data_->render_frame_rate_++;
    
    // threshold is 500ms
    auto elapse = now - last_video_frame_timestamp_;
    if (elapse >= RENDER_FROZEN_TIME || video_frozen_) {
        if (!video_frozen_) {
            stall_statistics_data_->video_stall_count_++;
        }
        
        stall_statistics_data_->video_stall_duration_ += elapse;
        video_frozen_ = false;
    }
    
    // threshold is 200ms
    auto elapse200 = now - last_video_frame_timestamp200_;
    if (elapse200 >= RENDER_FROZEN_TIME_200 || video_frozen200_) {
        if (!video_frozen200_) {
            stall_statistics_data_->video_stall_count200_++;
        }
        
        stall_statistics_data_->video_stall_duration200_ += elapse200;
        video_frozen200_ = false;
    }
    
    last_video_frame_timestamp_ = now;
    last_video_frame_timestamp200_ = now;
}

StallStatisticsData* VideoSinkProxy::GetStallStatisticsData() {
    return stall_statistics_data_.get();
}

void VideoSinkProxy::ResetStallStatisticsData() {
    stall_statistics_data_->ResetData();
}

void VideoSinkProxy::CheckRenderVideoFrame() {
    if (recv_first_video_frame_) {
        auto now = static_cast<int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                                                              std::chrono::system_clock::now().time_since_epoch()).count());
        // calculate the duration of pulling the video stream，and calculate fps
        // GetPullDurationAndFps(now);
        // update checkpoint timestamp
        last_check_timestamp_ = now;
        
        // threshold is 500ms
        auto elapse = now - last_video_frame_timestamp_;
        if (elapse >= RENDER_FROZEN_TIME) {
            if (!video_frozen_) {
                stall_statistics_data_->video_stall_count_++;
            }
            
            stall_statistics_data_->video_stall_duration_ += elapse;
            video_frozen_ = true;
            
            // update the timestamp of the last video frame
            last_video_frame_timestamp_ = now;
        }
        
        // threshold is 200ms
        auto elapse200 = now - last_video_frame_timestamp200_;
        if (elapse200 >= RENDER_FROZEN_TIME_200) {
            if (!video_frozen200_) {
                stall_statistics_data_->video_stall_count200_++;
            }
            
            stall_statistics_data_->video_stall_duration200_ += elapse200;
            video_frozen200_ = true;
            
            // update the timestamp of the last video frame
            last_video_frame_timestamp200_ = now;
        }
    }
}

}
