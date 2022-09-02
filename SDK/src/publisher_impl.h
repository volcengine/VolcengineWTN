/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "vewtn_interface.h"
#include "webrtc_wrapper.h"
#include <memory>
#include "api/video_codecs/video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"

namespace vewtn {

class Publisher : public IPublisher {
public:
    Publisher(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
              std::unique_ptr<webrtc::VideoEncoderFactory> enc,
              std::unique_ptr<webrtc::VideoDecoderFactory> dec);
    ~Publisher();
    
    static Publisher* GetInstance(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                  std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                  std::unique_ptr<webrtc::VideoDecoderFactory> dec);
    
    void SetLocalCanvas(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& local_sink) override;
    void SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) override;
    int StartPublish(const char* url) override;
    int StopPublish() override;
    bool IsPublishing() override;
    rtc::Thread* GetSignalingThread() override;
    rtc::Thread* GetWorkerThread() override;
    
private:
    static Publisher* mInstance;
    IPublisherEventHandler* handler_;
    rtc::scoped_refptr<WebRTCWrapper> webrtc_wrapper_;
    bool is_publishing = false;
};

} // namespace vewtn
