/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "wtn_engine.h"
#include "wtn_publisher_impl.h"
#include "wtn_subscriber_impl.h"

namespace vewtn {

RtcEngine* RtcEngine::rtc_instance_ = nullptr;
ExtendedParameters global_extend_param;

VEWTN_API void SetExtendedParameters(ExtendedParameters param) {
    global_extend_param = param;
}

VEWTN_API IRtcEngine* CreateRtcEngine(IRtcEventHandler* event_handler) {
    return RtcEngine::GetInstance(event_handler);
}

VEWTN_API void DestroyRtcEngine(const IRtcEngine* engine_ptr) {
    RtcEngine::ReleaseInstance(engine_ptr);
}

IRtcEngine* RtcEngine::GetInstance(IRtcEventHandler* event_handler) {
    if (rtc_instance_ == nullptr) {
        rtc_instance_ = new RtcEngine(event_handler);
    }
    return rtc_instance_;
}

void RtcEngine::ReleaseInstance(const IRtcEngine* engine_ptr) {
    if (rtc_instance_ == engine_ptr) {
        delete rtc_instance_;
        rtc_instance_ = nullptr;
    }
}

RtcEngine::RtcEngine(IRtcEventHandler* event_handler)
    : event_handler_(event_handler) {

}

RtcEngine::~RtcEngine() {
    webrtc::MutexLock lock(&mutex_);
    for (auto iter : pub_maps_) {
        delete iter.first;
    }
    pub_maps_.clear();
    for (auto iter : pub_maps_) {
        delete iter.first;
    }
    pub_maps_.clear();
}

bool RtcEngine::CheckInternalDeviceIsUsed(const PubExtraParam& pub_extra_param) {
    if (pub_extra_param.pub_audio_source == PubAudioSource::InternalRecord) {
        for (auto iter : pub_maps_) {
            // already has one
            if (iter.second.pub_audio_source == PubAudioSource::InternalRecord) {
                return true;
            }
        }
    }
    if (pub_extra_param.pub_video_source == PubVideoSource::InternalCamera) {
        for (auto iter : pub_maps_) {
            // already has one
            if (iter.second.pub_video_source == PubVideoSource::InternalCamera) {
                return true;
            }
        }
    }
    return false;
}

IPublisher* RtcEngine::CreatePublisher(const PubExtraParam& pub_extra_param) {
    webrtc::MutexLock lock(&mutex_);
    // check, only allowed one internal device
    if (CheckInternalDeviceIsUsed(pub_extra_param)) {
        return nullptr;
    }
    IPublisher* pub = new Publisher(event_handler_, pub_extra_param);
    pub_maps_[pub] = pub_extra_param;
    return pub;
}

void RtcEngine::DestroyPublisher(const IPublisher* publisher_ptr) {
    if (!publisher_ptr) {
        return;
    }
    webrtc::MutexLock lock(&mutex_);
    if(pub_maps_.find(publisher_ptr) != pub_maps_.end()){
        delete publisher_ptr;
        pub_maps_.erase(publisher_ptr);
    }
}

ISubscriber* RtcEngine::CreateSubscriber(const SubExtraParam& sub_extra_param) {
    webrtc::MutexLock lock(&mutex_);
    ISubscriber* sub = new Subscriber(event_handler_, sub_extra_param);
    sub_maps_[sub] = sub_extra_param;
    return sub;
}

void RtcEngine::DestroySubscriber(const ISubscriber* subscriber_ptr) {
    if (!subscriber_ptr) {
        return;
    }
    webrtc::MutexLock lock(&mutex_);
    if(sub_maps_.find(subscriber_ptr) != sub_maps_.end()){
        delete subscriber_ptr;
        sub_maps_.erase(subscriber_ptr);
    }
}

}    // end of namespace
