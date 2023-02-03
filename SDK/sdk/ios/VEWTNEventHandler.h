/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#pragma once

#include "wtn_interface.h"

@class VEWTNEngine;
@protocol VEWTNEngineDelegate;

class VEWTNEventHandler : public vewtn::IRtcEventHandler {
public:
    VEWTNEventHandler(id<VEWTNEngineDelegate> delegate);
    ~VEWTNEventHandler();
    
    //vewtn::IRtcEventHandler
    void OnPublishStateChanged(const char* stream_id, vewtn::State state, vewtn::Reason reason) override;
    void OnLocalStats(const char* stream_id, vewtn::RTCAudioStats audio_stats, vewtn::RTCVideoStats video_stats) override;
    void OnMuteAudioSuccess(const char* stream_id) override;
    void OnMuteAudioFailed(const char* stream_id, vewtn::Reason reason) override;
    void OnMuteVideoSuccess(const char* stream_id) override;
    void OnMuteVideoFailed(const char* stream_id, vewtn::Reason reason) override;
    
        // 订阅
    void OnSubscribeStateChanged(const char* stream_id, vewtn::State state, vewtn::Reason reason) override;
        // 拉流状态
    void OnRemoteStats(const char* stream_id, vewtn::RTCAudioStats audio_stats, vewtn::RTCVideoStats video_stats) override;
        // 远端首帧渲染
    void OnFirstRemoteVideoFrameRendered(const char* stream_id) override;
        // 卡顿信息
    void OnStallInfo(const char* stream_id, vewtn::StallInfo stall_info) override;
        // h264数据回调
    void OnEncodedVideoFrame(const char* stream_id, vewtn::EncodedVideoFrame encoded_video_frame) override;
        // yuv数据回调
    void OnVideoFrame(const char* stream_id, vewtn::VideoFrame video_frame) override;
        // pcm数据回调
    void OnAudioFrame(const char* stream_id, vewtn::AudioFrame audio_frame) override;
private:
    __weak id<VEWTNEngineDelegate> _delegate;
};
