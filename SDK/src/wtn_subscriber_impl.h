/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "wtn_webrtc_wrapper.h"

namespace vewtn {

class Subscriber : public ISubscriber {
public:
    Subscriber(IRtcEventHandler* event_handler, const SubExtraParam& sub_extra_param);
    virtual ~Subscriber();

    int StartSubscribe(const char* url) override;
    int StopSubscribe() override;
    bool IsSubscribed() override;

    int SetEnableSpeaker(bool enable) override;
    void SetRemoteView(void* remote_sink) override;
    int SetRemoteAudioMute(bool mute) override;
    int SetRemoteVideoMute(bool mute) override;

private:
    webrtc::Mutex mutex_;
    std::unique_ptr<rtc::Thread> thread_;
    rtc::scoped_refptr<WebRtcWrapper> webrtc_wrapper_;
    bool is_subscribed_ = false;
    std::string url_;
};

}  // namespace vewtn
