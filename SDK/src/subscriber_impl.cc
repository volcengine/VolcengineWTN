/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "subscriber_impl.h"

namespace vewtn {

Subscriber* Subscriber::s_subscriber_instance = nullptr;

VEWTN_API ISubscriber* CreateSubscriber(ISubscriberEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                        std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                        std::unique_ptr<webrtc::VideoDecoderFactory> dec) {
    ISubscriber* engine = Subscriber::GetInstance(eventHandler, audioDeviceModule, std::move(enc), std::move(dec));
    return engine;
}

VEWTN_API void DestroySubscriber(ISubscriber* subscriberPtr) {
    if (subscriberPtr){
        delete subscriberPtr;
        subscriberPtr = nullptr;
    }
}

Subscriber* Subscriber::GetInstance(ISubscriberEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                    std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                    std::unique_ptr<webrtc::VideoDecoderFactory> dec) {
    if (s_subscriber_instance == nullptr) {
        s_subscriber_instance = new Subscriber(eventHandler, audioDeviceModule, std::move(enc), std::move(dec));
    }
    return s_subscriber_instance;
}

Subscriber::~Subscriber() {
    webrtc_wrapper_ = nullptr;
    s_subscriber_instance = nullptr;
}

Subscriber::Subscriber(ISubscriberEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                       std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                       std::unique_ptr<webrtc::VideoDecoderFactory> dec)
: handler_(eventHandler) {
    webrtc_wrapper_ = rtc::make_ref_counted<WebRTCWrapper>(nullptr, handler_, audioDeviceModule, std::move(enc), std::move(dec));
}

void Subscriber::SetRemoteCanvas(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& remote_sink) {
    webrtc_wrapper_->SetRemoteVideoSink(remote_sink);
}

void Subscriber::SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) {
    webrtc_wrapper_->SetupVideoSource(video_source);
}

int Subscriber::StartSubscribe(const char* url) {
    is_subscribed = true;
    webrtc_wrapper_->Start(url);
    return 0;
}

int Subscriber::StopSubscribe() {
    is_subscribed = false;
    webrtc_wrapper_->Stop();
    return 0;
}

bool Subscriber::IsSubscribed() {
    return is_subscribed;
}

rtc::Thread* Subscriber::GetSignalingThread() {
    return webrtc_wrapper_->GetSignalingThread();
}

rtc::Thread* Subscriber::GetWorkerThread() {
    return webrtc_wrapper_->GetWorkerThread();
}

}
