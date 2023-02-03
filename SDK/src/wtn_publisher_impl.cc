/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "wtn_publisher_impl.h"

namespace vewtn {

Publisher::Publisher(IRtcEventHandler* event_handler, const PubExtraParam& pub_extra_param) {
    thread_ = rtc::Thread::Create();
    thread_->SetName("pub_worker_thread", nullptr);
    thread_->Start();
//    thread_->Invoke<void>(RTC_FROM_HERE, [this, event_handler, pub_extra_param]() {
        webrtc_wrapper_ = rtc::make_ref_counted<WebRtcWrapper>(event_handler, true, pub_extra_param, SubExtraParam());
//    });
}

Publisher::~Publisher() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::~Publisher begin";
    thread_->Invoke<void>(RTC_FROM_HERE, [this]() {
        RTC_LOG(LS_INFO) << "[WTN_API]Publisher::~Publisher Invoke";
        webrtc_wrapper_ = nullptr;
    });
    if (thread_) {
        thread_->Stop();
        thread_.reset(nullptr);
    }
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::~Publisher end";
}

int Publisher::StartPublish(const char* url) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StartPublish, url: " << url;
    if (url == nullptr) {
        return -1;
    }
    url_ = url;
    is_publishing_ = true;
    thread_->PostTask([&]() {
        webrtc_wrapper_->Start(url_);
    });
    return 0;
}

int Publisher::StopPublish() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StopPublish";
    is_publishing_ = false;
    thread_->PostTask([&]() {
        webrtc_wrapper_->Stop();
    });
    return 0;
}

bool Publisher::IsPublishing() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::IsPublishing, is_publishing_: " << is_publishing_;
    return is_publishing_;
}

void Publisher::SetLocalView(void* local_view) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::SetLocalView, local_view: " << local_view;
    thread_->PostTask([this, local_view]() {
        webrtc_wrapper_->SetLocalView(local_view);
    });
}

int Publisher::SetVideoConfig(const VideoConfig& video_config) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::SetVideoConfig"
                     << ", width: " << video_config.width
                     << ", height: " << video_config.height
                     << ", max_fpas: " << video_config.max_fps;
    thread_->PostTask([this, video_config]() {
        webrtc_wrapper_->SetVideoConfig(video_config);
    });
    return 0;
}

int Publisher::StartVideoCapture() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StartVideoCapture";
    thread_->PostTask([&]() {
        webrtc_wrapper_->StartVideoCapture();
    });
    return 0;
}

int Publisher::StopVideoCapture() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StopVideoCapture";
    thread_->PostTask([&]() {
        webrtc_wrapper_->StopVideoCapture();
    });
    return 0;
}

int Publisher::SwitchCamera() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::SwitchCamera";
    thread_->PostTask([&]() {
        webrtc_wrapper_->SwitchCamera();
    });
    return 0;
}

int Publisher::StartAudioCapture() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StartAudioCapture";
    thread_->PostTask([&]() {
        webrtc_wrapper_->StartAudioCapture();
    });
    return 0;
}

int Publisher::StopAudioCapture() {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::StopAudioCapture";
    thread_->PostTask([&]() {
        webrtc_wrapper_->StopAudioCapture();
    });
    return 0;
}

int Publisher::SetLocalAudioMute(bool mute) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::SetLocalAudioMute, mute: " << mute;
    thread_->PostTask([this, mute]() {
        webrtc_wrapper_->SetAudioMute(mute);
    });
    return 0;
}

int Publisher::SetLocalVideoMute(bool mute) {
    webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]Publisher::SetLocalVideoMute, mute: " << mute;
    thread_->PostTask([this, mute]() {
        webrtc_wrapper_->SetVideoMute(mute);
    });
    return 0;
}

bool Publisher::PushEncodedVideoFrame(const EncodedVideoFrame& h264_encoded_frame) {
    webrtc::MutexLock lock(&mutex_);
    if (!thread_) {
        return false;
    }
    return thread_->Invoke<bool>(RTC_FROM_HERE, [this, h264_encoded_frame]() {
        return webrtc_wrapper_->PushEncodedVideoFrame(h264_encoded_frame);
    });
}

bool Publisher::PushVideoFrame(const VideoFrame& yuv_video_frame) {
    webrtc::MutexLock lock(&mutex_);
    if (!thread_) {
        return false;
    }
    return thread_->Invoke<bool>(RTC_FROM_HERE, [this, yuv_video_frame]() {
        return webrtc_wrapper_->PushVideoFrame(yuv_video_frame);
    });
}

bool Publisher::PushAudioFrame(const AudioFrame& pcm_audio_frame) {
    webrtc::MutexLock lock(&mutex_);
    if (!thread_) {
        return false;
    }
    return thread_->Invoke<bool>(RTC_FROM_HERE, [this, pcm_audio_frame]() {
       return webrtc_wrapper_->PushAudioFrame(pcm_audio_frame);
    });
}

}  // end of namespace
