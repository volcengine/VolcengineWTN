/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#include "VEWTNEventHandler.h"
#import "VEWTNDefines.h"
#import "VEWTNEngine.h"

VEWTNEventHandler::VEWTNEventHandler(id<VEWTNEngineDelegate> delegate) : _delegate(delegate)  {
    
}

VEWTNEventHandler::~VEWTNEventHandler() {
    _delegate = nil;
}


void VEWTNEventHandler::OnMuteAudioSuccess(const char* stream_id) {
    if ([_delegate respondsToSelector:@selector(onMuteAudioSuccess:)]) {
        [_delegate onMuteAudioSuccess: [[NSString alloc] initWithUTF8String:stream_id]];
    }
}

static VEWTNReason nReason2oc(vewtn::Reason reason) {
    switch (reason) {
        case vewtn::Reason::Success:
            return VEWTNReasonSuccess;
            break;
        case vewtn::Reason::ParameterError:
            return VEWTNReasonParameterError;
            break;
        case vewtn::Reason::TokenError:
            return VEWTNReasonTokenError;
            break;
        case vewtn::Reason::ResourcesNotFound:
            return VEWTNReasonResourcesNotFound;
            break;
        case vewtn::Reason::ServerInternalError:
            return VEWTNReasonServerInternalError;
            break;
        case vewtn::Reason::SDKInternalError:
            return VEWTNReasonSDKInternalError;
            break;
        case vewtn::Reason::NetworkError:
            return VEWTNReasonParameterError;
            break;
        default:
            return VEWTNReasonUnknowError;
            break;
    }
}

void VEWTNEventHandler::OnMuteAudioFailed(const char* stream_id, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(onMuteAudioFailed:reason:)]) {
        [_delegate onMuteAudioFailed:[[NSString alloc] initWithUTF8String:stream_id] reason:nReason2oc(reason)];
    }
}

void VEWTNEventHandler::OnMuteVideoSuccess(const char* stream_id) {
    if ([_delegate respondsToSelector:@selector(onMuteVideoSuccess:)]) {
        [_delegate onMuteVideoSuccess:[[NSString alloc] initWithUTF8String:stream_id]];
    }
}

void VEWTNEventHandler::OnMuteVideoFailed(const char* stream_id, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(onMuteVideoFailed:reason:)]) {
        [_delegate onMuteVideoFailed:[[NSString alloc] initWithUTF8String:stream_id] reason:nReason2oc(reason)];
    }
}

static VEWTNState nState2oc(vewtn::State state) {
    switch (state) {
        case vewtn::State::StartSuccess:
            return VEWTNStateStartSuccess;
            break;
        case vewtn::State::StopSuccess:
            return VEWTNStateStopSuccess;
            break;
        case vewtn::State::StopFailed:
            return VEWTNStateStopFailed;
            break;
        default:
            return VEWTNStateStartFailed;
            break;
    }
}

void VEWTNEventHandler::OnPublishStateChanged(const char* stream_id, vewtn::State state, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(onPublishStateChanged:publishState:reason:)]) {
        [_delegate onPublishStateChanged:[[NSString alloc] initWithUTF8String:stream_id] publishState:nState2oc(state) reason:nReason2oc(reason)];
    }
}

    // 订阅
void VEWTNEventHandler::OnSubscribeStateChanged(const char* stream_id, vewtn::State state, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(onSubscribeStateChanged:subscribeState:reason:)]) {
        [_delegate onSubscribeStateChanged:[[NSString alloc] initWithUTF8String:stream_id] subscribeState:nState2oc(state) reason:nReason2oc(reason)];
    }
}

static VEWTNRTCAudioStats * nAudioStats2oc(vewtn::RTCAudioStats &audio_stats) {
    VEWTNRTCAudioStats * stats = [[VEWTNRTCAudioStats alloc] init];
    stats.bytes_sent = audio_stats.bytes_sent;
    stats.bytes_received = audio_stats.bytes_received;
    stats.bytes_sent_per_second = audio_stats.bytes_sent_per_second;
    stats.bytes_received_per_second = audio_stats.bytes_received_per_second;
    stats.audio_level = audio_stats.audio_level;
    stats.total_audio_energy = audio_stats.total_audio_energy;
    stats.round_trip_time_measurements = audio_stats.round_trip_time_measurements;
    stats.total_round_trip_time = audio_stats.total_round_trip_time;
    stats.packets_sent = audio_stats.packets_sent;
    stats.packets_received = audio_stats.packets_received;
    stats.packets_lost = audio_stats.packets_lost;
    stats.franction_loss = audio_stats.franction_loss;
    stats.jitter = audio_stats.jitter;
    return stats;
}

static VEWTNRTCVideoStats * nVideoStats2oc(vewtn::RTCVideoStats &video_stats) {
    VEWTNRTCVideoStats * stats = [[VEWTNRTCVideoStats alloc] init];
    stats.bytes_sent = video_stats.bytes_sent;
    stats.bytes_received = video_stats.bytes_received;
    stats.bytes_sent_per_second = video_stats.bytes_sent_per_second;
    stats.bytes_received_per_second = video_stats.bytes_received_per_second;
    stats.frame_width = video_stats.frame_width;
    stats.frame_height = video_stats.frame_height;
    stats.frames_per_second = video_stats.frames_per_second;
    stats.round_trip_time_measurements = video_stats.round_trip_time_measurements;
    stats.total_round_trip_time = video_stats.total_round_trip_time;
    stats.packets_sent = video_stats.packets_sent;
    stats.packets_received = video_stats.packets_received;
    stats.packets_lost = video_stats.packets_lost;
    stats.franction_loss = video_stats.franction_loss;
    stats.jitter = video_stats.jitter;
    return stats;
}

void VEWTNEventHandler::OnLocalStats(const char* stream_id, vewtn::RTCAudioStats audio_stats, vewtn::RTCVideoStats video_stats) {
    if ([_delegate respondsToSelector:@selector(onLocalStats:audioStats:videoStats:)]) {
        [_delegate onLocalStats:[[NSString alloc] initWithUTF8String:stream_id] audioStats:nAudioStats2oc(audio_stats) videoStats:nVideoStats2oc(video_stats)];
    }
}

    // 拉流状态
void VEWTNEventHandler::OnRemoteStats(const char* stream_id, vewtn::RTCAudioStats audio_stats, vewtn::RTCVideoStats video_stats) {
    if ([_delegate respondsToSelector:@selector(onRemoteStats:audioStats:videoStats:)]) {
        [_delegate onRemoteStats:[[NSString alloc] initWithUTF8String:stream_id] audioStats:nAudioStats2oc(audio_stats) videoStats:nVideoStats2oc(video_stats)];
    }
}

    // 远端首帧渲染
void VEWTNEventHandler::OnFirstRemoteVideoFrameRendered(const char* stream_id) {
    if ([_delegate respondsToSelector:@selector(onFirstRemoteVideoFrameRendered:)]) {
        [_delegate onFirstRemoteVideoFrameRendered:[[NSString alloc] initWithUTF8String:stream_id]];
    }
}

static VEWTNStallInfo * nStallInfo2oc(vewtn::StallInfo &stall_info) {
    VEWTNStallInfo * info = [[VEWTNStallInfo alloc] init];
    info.stall_count_200 = stall_info.stall_count_200;
    info.stall_duration_200 = stall_info.stall_duration_200;
    info.stall_count_500 = stall_info.stall_count_500;
    info.stall_duration_500 = stall_info.stall_duration_500;
    info.frame_rate = stall_info.frame_rate;
    return info;
}
    // 卡顿信息
void VEWTNEventHandler::OnStallInfo(const char* stream_id, vewtn::StallInfo stall_info) {
    if ([_delegate respondsToSelector:@selector(onStallInfo:stallInfo:)]) {
        [_delegate onStallInfo:[[NSString alloc] initWithUTF8String:stream_id] stallInfo:nStallInfo2oc(stall_info)];
    }
}

static VEWTNEncodedVideoFrame * nEncodedFrame2oc(vewtn::EncodedVideoFrame &encodedFrame) {
    VEWTNEncodedVideoFrame * frame = [[VEWTNEncodedVideoFrame alloc] init];
    frame.codec_type = (VEWTNEncodedVideoFrameType)encodedFrame.codec_type;
    frame.data = encodedFrame.data;
    frame.size = encodedFrame.size;
    frame.width = encodedFrame.width;
    frame.height = encodedFrame.height;
    frame.timestamp_ms = encodedFrame.timestamp_ms;
    frame.rotation = encodedFrame.rotation;
    frame.is_key_frame = encodedFrame.is_key_frame;
    return frame;
}
    // h264数据回调
void VEWTNEventHandler::OnEncodedVideoFrame(const char* stream_id, vewtn::EncodedVideoFrame encodedFrame) {
    if ([_delegate respondsToSelector:@selector(onEncodedVideoFrame:frame:)]) {
        [_delegate onEncodedVideoFrame:[[NSString alloc] initWithUTF8String:stream_id] frame:nEncodedFrame2oc(encodedFrame)];
    }
}

static VEWTNVideoFrame * nVideoFrame2oc(vewtn::VideoFrame &video_frame) {
    VEWTNVideoFrame * frame = [[VEWTNVideoFrame alloc] init];
    frame.type = (VEWTNVideoFrameType)video_frame.type;
    frame.y_buffer = video_frame.y_buffer;
    frame.u_buffer = video_frame.u_buffer;
    frame.v_buffer = video_frame.v_buffer;
    frame.y_stride = video_frame.y_stride;
    frame.u_stride = video_frame.u_stride;
    frame.v_stride = video_frame.v_stride;
    frame.width = video_frame.width;
    frame.height = video_frame.height;
    frame.timestamp_ms = video_frame.timestamp_ms;
    frame.rotation = video_frame.rotation;
    return frame;
}
    // yuv数据回调
void VEWTNEventHandler::OnVideoFrame(const char* stream_id, vewtn::VideoFrame video_frame) {
    if ([_delegate respondsToSelector:@selector(onVideoFrame:frame:)]) {
        [_delegate onVideoFrame:[[NSString alloc] initWithUTF8String:stream_id] frame:nVideoFrame2oc(video_frame)];
    }
}

static VEWTNAudioFrame * nAudioFrame2oc(vewtn::AudioFrame &audio_frame) {
    VEWTNAudioFrame * frame = [[VEWTNAudioFrame alloc] init];
    frame.type = (VEWTNAudioFrameType)audio_frame.type;
    frame.data = audio_frame.data;
    frame.sample_length = audio_frame.sample_length;
    frame.sample_rate = audio_frame.sample_rate;
    frame.timestamp_ms = audio_frame.timestamp_ms;
    frame.channels = audio_frame.channels;
    return frame;
}

    // pcm数据回调
void VEWTNEventHandler::OnAudioFrame(const char* stream_id, vewtn::AudioFrame audio_frame) {
    if ([_delegate respondsToSelector:@selector(onAudioFrame:frame:)]) {
        [_delegate onAudioFrame:[[NSString alloc] initWithUTF8String:stream_id] frame:nAudioFrame2oc(audio_frame)];
    }
}

