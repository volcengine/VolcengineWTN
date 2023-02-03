/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "rtc_base/message_handler.h"
#include "rtc_base/thread.h"
#include "rtc_base/thread_message.h"

namespace vewtn {

enum {
    MSG_STATE_CHANGED = 0,
    MSG_LOCAL_STATS,
    MSG_MUTE_AUDIO,
    MSG_MUTE_VIDEO,
    MSG_REMOTE_STATS,
    MSG_FIRST_REMOTE_VIDEO_FRAME,
    MSG_STALL_INFO,
    MSG_H264_VIDEO_FRAME,
    MSG_YUV_VIDEO_FRAME,
    MSG_PCM_AUDIO_FRAME,
    MSG_ICE_STAGE_CHANGED,
    MSG_ERROR,
};

struct StateChangedMsg : public rtc::MessageData {
    StateChangedMsg(std::string stream_id, State state, Reason reason)
        : stream_id(stream_id), state(state), reason(reason) {}
    std::string stream_id;
    State state;
    Reason reason;
};

struct MuteMsg : public rtc::MessageData {
    MuteMsg(std::string stream_id, Reason reason)
        : stream_id(stream_id), reason(reason) {}
    std::string stream_id;
    Reason reason;
};

struct BaseMsg : public rtc::MessageData {
    BaseMsg(std::string stream_id)
            : stream_id(stream_id) {}
    std::string stream_id;
};

struct StallInfoMsg : public rtc::MessageData {
    StallInfoMsg(std::string stream_id, StallInfo stall_info)
            : stream_id(stream_id),
              stall_info(stall_info) {}
    std::string stream_id;
    StallInfo stall_info;
};

struct StatsMsg : public rtc::MessageData {
    StatsMsg(std::string stream_id, RTCAudioStats audio_stats, RTCVideoStats video_stats)
            : stream_id(stream_id),
              audio_stats(audio_stats),
              video_stats(video_stats) {}
    std::string stream_id;
    RTCAudioStats audio_stats;
    RTCVideoStats video_stats;
};

struct IceStageChangedMsg : public rtc::MessageData {
    IceStageChangedMsg(std::string stream_id, IceStage ice_stage)
            : stream_id(stream_id),
              ice_stage(ice_stage) {}
    std::string stream_id;
    IceStage ice_stage;
};

struct ErrorMsg : public rtc::MessageData {
    ErrorMsg(std::string stream_id, ErrorCode code)
            : stream_id(stream_id),
              code(code) {}
    std::string stream_id;
    ErrorCode code;
};

class EventHandler : public rtc::MessageHandler {
public:
    explicit EventHandler(rtc::Thread* callback_thread, IRtcEventHandler* event_handler, bool is_publisher);
    ~EventHandler();

    // Implements MessageHandler.
    void OnMessage(rtc::Message* msg) override;

private:
    rtc::Thread* callback_thread_ = nullptr;
    IRtcEventHandler* event_handler_ = nullptr;
    bool is_publisher_ = true;
};

}  // namespace vewtn
