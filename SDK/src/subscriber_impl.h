/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "vewtn_interface.h"
#include "webrtc_wrapper.h"
#include <memory>

namespace vewtn {

class Subscriber : public ISubscriber {
public:
    Subscriber(ISubscriberEventHandler *eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
               std::unique_ptr<webrtc::VideoEncoderFactory> enc,
               std::unique_ptr<webrtc::VideoDecoderFactory> dec);
    
    ~Subscriber();
    
    static Subscriber *GetInstance(ISubscriberEventHandler *eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                   std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                   std::unique_ptr<webrtc::VideoDecoderFactory> dec);
    
    void SetRemoteCanvas(
                         std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> &remote_sink) override;
    
    void SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) override;
    
    int StartSubscribe(const char *url) override;
    
    int StopSubscribe() override;
    
    bool IsSubscribed() override;
    
    rtc::Thread* GetSignalingThread() override;
    
    rtc::Thread* GetWorkerThread() override;
    
private:
    static Subscriber *s_subscriber_instance;
    ISubscriberEventHandler *handler_;
    rtc::scoped_refptr<WebRTCWrapper> webrtc_wrapper_;
    bool is_subscribed = false;
};

}
