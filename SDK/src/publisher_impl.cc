/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "publisher_impl.h"

namespace vewtn {

Publisher* Publisher::mInstance = nullptr;

VEWTN_API IPublisher* CreatePublisher(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                      std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                      std::unique_ptr<webrtc::VideoDecoderFactory> dec) {
    IPublisher* engine = Publisher::GetInstance(eventHandler, audioDeviceModule, std::move(enc), std::move(dec));
    return engine;
}

VEWTN_API void DestroyPublisher(IPublisher* publisherPtr) {
    if (publisherPtr) {
        delete publisherPtr;
        publisherPtr = nullptr;
    }
}

Publisher* Publisher::GetInstance(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                  std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                  std::unique_ptr<webrtc::VideoDecoderFactory> dec) {
    if (mInstance == nullptr) {
        mInstance = new Publisher(eventHandler, audioDeviceModule, std::move(enc), std::move(dec));
    }
    return mInstance;
}

Publisher::Publisher(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                     std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                     std::unique_ptr<webrtc::VideoDecoderFactory> dec)
: handler_(eventHandler) {
    webrtc_wrapper_ = rtc::make_ref_counted<WebRTCWrapper>(handler_, nullptr, audioDeviceModule, std::move(enc), std::move(dec));
}

Publisher::~Publisher() {
    webrtc_wrapper_ = nullptr;
    mInstance = nullptr;
}

void Publisher::SetLocalCanvas(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& local_sink) {
    webrtc_wrapper_->SetLocalVideoSink(local_sink);
}

void Publisher::SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) {
    webrtc_wrapper_->SetupVideoSource(video_source);
}

int Publisher::StartPublish(const char* url) {
    is_publishing = true;
    webrtc_wrapper_->Start(url);
    return 0;
}

int Publisher::StopPublish() {
    is_publishing = false;
    webrtc_wrapper_->Stop();
    return 0;
}

bool Publisher::IsPublishing() {
    return is_publishing;
}

rtc::Thread* Publisher::GetSignalingThread() {
    return webrtc_wrapper_->GetSignalingThread();
}

rtc::Thread* Publisher::GetWorkerThread() {
    return webrtc_wrapper_->GetWorkerThread();
}

}
