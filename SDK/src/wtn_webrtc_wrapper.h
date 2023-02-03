/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

#include "api/peer_connection_interface.h"
#include "wtn_interface.h"
#include "external_module/external_audio_device_module.h"
#include "external_module/fake_encoder.h"
#include "external_module/external_video_source.h"
#include "external_module/video_sink_proxy.h"
#include "utils/whip_client.h"
#include "wtn_stats_callback.h"
#include "wtn_event_handler.h"

namespace vewtn {

class WebRtcWrapper : public webrtc::PeerConnectionObserver,
                      public webrtc::CreateSessionDescriptionObserver,
                      public WhipClientObserver,
                      public VideoSinkObserver {
public:
    WebRtcWrapper(IRtcEventHandler* event_handler, bool is_pub, PubExtraParam pub_extra_param, SubExtraParam sub_extra_param);
    virtual ~WebRtcWrapper();

    void Start(const std::string& url);
    void Stop();
    void SetLocalView(void* local_view);
    void SetRemoteView(void* remote_view);
    int SetVideoConfig(VideoConfig video_config);
    int StartVideoCapture();
    int StopVideoCapture();
    int SwitchCamera();
    int StartAudioCapture();
    int StopAudioCapture();
    int SetEnableSpeaker(bool enable);
    int SetAudioMute(bool mute);
    int SetVideoMute(bool mute);
    bool PushEncodedVideoFrame(EncodedVideoFrame h264_encoded_frame);
    bool PushVideoFrame(VideoFrame yuv_video_frame);
    bool PushAudioFrame(AudioFrame pcm_audio_frame);

private:
    void SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source);
    void CreatePeerConnectionFactory();
    void PreMuteProcess();
    void CreatePeerConnection();
    void Connect();
    void StartStats();
    void SetupInternalVideoTrack();
    void OnIceStageChanged(IceStage stage);
    void OnError(ErrorCode error);
    void OnStateChanged(State state, Reason reason);
    void ParserStreamId();
    std::vector<std::string> Split(std::string s, std::string delimiter);

    // PeerConnectionObserver implementation
    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;
    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
    void OnRenegotiationNeeded() override;
    void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
    void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override;
    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override;
    void OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) override;
    
    // CreateSessionDescriptionObserver implementation.
    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
    void OnFailure(webrtc::RTCError error) override;

    // WhipClientObserver implementation
    void OnStartSuccess(std::string sdp) override;
    void OnStartFailed(Reason reason) override;
    void OnStopSuccess() override;
    void OnStopFailed(Reason reason) override;
    void OnMuteAudioSuccess() override;
    void OnMuteAudioFailed(Reason reason) override;
    void OnMuteVideoSuccess() override;
    void OnMuteVideoFailed(Reason reason) override;

    // VideoSinkProxy implementation
    void OnFirstVideoFrameRendered() override;

private:
    class SetLocalSessionDescriptionObserver;
    class SetRemoteSessionDescriptionObserver;

    webrtc::SequenceChecker thread_checker_;
    webrtc::Mutex mutex_;
    bool call_started_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pcf_;
    std::unique_ptr<rtc::Thread> network_thread_;
    std::unique_ptr<rtc::Thread> worker_thread_;
    std::unique_ptr<rtc::Thread> signaling_thread_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc_;
    rtc::scoped_refptr<webrtc::VideoTrackInterface> local_video_track_;
    rtc::scoped_refptr<webrtc::AudioTrackInterface> local_audio_track_;
    std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> local_sink_;
    std::unique_ptr<VideoSinkProxy> remote_sink_;
    std::unique_ptr<rtc::TaskQueue> task_queue_;

    FakeEncoder* fake_encoder_ = nullptr;
    rtc::scoped_refptr<ExternalAudioDeviceModule> external_adm_;
    rtc::scoped_refptr<webrtc::AudioDeviceModule> internal_adm_;
    rtc::scoped_refptr<ExternalVideoTrackSource> external_video_source_;
    rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> internal_video_source_;
    rtc::scoped_refptr<RTCStatsCallback> stats_callback_;
    webrtc::TaskQueueFactory* task_queue_factory_;

    std::unique_ptr<WhipClient> whip_client_;
    IRtcEventHandler* event_handler_ = nullptr;
    bool is_publisher_ = true;
    PubExtraParam pub_extra_param_;
    SubExtraParam sub_extra_param_;
    std::string stream_id_;
    std::string start_url_;
    std::string audio_mute_params_;
    std::string video_mute_params_;
    std::string sdp_;
    VideoConfig video_config_;
    bool if_front_ = true;
    bool is_start_recording_ = false;
    std::unique_ptr<rtc::Thread> callback_thread_;
    std::unique_ptr<EventHandler> callback_;
};

}  // namespace vewtn
