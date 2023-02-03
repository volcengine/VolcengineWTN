/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "pc/video_track_source.h"
#include "media/base/video_broadcaster.h"

namespace vewtn {

class ExternalVideoSource final : public rtc::VideoSourceInterface<webrtc::VideoFrame> {
public:
    ExternalVideoSource();
    virtual ~ExternalVideoSource() {}

    void RemoveSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) override;
    void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink,
                         const rtc::VideoSinkWants& wants) override;
    bool PushVideoFrame(VideoFrame yuv_video_frame);
    bool FakeYuv(int width, int height, long timestamp);

private:
    webrtc::SequenceChecker thread_checker_;
    rtc::VideoBroadcaster broadcaster_;
    mutable webrtc::Mutex mutex_;
    rtc::VideoSinkWants wants_ RTC_GUARDED_BY(&mutex_);
};

class ExternalVideoTrackSource : public webrtc::VideoTrackSource {
public:
    explicit ExternalVideoTrackSource(bool remote)
        : webrtc::VideoTrackSource(remote) {}
    ~ExternalVideoTrackSource() {
        
    }
    
    bool PushVideoFrame(VideoFrame video_frame) {
        return source_.PushVideoFrame(video_frame);
    }
    bool FakeYuv(int width, int height, long timestamp) {
        return source_.FakeYuv(width, height, timestamp);
    }

protected:
    rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override {
        return &source_;
    }

private:
    ExternalVideoSource source_;
};

}    // end of namespace
