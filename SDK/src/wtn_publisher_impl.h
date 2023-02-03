/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "wtn_webrtc_wrapper.h"

namespace vewtn {

class Publisher : public IPublisher {
public:
    Publisher(IRtcEventHandler* event_handler, const PubExtraParam& pub_extra_param);
    virtual ~Publisher();

    int StartPublish(const char* url) override;
    int StopPublish() override;
    bool IsPublishing() override;
    void SetLocalView(void* local_view) override;

    int SetVideoConfig(const VideoConfig& video_config) override;
    int StartVideoCapture() override;
    int StopVideoCapture() override;
    int SwitchCamera() override;
    int StartAudioCapture() override;
    int StopAudioCapture() override;

    int SetLocalAudioMute(bool mute) override;
    int SetLocalVideoMute(bool mute) override;
    bool PushEncodedVideoFrame(const EncodedVideoFrame& h264_encoded_frame) override;
    bool PushVideoFrame(const VideoFrame& yuv_video_frame) override;
    bool PushAudioFrame(const AudioFrame& pcm_audio_frame) override;

private:
    webrtc::Mutex mutex_;
    std::unique_ptr<rtc::Thread> thread_;
    rtc::scoped_refptr<WebRtcWrapper> webrtc_wrapper_;
    bool is_publishing_ = false;
    std::string url_;
};

}  // namespace vewtn
