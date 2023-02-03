/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "api/video/video_sink_interface.h"
#include "api/video/video_frame.h"

namespace vewtn {

class StallStatisticsData {
public:
    StallStatisticsData()  = default;
    StallStatisticsData(int64_t stall_count, int64_t stall_duration, int64_t stall_count200, int64_t stall_duration200,
                        int64_t frame_rate, int pull_duration):
    video_stall_count_(stall_count), video_stall_duration_(stall_duration), video_stall_count200_(stall_count200),
    video_stall_duration200_(stall_duration200), render_frame_rate_(frame_rate)
    {}
    virtual ~StallStatisticsData() = default;;
    void ResetData()  {
        video_stall_count_ = 0;
        video_stall_duration_ = 0;
        video_stall_count200_ = 0;
        video_stall_duration200_ = 0;
        render_frame_rate_ = 0;
    }
public:
    // the number of stalls in a statistical period (the stall threshold is 500ms)
    int video_stall_count_ = 0;
    // stall time in a statistical period (the stall threshold is 500ms)
    int video_stall_duration_ = 0;
    // the number of stalls in a statistical period (the stall threshold is 200ms)
    int video_stall_count200_ = 0;
    // stall time in a statistical period (the stall threshold is 200ms)
    int video_stall_duration200_ = 0;
    // rendering frame rate in a statistical period
    int render_frame_rate_ = 0;
    // the actual pull time in a statistical period
    // int64_t video_pull_duration_ = 0;
};


class VideoSinkObserver {
public:
    virtual void OnFirstVideoFrameRendered() = 0;
};

class VideoSinkProxy : public rtc::VideoSinkInterface<webrtc::VideoFrame> {
public:
    VideoSinkProxy(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> video_sink,
                   VideoSinkObserver* observer, IRtcEventHandler *event_handler);
    ~VideoSinkProxy();
    void OnFrame(const webrtc::VideoFrame &frame) override;
    void OnDiscardedFrame() override;
    void OnConstraintsChanged(const webrtc::VideoTrackSourceConstraints& constraints) override;
    
    void CheckRenderVideoFrame();
    StallStatisticsData* GetStallStatisticsData();
    void ResetStallStatisticsData();
    void UpdateStreamId(const std::string& stream_id);
    
private:
    void StallStatistic();
    void GetPullDurationAndFps(int64_t now);
    void CopyData4Reporting();
    
    std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> video_sink_;
    VideoSinkObserver* observer_;
    bool is_first_frame_rendered_ = false;
    IRtcEventHandler* event_handler_ = nullptr;
    std::string stream_id_;
    
    int64_t last_video_frame_timestamp_ = 0;
    int64_t last_video_frame_timestamp200_ = 0;
    int64_t last_check_timestamp_ = 0;
    int64_t render_frame_ = 0;
    
    bool video_frozen_ = false;
    bool video_frozen200_ = false;
    bool recv_first_video_frame_ = false;
    
    std::unique_ptr<StallStatisticsData> stall_statistics_data_;
    std::unique_ptr<StallStatisticsData> stall_statistics_data4report_;
};

}
