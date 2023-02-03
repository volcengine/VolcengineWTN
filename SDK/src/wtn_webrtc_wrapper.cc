/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "wtn_webrtc_wrapper.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "api/rtc_event_log/rtc_event_log_factory.h"
#include "media/engine/webrtc_media_engine.h"
#include "external_module/fake_decoder.h"
#include "rtc_base/task_utils/repeating_task.h"

#if defined(WEBRTC_ANDROID)
#include "platform/android_platform_helper.h"
#elif defined(WEBRTC_IOS)
#include "sdk/objc/native/api/audio_device_module.h"
#include "platform/objc_platform_helper.h"
#endif

namespace vewtn {

extern ExtendedParameters global_extend_param;

class WebRtcWrapper::SetRemoteSessionDescriptionObserver
        : public webrtc::SetRemoteDescriptionObserverInterface {
public:
    SetRemoteSessionDescriptionObserver(WebRtcWrapper* wrapper)
            : wrapper_(wrapper) {}
    void OnSetRemoteDescriptionComplete(webrtc::RTCError error) override;
    
private:
    WebRtcWrapper* wrapper_;
};

class WebRtcWrapper::SetLocalSessionDescriptionObserver
        : public webrtc::SetSessionDescriptionObserver {
public:
    SetLocalSessionDescriptionObserver(WebRtcWrapper* wrapper)
            : wrapper_(wrapper) {}
    void OnSuccess() override;
    void OnFailure(webrtc::RTCError error) override;
private:
    WebRtcWrapper* wrapper_;
};

WebRtcWrapper::WebRtcWrapper(IRtcEventHandler* event_handler, bool is_pub, PubExtraParam pub_extra_param, SubExtraParam sub_extra_param)
    : event_handler_(event_handler),
      is_publisher_(is_pub),
      pub_extra_param_(pub_extra_param),
      sub_extra_param_(sub_extra_param) {
    thread_checker_.Detach();
    rtc::LogMessage::LogToDebug(rtc::LS_INFO);  // LS_INFO, LS_VERBOSE
    rtc::LogMessage::LogTimestamps(true);

    call_started_ = false;
    whip_client_ = std::make_unique<WhipClient>(this);

    callback_thread_ = rtc::Thread::Create();
    callback_thread_->SetName("wtn_callback_thread", nullptr);
    callback_thread_->Start();
    callback_ = std::make_unique<EventHandler>(callback_thread_.get(), event_handler, is_pub);

    CreatePeerConnectionFactory();
}

WebRtcWrapper::~WebRtcWrapper() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::~WebRtcWrapper begin";
    //webrtc::MutexLock lock(&mutex_);
    if (whip_client_) {
        whip_client_.reset();
    }
    if (callback_thread_) {
        rtc::MessageList list;
        callback_thread_->Clear(callback_.get(), rtc::MQID_ANY, &list);
        for (auto& msg : list) {
            delete msg.pdata;
        }

        callback_thread_->Stop();
        callback_thread_.reset(nullptr);
    }
    callback_.reset(nullptr);
    if (is_publisher_ && pub_extra_param_.pub_video_source == PubVideoSource::InternalCamera) {
        StopVideoCapture();
    }
    if (is_publisher_ && pub_extra_param_.pub_audio_source == PubAudioSource::InternalRecord) {
        StopAudioCapture();
    }
    
    if (pc_) {
        pc_->Close();
        pc_ = nullptr;
    }

    worker_thread_->Invoke<void>(RTC_FROM_HERE, [this]() {
        internal_adm_ = nullptr;
    });

    internal_video_source_ = nullptr;
    external_video_source_ = nullptr;
    event_handler_ = nullptr;
    task_queue_ = nullptr;
    stats_callback_ = nullptr;

    pcf_ = nullptr;  // 销毁pcf
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::~WebRtcWrapper end";
}

void WebRtcWrapper::CreatePeerConnectionFactory() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::CreatePeerConnectionFactory";
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
    task_queue_factory_ = pcf_deps.task_queue_factory.get();

    cricket::MediaEngineDependencies media_deps;
    if (is_publisher_ &&
        pub_extra_param_.pub_audio_source == PubAudioSource::PushExternalPCM) {
        external_adm_ = rtc::scoped_refptr<ExternalAudioDeviceModule>(
                ExternalAudioDeviceModule::Create(nullptr, stream_id_));
        media_deps.adm = external_adm_;
    } else if (!is_publisher_ &&
               sub_extra_param_.sub_audio_source == SubAudioSource::CallbackPCM) {
        external_adm_ = rtc::scoped_refptr<ExternalAudioDeviceModule>(
                ExternalAudioDeviceModule::Create(event_handler_, stream_id_));
        media_deps.adm = external_adm_;
    } else {
#if defined(WEBRTC_IOS)
        internal_adm_ = webrtc::CreateAudioDeviceModule();
#elif defined(WEBRTC_ANDROID)
        internal_adm_ = worker_thread_->Invoke < rtc::scoped_refptr <
                        webrtc::AudioDeviceModule >> (RTC_FROM_HERE, [&]() {
                            return webrtc::AudioDeviceModule::Create(
                                    webrtc::AudioDeviceModule::kPlatformDefaultAudio,
                                    pcf_deps.task_queue_factory.get());
                        });
#endif
        media_deps.adm = internal_adm_;
    }

    media_deps.task_queue_factory = pcf_deps.task_queue_factory.get();

    if (media_deps.audio_encoder_factory == nullptr) {
        media_deps.audio_encoder_factory = webrtc::CreateBuiltinAudioEncoderFactory();
    }
    if (media_deps.audio_decoder_factory == nullptr) {
        media_deps.audio_decoder_factory = webrtc::CreateBuiltinAudioDecoderFactory();
    }
    if (media_deps.audio_processing == nullptr) {
        media_deps.audio_processing = webrtc::AudioProcessingBuilder().Create();
    }

    if (is_publisher_ &&
        pub_extra_param_.pub_video_source == PubVideoSource::PushExternalH264) {
        std::unique_ptr <webrtc::VideoEncoderFactory> encoder_factory = std::make_unique<FunctionVideoEncoderFactory>(
                [&]() {
                    auto encode = std::make_unique<FakeEncoder>(webrtc::Clock::GetRealTimeClock());
                    fake_encoder_ = encode.get();
                    return encode;
                });
        media_deps.video_encoder_factory = std::move(encoder_factory);
    } else {
#if defined(WEBRTC_ANDROID)
        media_deps.video_encoder_factory = CreateAndroidEncoderFactory();
#elif defined(WEBRTC_IOS)
        media_deps.video_encoder_factory = CreateObjCEncoderFactory();
#else
        media_deps.video_encoder_factory = webrtc::CreateBuiltinVideoEncoderFactory();
#endif
    }

    if (!is_publisher_ && sub_extra_param_.sub_video_source == SubVideoSource::CallbackH264) {
        std::unique_ptr <webrtc::VideoDecoderFactory> decoder_factory = std::make_unique<FunctionVideoDecoderFactory>(
                [&]() {
                    auto decoder = std::make_unique<FakeDecoder>(event_handler_, stream_id_);
                    // fake_decoder_ = decode.get();
                    return decoder;
                });
        media_deps.video_decoder_factory = std::move(decoder_factory);
    } else {
#if defined(WEBRTC_ANDROID)
        media_deps.video_decoder_factory = CreateAndroidDecoderFactory();
#elif defined(WEBRTC_IOS)
        media_deps.video_decoder_factory = CreateObjCDecoderFactory();
#else
        media_deps.video_decoder_factory = webrtc::CreateBuiltinVideoDecoderFactory();
#endif
    }

    pcf_deps.media_engine = cricket::CreateMediaEngine(std::move(media_deps));
    RTC_LOG(LS_INFO) << "Media engine created: " << pcf_deps.media_engine.get();

    pcf_ = CreateModularPeerConnectionFactory(std::move(pcf_deps));
    RTC_LOG(LS_INFO) << "PeerConnectionFactory created: " << pcf_.get();
}

void WebRtcWrapper::Start(const std::string& url) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::Start, url: " << url;

#if defined(WEBRTC_ANDROID)
    if (!CheckPermission()) {
        OnStateChanged(State::StartFailed, Reason::DeviceNoPermission);
        return;
    }
#endif

    if (call_started_) {
        RTC_LOG(LS_WARNING) << "Call already started.";
        return;
    }
    call_started_ = true;
    start_url_ = url;
    ParserStreamId();
    PreMuteProcess();
    CreatePeerConnection();
    Connect();
}

std::vector<std::string> WebRtcWrapper::Split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    
    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    
    res.push_back (s.substr (pos_start));
    return res;
}

void WebRtcWrapper::ParserStreamId() {
    std::vector<std::string> split_url = Split(start_url_, "?");
    if (split_url.size() == 2) {
        std::string param = split_url[0];
        std::vector<std::string> split_param = Split(param, "/");
        size_t size = split_param.size();
        if (size > 0) {
            stream_id_ = split_param[size - 1];
        }
    }

}

void WebRtcWrapper::Stop() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::Stop";

    if (!call_started_) {
        RTC_LOG(LS_WARNING) << "Not yet started.";
        return;
    }

    call_started_ = false;

    //webrtc::MutexLock lock(&mutex_);
    if (whip_client_) {
        whip_client_->Stop();
    }
    if (pc_) {
        pc_->Close();
        pc_ = nullptr;
    }
    task_queue_ = nullptr;
    stats_callback_ = nullptr;
}

void WebRtcWrapper::PreMuteProcess() {
    // default use url params
    size_t start_pos = start_url_.find("MuteAudio");
    if (start_pos == std::string::npos) {
        start_url_ += audio_mute_params_;
    }
    start_pos = start_url_.find("MuteVideo");
    if (start_pos == std::string::npos) {
        start_url_ += video_mute_params_;
    }
}

void WebRtcWrapper::CreatePeerConnection() {
    //webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::CreatePeerConnection";
    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
    // default value can find at: src/media/base/media_config.h
    config.set_cpu_adaptation(false);
    config.disable_ipv6 = global_extend_param.disable_ipv6;
    RTC_LOG(LS_INFO) << "disable_ipv6: " << global_extend_param.disable_ipv6;
    config.tcp_candidate_policy = global_extend_param.tcp_candidate_policy ? 
        webrtc::PeerConnectionInterface::TcpCandidatePolicy::kTcpCandidatePolicyEnabled : webrtc::PeerConnectionInterface::TcpCandidatePolicy::kTcpCandidatePolicyDisabled;
    RTC_LOG(LS_INFO) << "tcp_candidate_policy: " << global_extend_param.tcp_candidate_policy;

    webrtc::PeerConnectionFactoryInterface::Options options;
    options.disable_encryption = global_extend_param.disable_encryption;
    RTC_LOG(LS_INFO) << "disable_encryption: " << global_extend_param.disable_encryption;
    options.network_ignore_mask = global_extend_param.network_ignore_mask;
    RTC_LOG(LS_INFO) << "network_ignore_mask: " << global_extend_param.network_ignore_mask;
    pcf_->SetOptions(options);
    webrtc::PeerConnectionDependencies deps(this);
    pc_ = pcf_->CreatePeerConnectionOrError(config, std::move(deps)).MoveValue();
    RTC_LOG(LS_INFO) << "PeerConnection created: " << pc_.get();

    webrtc::RtpTransceiverInit trans_init;
    if (is_publisher_) {
        trans_init.direction = webrtc::RtpTransceiverDirection::kSendOnly;
    } else {
        trans_init.direction = webrtc::RtpTransceiverDirection::kRecvOnly;
    }

    if (is_publisher_ && (pub_extra_param_.pub_video_source == PubVideoSource::PushExternalH264
                          || pub_extra_param_.pub_video_source == PubVideoSource::PushExternalYUV)) {
        external_video_source_ = rtc::make_ref_counted<ExternalVideoTrackSource>(false);
        SetVideoSource(external_video_source_);
    } else {
        SetupInternalVideoTrack();
    }

    if (local_video_track_) {
        webrtc::RtpTransceiverInit video_trans_init = trans_init;
        webrtc::RtpEncodingParameters params;
        if (global_extend_param.video_max_kbps != 0) {
            params.max_bitrate_bps = global_extend_param.video_max_kbps;
        }
        video_trans_init.send_encodings.push_back(params);
        auto trans = pc_->AddTransceiver(local_video_track_, video_trans_init);
        if (is_publisher_) {
            // disable degradation
            rtc::scoped_refptr<webrtc::RtpSenderInterface> sender = trans.value()->sender();
            webrtc::RtpParameters rtp_param;
            rtp_param.degradation_preference = webrtc::DegradationPreference::DISABLED;
            sender->SetParameters(rtp_param);
        }
    }

    rtc::scoped_refptr<webrtc::AudioSourceInterface> audio_source = pcf_->CreateAudioSource(cricket::AudioOptions());
    local_audio_track_ = pcf_->CreateAudioTrack("audio", audio_source.get());
    webrtc::RtpTransceiverInit audio_trans_init = trans_init;
    webrtc::RtpEncodingParameters params;
    if (global_extend_param.audio_max_kbps != 0) {
        params.max_bitrate_bps = global_extend_param.audio_max_kbps;;
    }
    audio_trans_init.send_encodings.push_back(params);
    pc_->AddTransceiver(local_audio_track_, audio_trans_init);
    RTC_LOG(LS_INFO) << "Local audio sink set up: " << local_video_track_.get();

    if (!is_publisher_ || (is_publisher_ && !is_start_recording_ &&
                           pub_extra_param_.pub_audio_source == PubAudioSource::InternalRecord)) {
        pc_->SetAudioRecording(false);
    }

#if defined(WEBRTC_LINUX)
    if (is_publisher_ && pub_extra_param_.pub_audio_source == PubAudioSource::PushExternalPCM && external_adm_) {
        external_adm_->InitRecording();
        external_adm_->StartRecording();
    }
#endif
    if (sub_extra_param_.sub_video_source == SubVideoSource::CallbackYUV) {
        SetRemoteView(nullptr);
    }
}

void WebRtcWrapper::Connect() {
    RTC_LOG(LS_INFO) << "CreateOffer";
    //webrtc::MutexLock lock(&mutex_);
    pc_->CreateOffer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
}

void WebRtcWrapper::SetLocalView(void* local_view) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetLocalView";
 #if defined(WEBRTC_IOS)
    ObjcLocalViewToNativeVideoSink(local_view);
#elif defined(WEBRTC_ANDROID)
    local_sink_ = std::move(JavaToNativeVideoSink(local_view));
#endif
}

void WebRtcWrapper::SetRemoteView(void* remote_view) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetRemoteView";
#if defined(WEBRTC_IOS)
    remote_sink_ = std::make_unique<VideoSinkProxy>(
            std::move(ObjcToNativeVideoSink(remote_view)), this,
            sub_extra_param_.sub_video_source == SubVideoSource::CallbackYUV
            ? event_handler_ : nullptr);
#elif defined(WEBRTC_ANDROID)
    remote_sink_ = std::make_unique<VideoSinkProxy>(
            remote_view ? std::move(JavaToNativeVideoSink(remote_view)) : nullptr, this,
            sub_extra_param_.sub_video_source == SubVideoSource::CallbackYUV
            ? event_handler_ : nullptr);
#else
    remote_sink_ = std::make_unique<VideoSinkProxy>(nullptr, this,
            sub_extra_param_.sub_video_source == SubVideoSource::CallbackYUV
            ? event_handler_ : nullptr);
#endif
}

void WebRtcWrapper::SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) {
    if (video_source) {
        local_video_track_ = pcf_->CreateVideoTrack("video", video_source.get());
        if (local_sink_) {
            local_video_track_->AddOrUpdateSink(local_sink_.get(), rtc::VideoSinkWants());
        }
    }
}

int WebRtcWrapper::SetAudioMute(bool mute) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetAudioMute, mute: " << mute;
    if (mute) {
        audio_mute_params_ = "&MuteAudio=true";
    } else {
        audio_mute_params_ = "&MuteAudio=false";
    }

    // call before pub/sub
    if (!call_started_) {
        return 0;
    }
    if (local_audio_track_->enabled() && mute) {
        local_audio_track_->set_enabled(false);
    } else {
        local_audio_track_->set_enabled(true);
    }
    //webrtc::MutexLock lock(&mutex_);
    return whip_client_->SetAudioMute(mute);
}

int WebRtcWrapper::SetVideoMute(bool mute) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetVideoMute, mute: " << mute;
    if (mute) {
        video_mute_params_ = "&MuteVideo=true";
    } else {
        video_mute_params_ = "&MuteVideo=false";
    }

    // call before pub/sub
    if (!call_started_) {
        return 0;
    }
    if (local_video_track_->enabled() && mute) {
        local_video_track_->set_enabled(false);
    } else {
        local_video_track_->set_enabled(true);
    }
    //webrtc::MutexLock lock(&mutex_);
    return whip_client_->SetVideoMute(mute);
}

int WebRtcWrapper::SetVideoConfig(VideoConfig video_config) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetVideoConfig"
                     << ", width: " << video_config.width
                     << ", height: " << video_config.height
                     << ", max_fpas: " << video_config.max_fps;
    video_config_ = video_config;
    return 0;
}

void WebRtcWrapper::SetupInternalVideoTrack() {
    if (!internal_video_source_) {
#if defined(WEBRTC_IOS)
        internal_video_source_ = CreateObjCVideoTrackSource(is_publisher_, signaling_thread_.get(), worker_thread_.get());
#elif defined(WEBRTC_ANDROID)
        internal_video_source_ = CreateAndroidVideoTrackSource(signaling_thread_.get());
#else
        internal_video_source_ = rtc::make_ref_counted<ExternalVideoTrackSource>(false);
#endif
       SetVideoSource(internal_video_source_);
    }
}

int WebRtcWrapper::StartVideoCapture() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::StartVideoCapture";
    if (pub_extra_param_.pub_video_source != PubVideoSource::InternalCamera) {
        return -1;
    }
    SetupInternalVideoTrack();
#if defined(WEBRTC_IOS)
    StartObjCVideoCapturer(if_front_,
                           video_config_.width, video_config_.height, video_config_.max_fps);
#elif defined(WEBRTC_ANDROID)
    if (!CheckCameraPermission()) {
        OnError(ErrorCode::CameraNoPerssion);
        return -2;
    }
    StartAndroidVideoCapturer(internal_video_source_, if_front_,
                              video_config_.width, video_config_.height, video_config_.max_fps);
#endif
    return 0;
}

int WebRtcWrapper::StopVideoCapture() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::StopVideoCapture";
    if (pub_extra_param_.pub_video_source != PubVideoSource::InternalCamera) {
        return -1;
    }
#if defined(WEBRTC_IOS)
    StopObjCVideoCapturer();
#elif defined(WEBRTC_ANDROID)
    StopAndroidVideoCaptuer();
#endif
    return 0;
}

int WebRtcWrapper::SwitchCamera() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SwitchCamera";
    if (pub_extra_param_.pub_video_source != PubVideoSource::InternalCamera) {
        return -1;
    }
    if_front_ = !if_front_;
#if defined(WEBRTC_IOS)
    StartObjCVideoCapturer(if_front_,
                           video_config_.width, video_config_.height, video_config_.max_fps);
#elif defined(WEBRTC_ANDROID)
    SwitchAndroidVideoCaptuer();
#endif
    return 0;
}

int WebRtcWrapper::StartAudioCapture() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::StartAudioCapture";
    if (pub_extra_param_.pub_audio_source != PubAudioSource::InternalRecord) {
        return -1;
    }
#if defined(WEBRTC_ANDROID)
    if (!CheckRecordPermission()) {
        OnError(ErrorCode::RecordNoPerssion);
        return -2;
    }
#endif
    worker_thread_->PostTask([&] {
        is_start_recording_ = true;
        if (!internal_adm_->Recording()) {
            internal_adm_->InitRecording();
            internal_adm_->StartRecording();
        }
    });
    return 0;
}

int WebRtcWrapper::StopAudioCapture() {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::StopAudioCapture";
    if (pub_extra_param_.pub_audio_source != PubAudioSource::InternalRecord) {
        return -1;
    }
#if defined(WEBRTC_ANDROID)
    if (!CheckRecordPermission()) {
        OnError(ErrorCode::RecordNoPerssion);
        return -2;
    }
#endif
    worker_thread_->PostTask([&] {
        is_start_recording_ = false;
        if (internal_adm_->Recording()) {
            internal_adm_->StopRecording();
        }
    });
    return 0;
}

int WebRtcWrapper::SetEnableSpeaker(bool enable) {
    RTC_LOG(LS_INFO) << "[WTN_API]WebRtcWrapper::SetEnableSpeaker";
    if (sub_extra_param_.sub_audio_source != SubAudioSource::InternalPlay) {
        return -1;
    }
#if defined(WEBRTC_IOS)
    SetObjCEnableSpeaker(enable);
#elif defined(WEBRTC_ANDROID)
    SetAndroidEnableSpeaker(enable);
#endif
    return 0;
}

bool WebRtcWrapper::PushEncodedVideoFrame(EncodedVideoFrame encoded_frame) {
    if (pub_extra_param_.pub_video_source != PubVideoSource::PushExternalH264) {
        return false;
    }
    if (pc_ && fake_encoder_) {
        return fake_encoder_->PushEncodedVideoFrame(encoded_frame);
    } else {
        if (external_video_source_) {
            external_video_source_->FakeYuv(encoded_frame.width, encoded_frame.height,
                                            encoded_frame.timestamp_ms);
        }
    }
    return false;
}

bool WebRtcWrapper::PushVideoFrame(VideoFrame video_frame) {
    if (pub_extra_param_.pub_video_source != PubVideoSource::PushExternalYUV) {
        return false;
    }
    if (!pc_ || !external_video_source_) {
        return false;
    }
    return external_video_source_->PushVideoFrame(video_frame);
}

bool WebRtcWrapper::PushAudioFrame(AudioFrame audio_frame) {
    if (pub_extra_param_.pub_audio_source != PubAudioSource::PushExternalPCM) {
        return false;
    }
    if (!pc_ || !external_adm_) {
        return false;
    }
    return external_adm_->PushAudioFrame(audio_frame);
}

void WebRtcWrapper::OnIceStageChanged(IceStage stage) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnIceStageChanged: " << stage;
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_ICE_STAGE_CHANGED,
                        new IceStageChangedMsg(stream_id_, stage));
    }
}

void WebRtcWrapper::OnError(ErrorCode error) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnError: " << error;
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_ERROR, new ErrorMsg(stream_id_, error));
    }
}

void WebRtcWrapper::OnSignalingChange(
        webrtc::PeerConnectionInterface::SignalingState new_state) {
    RTC_LOG(LS_INFO) << "OnSignalingChange: " << new_state;
}

void WebRtcWrapper::OnDataChannel(
        rtc::scoped_refptr <webrtc::DataChannelInterface> data_channel) {
    RTC_LOG(LS_INFO) << "OnDataChannel";
}

void WebRtcWrapper::OnRenegotiationNeeded() {
    RTC_LOG(LS_INFO) << "OnRenegotiationNeeded";
}

void WebRtcWrapper::OnIceConnectionChange(
        webrtc::PeerConnectionInterface::IceConnectionState new_state) {
    RTC_LOG(LS_INFO) << "OnIceConnectionChange: " << new_state << " : " << webrtc::PeerConnectionInterface::AsString(new_state);
    if (global_extend_param.disable_encryption) {
        if (new_state == webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionFailed) {
            OnError(ErrorCode::ConnectionLost);
        } else if (new_state == webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionConnected) {
            OnIceStageChanged(IceStage::IceConnected);
            OnStateChanged(State::StartSuccess, Reason::Success);
        }
    }
    if (new_state == webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionChecking) {
        OnIceStageChanged(IceStage::SetAnswer);
    }
}

void WebRtcWrapper::OnIceGatheringChange(
        webrtc::PeerConnectionInterface::IceGatheringState new_state) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnIceGatheringChange: " << new_state << " : " << webrtc::PeerConnectionInterface::AsString(new_state);
}

void WebRtcWrapper::OnIceCandidate(
        const webrtc::IceCandidateInterface *candidate) {
    RTC_LOG(LS_INFO) << "OnIceCandidate: " << candidate->server_url();
}

void WebRtcWrapper::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnConnectionChange: " << new_state << " : " << webrtc::PeerConnectionInterface::AsString(new_state);
    if (!global_extend_param.disable_encryption) {
        if (new_state == webrtc::PeerConnectionInterface::PeerConnectionState::kFailed) {
            OnError(ErrorCode::ConnectionLost);
        } else if (new_state == webrtc::PeerConnectionInterface::PeerConnectionState::kConnected) {
            OnIceStageChanged(IceStage::IceConnected);
            OnStateChanged(State::StartSuccess, Reason::Success);
        }
    }
}

void WebRtcWrapper::OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnTrack";
    //webrtc::MutexLock lock(&mutex_);
    if (remote_sink_) {
        rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track = transceiver->receiver()->track();
        if (track && track->kind() == webrtc::MediaStreamTrackInterface::kVideoKind) {
            static_cast<webrtc::VideoTrackInterface*>(track.get())->AddOrUpdateSink(remote_sink_.get(),
                                                                                    rtc::VideoSinkWants());
            RTC_LOG(LS_INFO) << "Remote video sink set up: " << track.get();
        }
    }
}

void WebRtcWrapper::OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnRemoveTrack";
}

void WebRtcWrapper::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
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

    //webrtc::MutexLock lock(&mutex_);
    // Ownership of desc was transferred to us, now we transfer it forward.
    pc_->SetLocalDescription(
                             rtc::make_ref_counted<SetLocalSessionDescriptionObserver>(this).get(), desc);
}

void WebRtcWrapper::OnFailure(webrtc::RTCError error) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnFailure, create offer: " << ToString(error.type())
                     << ": " << error.message();
    OnStateChanged(State::StartFailed, Reason::SDKInternalError);
}

void WebRtcWrapper::OnStartSuccess(std::string sdp) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnStartSuccess: " << sdp;
    //webrtc::MutexLock lock(&mutex_);
    if (pc_) {
        OnIceStageChanged(IceStage::ReceiveAnswer);
        std::unique_ptr<webrtc::SessionDescriptionInterface> answer(webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer, sdp));
        pc_->SetRemoteDescription(std::move(answer),
                                  rtc::make_ref_counted<SetRemoteSessionDescriptionObserver>(this));
        if (is_publisher_) {
            StartStats();
        }
    }
}

void WebRtcWrapper::OnStartFailed(Reason reason) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnStartFailed, reason:" << (int)reason;
    OnStateChanged(State::StartFailed, reason);
}

void WebRtcWrapper::OnStopSuccess() {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnStopSuccess";
    OnStateChanged(State::StopSuccess, Reason::Success);
}

void WebRtcWrapper::OnStopFailed(Reason reason) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnStopFailed, reason:" << (int)reason;
    OnStateChanged(State::StartFailed, reason);
}

void WebRtcWrapper::OnFirstVideoFrameRendered() {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnFirstVideoFrameRendered";
    //webrtc::MutexLock lock(&mutex_);
    remote_sink_->UpdateStreamId(stream_id_);
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_FIRST_REMOTE_VIDEO_FRAME,
                               new BaseMsg(stream_id_));
    }

    StartStats();
}

void WebRtcWrapper::OnStateChanged(State state, Reason reason) {
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnStateChanged, state: " << state << ", reason: " << reason;
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_STATE_CHANGED,
                        new StateChangedMsg(stream_id_, state, reason));
    }
}

void WebRtcWrapper::StartStats() {
    task_queue_ = std::make_unique<rtc::TaskQueue>(task_queue_factory_->CreateTaskQueue(
            "stream_stats_thread", webrtc::TaskQueueFactory::Priority::NORMAL));
    stats_callback_ = rtc::make_ref_counted<RTCStatsCallback>(callback_thread_.get(), callback_.get(), is_publisher_, stream_id_);
    webrtc::TimeDelta interval = webrtc::TimeDelta::Millis(1000);

   webrtc::RepeatingTaskHandle::Start(task_queue_->Get(), [this, interval] {
       static bool is_first = true;
       if (is_first) {
           is_first = false;
           return interval;
       }

       {
           //webrtc::MutexLock lock(&mutex_);
           if (pc_ && stats_callback_) {
               pc_->GetStats(stats_callback_.get());
           }
       }
       if (remote_sink_.get()) {
           remote_sink_->CheckRenderVideoFrame();
           auto stall_data = remote_sink_->GetStallStatisticsData();
           RTC_LOG(LS_INFO) << "[WTNSDK]OnStallInfo: video_stall_count500: " << stall_data->video_stall_count_
                           << ", video_stall_duration500: " << stall_data->video_stall_duration_
                           << ", video_stall_count200: " << stall_data->video_stall_count200_
                           << ", video_stall_duration200: " << stall_data->video_stall_duration200_
                           << ", render_frame_rate: " << stall_data->render_frame_rate_;
           if (callback_thread_) {
               StallInfo stall_info;
               stall_info.stall_count_500 = stall_data->video_stall_count_;
               stall_info.stall_duration_500 = stall_data->video_stall_duration_;
               stall_info.stall_count_200 = stall_data->video_stall_count200_;
               stall_info.stall_duration_200 = stall_data->video_stall_duration200_;
               stall_info.frame_rate = stall_data->render_frame_rate_;
               auto stall_info_msg = new StallInfoMsg(stream_id_, stall_info);
               callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_STALL_INFO,
                                      stall_info_msg);
               RTC_LOG(LS_INFO) << "[WTNSDK]OnStallInfo post";
           }
           remote_sink_->ResetStallStatisticsData();
       }
       return interval;
   });
}

void WebRtcWrapper::OnMuteAudioSuccess() {
    //webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnMuteAudioSuccess";
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_MUTE_AUDIO,
                           new MuteMsg(stream_id_, Reason::Success));
    }
}

void WebRtcWrapper::OnMuteAudioFailed(Reason reason) {
    //webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnMuteAudioFailed";
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_MUTE_AUDIO,
                           new MuteMsg(stream_id_, reason));
    }
}

void WebRtcWrapper::OnMuteVideoSuccess() {
    //webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnMuteVideoSuccess";
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_MUTE_VIDEO,
                           new MuteMsg(stream_id_, Reason::Success));
    }
}

void WebRtcWrapper::OnMuteVideoFailed(Reason reason) {
    //webrtc::MutexLock lock(&mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::OnMuteVideoFailed";
    if (callback_thread_) {
        callback_thread_->Post(RTC_FROM_HERE, callback_.get(), MSG_MUTE_VIDEO,
                           new MuteMsg(stream_id_, reason));
    }
}

void WebRtcWrapper::SetLocalSessionDescriptionObserver::OnSuccess() {
    //webrtc::MutexLock lock(&wrapper_->mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::SetLocalSessionDescriptionObserver::OnSuccess";
    if (wrapper_ && wrapper_->whip_client_) {
        wrapper_->OnIceStageChanged(IceStage::SendOffer);
        wrapper_->whip_client_->Start(wrapper_->start_url_, wrapper_->sdp_);
    }
}

void WebRtcWrapper::SetLocalSessionDescriptionObserver::OnFailure(webrtc::RTCError error) {
    //webrtc::MutexLock lock(&wrapper_->mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::SetLocalSessionDescriptionObserver::OnFailure"
                     << ToString(error.type()) << ": " << error.message();
    if (wrapper_) {
        wrapper_->OnStateChanged(State::StartFailed, Reason::SDKInternalError);
    }
}

void WebRtcWrapper::SetRemoteSessionDescriptionObserver::OnSetRemoteDescriptionComplete(webrtc::RTCError error) {
    //webrtc::MutexLock lock(&wrapper_->mutex_);
    RTC_LOG(LS_INFO) << "[WTN_CB]WebRtcWrapper::SetRemoteSessionDescriptionObserver::OnSetRemoteDescriptionComplete: "
                     << error.message();
    if (error.type() != webrtc::RTCErrorType::NONE && wrapper_) {
        wrapper_->OnStateChanged(State::StartFailed, Reason::SDKInternalError);
    }
}

}  // namespace vewtn
