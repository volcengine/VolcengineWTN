/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "webrtc_wrapper.h"
#include <utility>
#include <memory>

#include "api/audio_codecs/audio_decoder_factory.h"
#include "api/audio_codecs/audio_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "api/peer_connection_interface.h"
#include "api/rtc_event_log/rtc_event_log_factory.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "media/engine/internal_decoder_factory.h"
#include "media/engine/internal_encoder_factory.h"
#include "media/engine/webrtc_media_engine.h"
#include "media/engine/webrtc_media_engine_defaults.h"

namespace vewtn {

class WebRTCWrapper::SetRemoteSessionDescriptionObserver
        : public webrtc::SetRemoteDescriptionObserverInterface {
public:
    SetRemoteSessionDescriptionObserver(WebRTCWrapper* wrapper)
            : wrapper_(wrapper) {}
    void OnSetRemoteDescriptionComplete(webrtc::RTCError error) override;
    
private:
    WebRTCWrapper* wrapper_;
};

class WebRTCWrapper::SetLocalSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
public:
    SetLocalSessionDescriptionObserver(WebRTCWrapper* wrapper)
            : wrapper_(wrapper) {}
    void OnSuccess() override;
    void OnFailure(webrtc::RTCError error) override;
private:
    WebRTCWrapper* wrapper_;
};

WebRTCWrapper::WebRTCWrapper(IPublisherEventHandler* pub_handler, ISubscriberEventHandler* sub_handler,
                             rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                             std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                             std::unique_ptr<webrtc::VideoDecoderFactory> dec)
        : call_started_(false),
        publisher_handler_(pub_handler),
        subscriber_handler_(sub_handler),
        audioDeviceModule_(audioDeviceModule),
        enc_(std::move(enc)),
        dec_(std::move(dec)) {
    whip_client_ = std::make_unique<WhipClient>(this);
    thread_checker_.Detach();
    CreatePeerConnectionFactory();
}

WebRTCWrapper::~WebRTCWrapper() {
    if (whip_client_) {
        whip_client_.reset();
    }
    publisher_handler_ = nullptr;
    subscriber_handler_ = nullptr;
    
    if (pc_ != nullptr) {
        pc_->Close();
        pc_ = nullptr;
    }
    pcf_ = nullptr;
    local_sink_ = nullptr;
    remote_sink_ = nullptr;
}

void WebRTCWrapper::Start(const std::string& url) {
    RTC_DCHECK_RUN_ON(&thread_checker_);
    RTC_LOG(LS_INFO) << "Start";

    if (call_started_) {
        RTC_LOG(LS_WARNING) << "Call already started.";
        return;
    }
    call_started_ = true;
    start_url_ = url;
    
    CreatePeerConnection();
    Connect();
}

void WebRTCWrapper::Stop() {
    RTC_DCHECK_RUN_ON(&thread_checker_);
    RTC_LOG(LS_INFO) << "Stop";

    if (!call_started_) {
        RTC_LOG(LS_WARNING) << "Not yet started.";
        return;
    }
    
    call_started_ = false;
    
    if (whip_client_) {
        whip_client_->Stop();
    }
    if (pc_ != nullptr) {
        pc_->Close();
        pc_ = nullptr;
    }
}

void WebRTCWrapper::CreatePeerConnectionFactory() {
    network_thread_ = rtc::Thread::CreateWithSocketServer();
    network_thread_->SetName("network_thread", nullptr);
    RTC_CHECK(network_thread_->Start()) << "Failed to start thread";
    
    worker_thread_ = rtc::Thread::Create();
    worker_thread_->SetName("worker_thread", nullptr);
    RTC_CHECK(worker_thread_->Start()) << "Failed to start thread";
    
    signaling_thread_ = rtc::Thread::Create();
    signaling_thread_->SetName("signaling_thread", nullptr);
    RTC_CHECK(signaling_thread_->Start()) << "Failed to start thread";
    
    webrtc::PeerConnectionFactoryDependencies pcf_deps;
    pcf_deps.network_thread = network_thread_.get();
    pcf_deps.worker_thread = worker_thread_.get();
    pcf_deps.signaling_thread = signaling_thread_.get();
    pcf_deps.task_queue_factory = webrtc::CreateDefaultTaskQueueFactory();
    pcf_deps.call_factory = webrtc::CreateCallFactory();
    pcf_deps.event_log_factory = std::make_unique<webrtc::RtcEventLogFactory>(
                                                                              pcf_deps.task_queue_factory.get());
    
    cricket::MediaEngineDependencies media_deps;
    if (audioDeviceModule_){
        media_deps.adm = audioDeviceModule_;
    }
    
    media_deps.task_queue_factory = pcf_deps.task_queue_factory.get();
    media_deps.video_encoder_factory =
    std::make_unique<webrtc::InternalEncoderFactory>();
    media_deps.video_decoder_factory =
    std::make_unique<webrtc::InternalDecoderFactory>();
    
    if (media_deps.task_queue_factory == nullptr) {
        static webrtc::TaskQueueFactory* const task_queue_factory =
        webrtc::CreateDefaultTaskQueueFactory().release();
        media_deps.task_queue_factory = task_queue_factory;
    }
    if (media_deps.audio_encoder_factory == nullptr)
        media_deps.audio_encoder_factory = webrtc::CreateBuiltinAudioEncoderFactory();
    if (media_deps.audio_decoder_factory == nullptr)
        media_deps.audio_decoder_factory = webrtc::CreateBuiltinAudioDecoderFactory();
    if (media_deps.audio_processing == nullptr)
        media_deps.audio_processing = webrtc::AudioProcessingBuilder().Create();

    if (enc_){
        media_deps.video_encoder_factory = std::move(enc_);
    } else {
        media_deps.video_encoder_factory = webrtc::CreateBuiltinVideoEncoderFactory();
    }
    if (dec_) {
        media_deps.video_decoder_factory = std::move(dec_);
    } else {
        media_deps.video_decoder_factory = webrtc::CreateBuiltinVideoDecoderFactory();
    }

    pcf_deps.media_engine = cricket::CreateMediaEngine(std::move(media_deps));
    RTC_LOG(LS_INFO) << "Media engine created: " << pcf_deps.media_engine.get();
    
    pcf_ = CreateModularPeerConnectionFactory(std::move(pcf_deps));
    RTC_LOG(LS_INFO) << "PeerConnectionFactory created: " << pcf_.get();
}

void WebRTCWrapper::CreatePeerConnection() {
    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    // default value can find at: src/media/base/media_config.h
    config.set_cpu_adaptation(false);
    // Encryption has to be disabled for loopback to work.
    webrtc::PeerConnectionFactoryInterface::Options options;
    options.disable_encryption = true;
    pcf_->SetOptions(options);
    webrtc::PeerConnectionDependencies deps(this);
    pc_ = pcf_->CreatePeerConnectionOrError(config, std::move(deps)).MoveValue();
    // pc_->SetAudioRecording(false);

    RTC_LOG(LS_INFO) << "PeerConnection created: " << pc_.get();

    if (local_video_track_) {
        pc_->AddTransceiver(local_video_track_);
    }

    rtc::scoped_refptr <webrtc::AudioSourceInterface> audio_source =
    pcf_->CreateAudioSource(cricket::AudioOptions());
    rtc::scoped_refptr <webrtc::AudioTrackInterface> local_audio_track =
    pcf_->CreateAudioTrack("audio", audio_source.get());
    pc_->AddTransceiver(local_audio_track);
    RTC_LOG(LS_INFO) << "Local audio sink set up: " << local_video_track_.get();

    if (remote_sink_) {
        for (const rtc::scoped_refptr<webrtc::RtpTransceiverInterface>& tranceiver :
             pc_->GetTransceivers()) {
            rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track =
            tranceiver->receiver()->track();
            if (track &&
                track->kind() == webrtc::MediaStreamTrackInterface::kVideoKind) {
                static_cast<webrtc::VideoTrackInterface*>(track.get())
                ->AddOrUpdateSink(remote_sink_.get(), rtc::VideoSinkWants());
                RTC_LOG(LS_INFO) << "Remote video sink set up: " << track.get();
                break;
            }
        }
    }
}

void WebRTCWrapper::Connect() {
    RTC_LOG(LS_INFO) << "CreateOffer"; 
    pc_->CreateOffer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}

void WebRTCWrapper::OnSignalingChange(
                                      webrtc::PeerConnectionInterface::SignalingState new_state) {
    RTC_LOG(LS_INFO) << "OnSignalingChange: " << new_state;
}

void WebRTCWrapper::OnDataChannel(
                                  rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {
    RTC_LOG(LS_INFO) << "OnDataChannel";
}

void WebRTCWrapper::OnRenegotiationNeeded() {
    RTC_LOG(LS_INFO) << "OnRenegotiationNeeded";
}

void WebRTCWrapper::OnIceConnectionChange(
                                          webrtc::PeerConnectionInterface::IceConnectionState new_state) {
    RTC_LOG(LS_INFO) << "OnIceConnectionChange: " << new_state;
}

void WebRTCWrapper::OnIceGatheringChange(
                                         webrtc::PeerConnectionInterface::IceGatheringState new_state) {
    RTC_LOG(LS_INFO) << "OnIceGatheringChange: " << new_state;
}

void WebRTCWrapper::OnIceCandidate(
                                   const webrtc::IceCandidateInterface* candidate) {
    RTC_LOG(LS_INFO) << "OnIceCandidate: " << candidate->server_url();
    //        webrtc::MutexLock lock(&client_->pc_mutex_);
    RTC_DCHECK(pc_ != nullptr);
    pc_->AddIceCandidate(candidate);
}

void WebRTCWrapper::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    desc->ToString(&sdp_);
    
    // profile-level-id only support 42e01f
    // example:
    // not support: a=fmtp:96 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=640c34
    // not support: a=fmtp:98 level-asymmetry-allowed=1;packetization-mode=1;profile-level-id=42e034
    size_t start_pos = sdp_.find("42e0");
    if(start_pos != std::string::npos){
        sdp_.replace(start_pos + 4, 2, "1f");
    }

#if defined(WEBRTC_ANDROID)
    RTC_LOG(LS_INFO) << "Created offer: " << sdp_.substr(0, sdp_.size()/4);
    RTC_LOG(LS_INFO) << sdp_.substr(sdp_.size()/4, sdp_.size()/4);
    RTC_LOG(LS_INFO) << sdp_.substr(sdp_.size()/2, sdp_.size()/4);
    RTC_LOG(LS_INFO) << sdp_.substr(sdp_.size()*3/4, sdp_.size()/4);
#else
    RTC_LOG(LS_INFO) << "Created offer: " << sdp_;
#endif
    
    // Ownership of desc was transferred to us, now we transfer it forward.
    pc_->SetLocalDescription(
                             rtc::make_ref_counted<SetLocalSessionDescriptionObserver>(this).get(), desc);
}

void WebRTCWrapper::OnFailure(webrtc::RTCError error) {
    RTC_LOG(LS_INFO) << "Failed to create offer: " << ToString(error.type())
    << ": " << error.message();
    
    Reason reason = Reason::SDKInternalError;
    if (publisher_handler_) {
        publisher_handler_->OnPublishStateChanged(PublishState::StartPublishFailed, reason);
    }
    if (subscriber_handler_) {
        subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StartSubscribeFailed, reason);
    }
}

void WebRTCWrapper::OnStartSuccess(std::string sdp) {
    RTC_LOG(LS_INFO) << "OnStartSuccess: " << sdp;
    if (pc_) {
        std::unique_ptr<webrtc::SessionDescriptionInterface> answer(
                                                                    webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp));
        pc_->SetRemoteDescription(
                                  std::move(answer),
                                  rtc::make_ref_counted<SetRemoteSessionDescriptionObserver>(this));
    }
}

void WebRTCWrapper::OnStartFailed(Reason reason) {
    RTC_LOG(LS_INFO) << "OnStartFailed, reason:" << (int)reason;
    if (publisher_handler_) {
        publisher_handler_->OnPublishStateChanged(PublishState::StartPublishFailed, reason);
    }
    if (subscriber_handler_) {
        subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StartSubscribeFailed, reason);
    }
}

void WebRTCWrapper::OnStopSuccess() {
    RTC_LOG(LS_INFO) << "OnStopSuccess";
    if (publisher_handler_) {
        publisher_handler_->OnPublishStateChanged(PublishState::StopPublishSuccess, Reason::Success);
    }
    if (subscriber_handler_) {
        subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StopSubscribeSuccess, Reason::Success);
    }
}

void WebRTCWrapper::OnStopFailed(Reason reason) {
    RTC_LOG(LS_INFO) << "OnStopFailed, reason:" << (int)reason;
    if (publisher_handler_) {
        publisher_handler_->OnPublishStateChanged(PublishState::StopPublishFailed, reason);
    }
    if (subscriber_handler_) {
        subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StopSubscribeFailed, reason);
    }
}

void WebRTCWrapper::SetLocalSessionDescriptionObserver::OnSuccess() {
    RTC_LOG(LS_INFO) << "Set local description success!";
    if (wrapper_ && wrapper_->whip_client_) {
        wrapper_->whip_client_->Start(wrapper_->start_url_, wrapper_->sdp_);
    }
}

void WebRTCWrapper::SetLocalSessionDescriptionObserver::OnFailure(webrtc::RTCError error) {
    RTC_LOG(LS_INFO) << "Set local description failure: "
    << ToString(error.type()) << ": " << error.message();
    
    Reason reason = Reason::SDKInternalError;
    if (wrapper_ && wrapper_->publisher_handler_) {
        wrapper_->publisher_handler_->OnPublishStateChanged(PublishState::StartPublishFailed, reason);
    }
    if (wrapper_ && wrapper_->subscriber_handler_) {
        wrapper_->subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StartSubscribeFailed, reason);
    }
}

void WebRTCWrapper::SetRemoteSessionDescriptionObserver::OnSetRemoteDescriptionComplete(
                                                                                        webrtc::RTCError error) {
    RTC_LOG(LS_INFO) << "Set remote description: " << error.message();
    
    if (error.type() == webrtc::RTCErrorType::NONE) {
        if (wrapper_ && wrapper_->publisher_handler_) {
            wrapper_->publisher_handler_->OnPublishStateChanged(PublishState::StartPublishSuccess, Reason::Success);
        }
        if (wrapper_ && wrapper_->subscriber_handler_) {
            wrapper_->subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StartSubscribeSuccess, Reason::Success);
        }
    } else {
        Reason reason = Reason::SDKInternalError;
        if (wrapper_ && wrapper_->publisher_handler_) {
            wrapper_->publisher_handler_->OnPublishStateChanged(PublishState::StartPublishFailed, reason);
        }
        if (wrapper_ && wrapper_->subscriber_handler_) {
            wrapper_->subscriber_handler_->OnSubscribeStateChanged(SubscribeState::StartSubscribeFailed, reason);
        }
    }
}

}
