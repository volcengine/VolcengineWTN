/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "wtn_subscriber_impl.h"

namespace vewtn {

Subscriber::Subscriber(IRtcEventHandler* event_handler, const SubExtraParam& sub_extra_param) {
    thread_ = rtc::Thread::Create();
    thread_->SetName("sub_worker_thread", nullptr);
    thread_->Start();
    thread_->Invoke<void>(RTC_FROM_HERE, [this, event_handler, sub_extra_param]() {
        webrtc_wrapper_ = rtc::make_ref_counted<WebRtcWrapper>(event_handler, false, PubExtraParam(), sub_extra_param);
    });
}

Subscriber::~Subscriber() {
    webrtc::MutexLock lock(&mutex_);
    thread_->Invoke<void>(RTC_FROM_HERE, [this]() {
        webrtc_wrapper_ = nullptr;
    });
    if (thread_) {
        thread_->Stop();
        thread_.reset(nullptr);
    }
}

int Subscriber::StartSubscribe(const char* url) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::StartSubscribe, url: " << url;
    if (url == nullptr) {
        return -1;
    }
    url_ = url;
    is_subscribed_ = true;
    thread_->PostTask([&]() {
        webrtc_wrapper_->Start(url_);
    });
    return 0;
}

int Subscriber::StopSubscribe() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::StopSubscribe";
    is_subscribed_ = false;
    thread_->PostTask([&]() {
        webrtc_wrapper_->Stop();
    });
    return 0;
}

bool Subscriber::IsSubscribed() {
    return is_subscribed_;
}

int Subscriber::SetEnableSpeaker(bool enable) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::SetEnableSpeaker, enable: " << enable;
    thread_->PostTask([this, enable]() {
        webrtc_wrapper_->SetEnableSpeaker(enable);
    });
    return 0;
}

void Subscriber::SetRemoteView(void* remote_sink) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::SetRemoteView, remote_sink: " << remote_sink;
    thread_->PostTask([this, remote_sink]() {
        webrtc_wrapper_->SetRemoteView(remote_sink);
    });
}

int Subscriber::SetRemoteAudioMute(bool mute) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::SetRemoteAudioMute, mute: " << mute;
    thread_->PostTask([this, mute]() {
        webrtc_wrapper_->SetAudioMute(mute);
    });
    return 0;
}

int Subscriber::SetRemoteVideoMute(bool mute) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Subscriber::SetRemoteVideoMute, mute: " << mute;
    thread_->PostTask([this, mute]() {
        webrtc_wrapper_->SetVideoMute(mute);
    });
    return 0;
}

}    // end of namespace
