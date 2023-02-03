
/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "wtn_event_handler.h"
#include "rtc_base/logging.h"

namespace vewtn {

EventHandler::EventHandler(rtc::Thread* callback_thread, IRtcEventHandler* event_handler, bool is_publisher)
    : callback_thread_(callback_thread),
      event_handler_(event_handler),
      is_publisher_(is_publisher) {

}

EventHandler::~EventHandler() {

}

void EventHandler::OnMessage(rtc::Message* msg) {
    RTC_DCHECK_RUN_ON(callback_thread_);
    if (!event_handler_) {
        return;
    }
    // common
    // RTC_LOG(LS_INFO) << "[WTN_CB]OnMessage, msg->message_id: " << msg->message_id;
    switch (msg->message_id) {
        case MSG_MUTE_AUDIO: {
            MuteMsg* param = static_cast<MuteMsg*>(msg->pdata);
            if (param->reason == Reason::Success) {
                RTC_LOG(LS_INFO) << "[WTN_CB]OnMuteAudioSuccess, stream_id: " << param->stream_id;
                event_handler_->OnMuteAudioSuccess(param->stream_id.c_str());
            } else {
                RTC_LOG(LS_INFO) << "[WTN_CB]OnMuteAudioFailed, stream_id: " << param->stream_id << ", reason: " << param->reason;
                event_handler_->OnMuteAudioFailed(param->stream_id.c_str(), param->reason);
            }
            delete param;
            break;
        }
        case MSG_MUTE_VIDEO: {
            MuteMsg* param = static_cast<MuteMsg*>(msg->pdata);
            if (param->reason == Reason::Success) {
                RTC_LOG(LS_INFO) << "[WTN_CB]OnMuteVideoSuccess, stream_id: " << param->stream_id;
                event_handler_->OnMuteVideoSuccess(param->stream_id.c_str());
            } else {
                RTC_LOG(LS_INFO) << "[WTN_CB]OnMuteVideoFailed, stream_id: " << param->stream_id << ", reason: " << param->reason;
                event_handler_->OnMuteVideoFailed(param->stream_id.c_str(), param->reason);
            }
            delete param;
            break;
        }
        case MSG_ICE_STAGE_CHANGED: {
            IceStageChangedMsg* param = static_cast<IceStageChangedMsg*>(msg->pdata);
            RTC_LOG(LS_INFO) << "[WTN_CB]OnIceStageChanged, ice_stage: " << param->ice_stage;
            event_handler_->OnIceStageChanged(param->stream_id.c_str(), param->ice_stage);
            delete param;
            break;
        }
        case MSG_ERROR: {
            ErrorMsg* param = static_cast<ErrorMsg*>(msg->pdata);
            RTC_LOG(LS_INFO) << "[WTN_CB]OnError, stream_id: " << param->stream_id << ", code: " << param->code;
            event_handler_->OnError(param->stream_id.c_str(), param->code);
            delete param;
            break;
        }
        default: {
            break;
        }
    }

    if (is_publisher_) {
        switch (msg->message_id) {
            case MSG_STATE_CHANGED: {
                StateChangedMsg* param = static_cast<StateChangedMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnPublishStateChanged, stream_id: " << param->stream_id
                                 << ", state: " << param->state
                                 << ", reason: " << param->reason;
                event_handler_->OnPublishStateChanged(param->stream_id.c_str(), param->state, param->reason);
                delete param;
                break;
            }
            case MSG_LOCAL_STATS: {
                StatsMsg* param = static_cast<StatsMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnLocalStats, stream_id: " << param->stream_id;
                event_handler_->OnLocalStats(param->stream_id.c_str(), param->audio_stats, param->video_stats);
                delete param;
                break;
            }
            default: {
                break;
            }
        }
    } else {
        switch (msg->message_id) {
            case MSG_STATE_CHANGED: {
                StateChangedMsg* param = static_cast<StateChangedMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnSubscribeStateChanged, stream_id: " << param->stream_id
                                 << ", state: " << param->state
                                 << ", reason: " << param->reason;
                event_handler_->OnSubscribeStateChanged(param->stream_id.c_str(), param->state, param->reason);
                delete param;
                break;
            }
            case MSG_REMOTE_STATS: {
                StatsMsg* param = static_cast<StatsMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnRemoteStats, stream_id: " << param->stream_id;
                event_handler_->OnRemoteStats(param->stream_id.c_str(), param->audio_stats, param->video_stats);
                delete param;
                break;
            }
            case MSG_FIRST_REMOTE_VIDEO_FRAME: {
                BaseMsg* param = static_cast<BaseMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnFirstRemoteVideoFrameRendered, stream_id: " << param->stream_id;
                event_handler_->OnFirstRemoteVideoFrameRendered(param->stream_id.c_str());
                delete param;
                break;
            }
            case MSG_STALL_INFO: {
                StallInfoMsg* param = static_cast<StallInfoMsg*>(msg->pdata);
                RTC_LOG(LS_INFO) << "[WTN_CB]OnStallInfo, stream_id: " << param->stream_id;
                event_handler_->OnStallInfo(param->stream_id.c_str(), param->stall_info);
                delete param;
                break;
            }
            default: {
                break;
            }
        }
    }
}

}  // end of namespace
