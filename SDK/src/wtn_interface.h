/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#ifndef WTN_INTERFACE_H__
#define WTN_INTERFACE_H__

#include <stdint.h>

#if defined(WEBRTC_LINUX)
#define VEWTN_API extern "C"
#else
#define VEWTN_API __attribute__((__visibility__("default"))) extern "C"
#endif

namespace vewtn {

enum PubAudioSource {
    InternalRecord = 0,           // default
    PushExternalPCM
};

enum PubVideoSource {
    InternalCamera = 0,           // default
    PushExternalYUV,
    PushExternalH264
};

enum SubAudioSource {
    InternalPlay = 0,           // default
    CallbackPCM
};

enum SubVideoSource {
    InternalRender = 0,           // default
    CallbackYUV,
    CallbackH264
};

enum State {
    StartSuccess,  // 开始发布/订阅成功
    StartFailed,   // 开始发布/订阅失败
    StopSuccess,   // 停止发布/订阅成功
    StopFailed,    // 停止发布/订阅失败
};

// 发布或订阅失败原因
enum Reason {
    Success,              // 成功
    ParameterError,       // 参数错误
    TokenError,           // token错误(包括token错误、token过期、token鉴权失败)
    ResourcesNotFound,    // 未找到资源
    ServerInternalError,  // 服务器内部错误
    SDKInternalError,     // SDK 内部错误(包括设置sdp失败)
    NetworkError,         // 网络错误
    UnknownError,         // 未知错误
    StreamNotExist,       // 流不存在
    DeviceNoPermission,   // 没有设备权限（摄像头 or 麦克风）
};

// ice 建连时的阶段性进展
enum IceStage {
    SendOffer,        // 发出 offer sdp 请求
    ReceiveAnswer,    // 收到 answer sdp 回复
    SetAnswer,        // 设置 answer
    IceConnected,     // ice 建连成功
};

// 错误码
enum ErrorCode {
    ConnectionRequestError = 10000, // 连接请求返回错误
    ConnectionLost = 10001,         // 连接中断
    CameraNoPerssion = 10002,       // 无摄像头权限
    RecordNoPerssion = 10003,       // 无麦克风权限
};

enum EncodedVideoFrameType {
    CODEC_H264,
};

enum VideoFrameType {
    YUV_I420,
};

enum AudioFrameType {
    PCM_16,
};

struct PubExtraParam {
    PubAudioSource pub_audio_source = PubAudioSource::InternalRecord;
    PubVideoSource pub_video_source = PubVideoSource::InternalCamera;
};

struct SubExtraParam {
    SubAudioSource sub_audio_source = SubAudioSource::InternalPlay;
    SubVideoSource sub_video_source = SubVideoSource::InternalRender;
};

struct EncodedVideoFrame {
    EncodedVideoFrameType codec_type = EncodedVideoFrameType::CODEC_H264;
    const uint8_t* data = nullptr; 
    int size = 0;
    int width = 0;
    int height = 0;
    int64_t timestamp_ms = 0;
    int rotation = 0;
    bool is_key_frame = false;    // true:I-frame | false:P-frame
};

struct VideoFrame {
    VideoFrameType type = VideoFrameType::YUV_I420;
    int width = 0;
    int height = 0;
    const uint8_t* y_buffer = nullptr;
    const uint8_t* u_buffer = nullptr;
    const uint8_t* v_buffer = nullptr;
    int y_stride = 0;
    int u_stride = 0;
    int v_stride = 0;
    int64_t timestamp_ms = 0;
    int rotation = 0;
};

struct AudioFrame {
    AudioFrameType type = AudioFrameType::PCM_16;
    const uint8_t* data = nullptr;
    int sample_length = 0;
    int64_t timestamp_ms = 0;
    int sample_rate = 0;
    int channels = 0;
};

struct VideoConfig {
    int width = 640;
    int height = 960;
    int max_fps = 15;
};

struct ExtendedParameters {
    bool disable_encryption = false;  // default open
    bool disable_ipv6 = false;
    int network_ignore_mask = 1 << 4;
    bool tcp_candidate_policy = 1;
    int audio_max_kbps = 0;
    int video_max_kbps = 0;
};

struct StallInfo {
    int stall_count_500 = 0;
    int stall_duration_500 = 0;
    int stall_count_200 = 0;
    int stall_duration_200 = 0;
    int frame_rate = 0;
};

struct RTCAudioStats {
    int64_t bytes_sent;
    int64_t bytes_received;
    double bytes_sent_per_second;
    double bytes_received_per_second;
    double audio_level;
    double total_audio_energy;
    const char* codec_id;
    uint64_t round_trip_time_measurements;
    double total_round_trip_time;
    uint32_t packets_sent;
    uint32_t packets_received;
    uint32_t packets_lost;  // only pub
    double franction_loss;
    double jitter;

    RTCAudioStats() {
        bytes_sent = 0;
        bytes_received = 0;
        bytes_sent_per_second = 0.0;
        bytes_received_per_second = 0.0;
        audio_level = 0.0;
        total_audio_energy = 0.0;
        round_trip_time_measurements = 0;
        total_round_trip_time = 1;
        packets_sent = 0;
        packets_received = 0;
        franction_loss = 0.0;
        packets_lost = 0;
        jitter = 0;
    }
};

struct RTCVideoStats {
    int64_t bytes_sent;
    int64_t bytes_received;
    int64_t bytes_sent_per_second;
    int64_t bytes_received_per_second;
    int64_t frame_width;
    int64_t frame_height;
    double frames_per_second;
    const char* codec_id;
    uint64_t round_trip_time_measurements;
    double total_round_trip_time;
    uint32_t packets_sent;
    uint32_t packets_received;
    uint32_t packets_lost;  // only pub
    double franction_loss;
    double jitter;
    
    RTCVideoStats() {
        bytes_sent = 0;
        bytes_received = 0;
        bytes_sent_per_second = 0.0;
        bytes_received_per_second = 0.0;
        frame_width = 0;
        frame_height = 0;
        frames_per_second = 0.0;
        round_trip_time_measurements = 0;
        total_round_trip_time = 1;
        packets_sent = 0;
        packets_received = 0;
        franction_loss = 0.0;
        packets_lost = 0;
        jitter = 0;
    }
};

class IRtcEventHandler {
public:
    virtual void OnError(const char* stream_id, ErrorCode error) {}
    virtual void OnIceStageChanged(const char* stream_id, IceStage stage) {}
    // 发布状态
    virtual void OnPublishStateChanged(const char* stream_id, State state, Reason reason) {}
    // 推流状态
    virtual void OnLocalStats(const char* stream_id, RTCAudioStats audio_stats, RTCVideoStats video_stats) {}
    // mute
    virtual void OnMuteAudioSuccess(const char* stream_id) {}
    virtual void OnMuteAudioFailed(const char* stream_id, Reason reason) {}
    virtual void OnMuteVideoSuccess(const char* stream_id) {}
    virtual void OnMuteVideoFailed(const char* stream_id, Reason reason) {}
    // 订阅
    virtual void OnSubscribeStateChanged(const char* stream_id, State state, Reason reason) {}
    // 拉流状态
    virtual void OnRemoteStats(const char* stream_id, RTCAudioStats audio_stats, RTCVideoStats video_stats) {}
    // 远端首帧渲染
    virtual void OnFirstRemoteVideoFrameRendered(const char* stream_id) {}
    // 卡顿信息
    virtual void OnStallInfo(const char* stream_id, StallInfo stall_info) {}
    // h264数据回调
    virtual void OnEncodedVideoFrame(const char* stream_id, EncodedVideoFrame encoded_video_frame) {}
    // yuv数据回调
    virtual void OnVideoFrame(const char* stream_id, VideoFrame video_frame) {}
    // pcm数据回调
    virtual void OnAudioFrame(const char* stream_id, AudioFrame audio_frame) {}
};

class IPublisher {
public:
    virtual ~IPublisher() = default;
    virtual int StartPublish(const char* url) = 0;
    virtual int StopPublish() = 0;
    virtual bool IsPublishing() = 0;
    virtual void SetLocalView(void* local_view) = 0;
    virtual int SetVideoConfig(const VideoConfig& video_config) = 0;
    virtual int StartVideoCapture() = 0;
    virtual int StopVideoCapture() = 0;
    virtual int SwitchCamera() = 0;
    virtual int StartAudioCapture() = 0;
    virtual int StopAudioCapture() = 0;
    virtual int SetLocalAudioMute(bool mute) = 0;
    virtual int SetLocalVideoMute(bool mute) = 0;
    virtual bool PushEncodedVideoFrame(const EncodedVideoFrame& encoded_video_frame) = 0;
    virtual bool PushVideoFrame(const VideoFrame& video_frame) = 0;
    virtual bool PushAudioFrame(const AudioFrame& audio_frame) = 0;
};

class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual int StartSubscribe(const char* url) = 0;
    virtual int StopSubscribe() = 0;
    virtual bool IsSubscribed() = 0;

    virtual int SetEnableSpeaker(bool enable) = 0;
    virtual void SetRemoteView(void* remote_sink) = 0;
    virtual int SetRemoteAudioMute(bool mute) = 0;
    virtual int SetRemoteVideoMute(bool mute) = 0;
};

class IRtcEngine {
public:
    virtual ~IRtcEngine() = default;

    virtual IPublisher* CreatePublisher(const PubExtraParam& pub_extra_param) = 0;
    virtual void DestroyPublisher(const IPublisher* publisher_ptr) = 0;

    virtual ISubscriber* CreateSubscriber(const SubExtraParam& sub_extra_param) = 0;
    virtual void DestroySubscriber(const ISubscriber* subscriber_ptr) = 0;
};

VEWTN_API void SetExtendedParameters(ExtendedParameters param);
VEWTN_API IRtcEngine* CreateRtcEngine(IRtcEventHandler* event_handler);
VEWTN_API void DestroyRtcEngine(const IRtcEngine* engine_ptr);

}    // end of namespace

#endif
