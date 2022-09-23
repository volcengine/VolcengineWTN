/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "api/scoped_refptr.h"
#include "api/sequence_checker.h"
#include "rtc_base/synchronization/mutex.h"
#include "whip_client.h"
#include "vewtn_interface.h"

namespace vewtn {

class WebRTCWrapper : public webrtc::PeerConnectionObserver,
public webrtc::CreateSessionDescriptionObserver,
public WhipClientObserver  {
public:
    WebRTCWrapper(IPublisherEventHandler* pub_handler, ISubscriberEventHandler* sub_handler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                  std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                  std::unique_ptr<webrtc::VideoDecoderFactory> dec);
    ~WebRTCWrapper();
    
    void Start(const std::string& url);
    void Stop();
    
    void SetLocalVideoSink(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& local_sink) {
        local_sink_ = std::move(local_sink);
    }
    void SetRemoteVideoSink(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& remote_sink) {
        remote_sink_ = std::move(remote_sink);
    }
    void SetupVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) {
        if (video_source) {
            local_video_track_ = pcf_->CreateVideoTrack("video", video_source.get());
            if (local_sink_) {
                local_video_track_->AddOrUpdateSink(local_sink_.get(), rtc::VideoSinkWants());
            }
        }
    }
    
    rtc::Thread* GetSignalingThread() {
        return signaling_thread_.get();
    }
    
    rtc::Thread* GetWorkerThread() {
        return worker_thread_.get();
    }
    
private:
    void CreatePeerConnectionFactory() RTC_RUN_ON(thread_checker_);
    void CreatePeerConnection() RTC_RUN_ON(thread_checker_);
    void Connect() RTC_RUN_ON(thread_checker_);
    
    // PeerConnectionObserver implementation
    void OnSignalingChange(
                           webrtc::PeerConnectionInterface::SignalingState new_state) override;
    void OnDataChannel(
                       rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
    void OnRenegotiationNeeded() override;
    void OnIceConnectionChange(
                               webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
    void OnIceGatheringChange(
                              webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
    void OnFailure(webrtc::RTCError error) override;
    
    // WhipClientObserver implementation
    void OnStartSuccess(std::string sdp) override;
    void OnStartFailed(Reason reason) override;
    void OnStopSuccess() override;
    void OnStopFailed(Reason reason) override;
    
private:
    class SetLocalSessionDescriptionObserver;
    class SetRemoteSessionDescriptionObserver;
    
    webrtc::SequenceChecker thread_checker_;
    bool call_started_ RTC_GUARDED_BY(thread_checker_);
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pcf_
    RTC_GUARDED_BY(thread_checker_);
    std::unique_ptr<rtc::Thread> network_thread_ RTC_GUARDED_BY(thread_checker_);
    std::unique_ptr<rtc::Thread> worker_thread_ RTC_GUARDED_BY(thread_checker_);
    std::unique_ptr<rtc::Thread> signaling_thread_
    RTC_GUARDED_BY(thread_checker_);
    std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> local_sink_
    RTC_GUARDED_BY(thread_checker_);
    std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> remote_sink_
    RTC_GUARDED_BY(thread_checker_);
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc_;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> local_video_track_;
    IPublisherEventHandler* publisher_handler_;
    ISubscriberEventHandler* subscriber_handler_;
    std::unique_ptr<WhipClient> whip_client_;
    std::string start_url_;
    std::string sdp_;
    rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule_;
    std::unique_ptr<webrtc::VideoEncoderFactory> enc_;
    std::unique_ptr<webrtc::VideoDecoderFactory> dec_;
};
}

