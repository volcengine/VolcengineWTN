/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#include "wtn_stats_callback.h"
#include "rtc_base/logging.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <rtc_base/location.h>

namespace vewtn {

RTCStatsCallback::RTCStatsCallback(rtc::Thread* callback_thread, EventHandler* callback, bool is_pub, const std::string& stream_id)
    : callback_thread_(callback_thread),
      callback_(callback),
      is_publisher_(is_pub),
      stream_id_(stream_id) {
}

void RTCStatsCallback::OnStatsDelivered(const rtc::scoped_refptr<const webrtc::RTCStatsReport>& report) {
    RTCAudioStats audio_stats;
    RTCVideoStats video_stats;
    if (is_publisher_) {
        auto outbound = report->GetStatsOfType<webrtc::RTCOutboundRTPStreamStats>();
        for (const auto& stat : outbound) {
            if (stat->kind.is_defined()) {
                static int64_t last_audio_sent = 0;
                static int64_t last_video_sent = 0;
                std::string kind = stat->kind.ValueToString();
                if (kind == "audio") {
                    if (stat->bytes_sent.is_defined()) {
                        if (last_audio_sent == 0) {
                            audio_stats.bytes_sent = stat->bytes_sent.ValueOrDefault(0ul);
                            last_audio_sent = audio_stats.bytes_sent;
                            audio_stats.bytes_sent_per_second = last_audio_sent;
                        } else {
                            audio_stats.bytes_sent = stat->bytes_sent.ValueOrDefault(0ul);
                            audio_stats.bytes_sent_per_second = audio_stats.bytes_sent - last_audio_sent;
                            last_audio_sent = audio_stats.bytes_sent;
                        }
                    }
                    if (stat->packets_sent.is_defined()) {
                        audio_stats.packets_sent = stat->packets_sent.ValueOrDefault(0ul);
                    }
                    auto audio_source = report->GetStatsOfType<webrtc::RTCAudioSourceStats>();
                    for (const auto& stat : audio_source) {
                        if (stat->audio_level.is_defined()) {
                            audio_stats.audio_level = stat->audio_level.ValueOrDefault(0ul);
                        }
                        if (stat->total_audio_energy.is_defined()) {
                            audio_stats.total_audio_energy = stat->total_audio_energy.ValueOrDefault(0ul);
                        }
                    }
                } else if (kind == "video") {
                    if (stat->bytes_sent.is_defined()) {
                        if (last_video_sent == 0) {
                            video_stats.bytes_sent = stat->bytes_sent.ValueOrDefault(0ul);
                            last_video_sent = video_stats.bytes_sent;
                            video_stats.bytes_sent_per_second = last_video_sent;
                        } else {
                            video_stats.bytes_sent = stat->bytes_sent.ValueOrDefault(0ul);
                            video_stats.bytes_sent_per_second = video_stats.bytes_sent - last_video_sent;
                            last_video_sent = video_stats.bytes_sent;
                        }
                    }
                    if (stat->frame_width.is_defined()) {
                        video_stats.frame_width = stat->frame_width.ValueOrDefault(0ul);
                    }
                    if (stat->frame_height.is_defined()) {
                        video_stats.frame_height = stat->frame_height.ValueOrDefault(0ul);
                    }
                    if (stat->frames_per_second.is_defined()) {
                        video_stats.frames_per_second = stat->frames_per_second.ValueOrDefault(0ul);
                    }
                    if (stat->packets_sent.is_defined()) {
                        video_stats.packets_sent = stat->packets_sent.ValueOrDefault(0ul);
                    }
                }
            }
        }
        
        auto remote_inbound = report->GetStatsOfType<webrtc::RTCRemoteInboundRtpStreamStats>();
        for (const auto& stat : remote_inbound) {
            if (stat->kind.is_defined()) {
                std::string kind = stat->kind.ValueToString();
                if (kind == "audio") {
                    if (stat->total_round_trip_time.is_defined()) {
                        audio_stats.total_round_trip_time = stat->total_round_trip_time.ValueOrDefault(0ul);
                    }
                    if (stat->round_trip_time_measurements.is_defined()) {
                        audio_stats.round_trip_time_measurements = stat->round_trip_time_measurements.ValueOrDefault(0ul);
                    }
                    if (stat->packets_lost.is_defined()) {
                        audio_stats.packets_lost = stat->packets_lost.ValueOrDefault(0ul);
                    }
                    if (stat->jitter.is_defined()) {
                        audio_stats.jitter = stat->jitter.ValueOrDefault(0ul);
                    }
                } else if (kind == "video") {
                    if (stat->total_round_trip_time.is_defined()) {
                        video_stats.total_round_trip_time = stat->total_round_trip_time.ValueOrDefault(0ul);
                    }
                    if (stat->round_trip_time_measurements.is_defined()) {
                        video_stats.round_trip_time_measurements = stat->round_trip_time_measurements.ValueOrDefault(0ul);
                    }
                    if (stat->packets_lost.is_defined()) {
                        video_stats.packets_lost = stat->packets_lost.ValueOrDefault(0ul);
                    }
                    if (stat->jitter.is_defined()) {
                        video_stats.jitter = stat->jitter.ValueOrDefault(0ul);
                    }
                }
            }
        }

        if (audio_stats.packets_sent != 0) {
            audio_stats.franction_loss = audio_stats.packets_lost / audio_stats.packets_sent;
        }
        if (video_stats.packets_sent != 0) {
            video_stats.franction_loss = video_stats.packets_lost / video_stats.packets_sent;
        }

        if (callback_thread_ && callback_) {
            auto msg = new StatsMsg(stream_id_, audio_stats, video_stats);
            callback_thread_->Post(RTC_FROM_HERE, callback_, MSG_LOCAL_STATS, msg);
        }
    } 

    if (!is_publisher_) {
        auto inbound = report->GetStatsOfType<webrtc::RTCInboundRTPStreamStats>();
        for (const auto& stat : inbound) {
            if (stat->kind.is_defined()) {
                static int64_t last_audio_recv = 0;
                static int64_t last_video_recv = 0;
                std::string kind = stat->kind.ValueToString();
                if (kind == "audio") {
                    if (stat->bytes_received.is_defined()) {
                        if (last_audio_recv == 0) {
                            audio_stats.bytes_received = stat->bytes_received.ValueOrDefault(0ul);
                            last_audio_recv = audio_stats.bytes_received;
                            audio_stats.bytes_received_per_second = last_audio_recv;
                        } else {
                            audio_stats.bytes_received = stat->bytes_received.ValueOrDefault(0ul);
                            audio_stats.bytes_received_per_second = audio_stats.bytes_received - last_audio_recv;
                            last_audio_recv = audio_stats.bytes_received;
                        }
                    }
                    if (stat->audio_level.is_defined()) {
                        audio_stats.audio_level = stat->audio_level.ValueOrDefault(0ul);
                    }
                    if (stat->total_audio_energy.is_defined()) {
                        audio_stats.total_audio_energy = stat->total_audio_energy.ValueOrDefault(0ul);
                    }
                    if (stat->packets_received.is_defined()) {
                        audio_stats.packets_received = stat->packets_received.ValueOrDefault(0ul);
                    }
                    if (stat->jitter.is_defined()) {
                        audio_stats.jitter = stat->jitter.ValueOrDefault(0ul);
                    }
                } else if (kind == "video") {
                    if (stat->bytes_received.is_defined()) {
                        if (last_video_recv == 0) {
                            video_stats.bytes_received = stat->bytes_received.ValueOrDefault(0ul);
                            last_video_recv = video_stats.bytes_received;
                            video_stats.bytes_received_per_second = last_video_recv;
                        } else {
                            video_stats.bytes_received = stat->bytes_received.ValueOrDefault(0ul);
                            video_stats.bytes_received_per_second = video_stats.bytes_received - last_video_recv;
                            last_video_recv = video_stats.bytes_received;
                        }
                    }
                    if (stat->frame_width.is_defined()) {
                        video_stats.frame_width = stat->frame_width.ValueOrDefault(0ul);
                    }
                    if (stat->frame_height.is_defined()) {
                        video_stats.frame_height = stat->frame_height.ValueOrDefault(0ul);
                    }
                    if (stat->frames_per_second.is_defined()) {
                        video_stats.frames_per_second = stat->frames_per_second.ValueOrDefault(0ul);
                    }
                    if (stat->packets_received.is_defined()) {
                        video_stats.packets_received = stat->packets_received.ValueOrDefault(0ul);
                    }
                    if (stat->jitter.is_defined()) {
                        video_stats.jitter = stat->jitter.ValueOrDefault(0ul);
                    }
                }
            }
        }

        auto remote_outbound = report->GetStatsOfType<webrtc::RTCRemoteOutboundRtpStreamStats>();
        for (const auto& stat : remote_outbound) {
            if (stat->kind.is_defined()) {
                std::string kind = stat->kind.ValueToString();
                if (kind == "audio") {
                    if (stat->total_round_trip_time.is_defined()) {
                        audio_stats.total_round_trip_time = stat->total_round_trip_time.ValueOrDefault(0ul);
                    }
                    if (stat->round_trip_time_measurements.is_defined()) {
                        audio_stats.round_trip_time_measurements = stat->round_trip_time_measurements.ValueOrDefault(0ul);
                        
                    } 
                    if (stat->packets_sent.is_defined()) {
                        audio_stats.packets_sent = stat->packets_sent.ValueOrDefault(0ul);
                    } 
                } else if (kind == "video") {
                    if (stat->total_round_trip_time.is_defined()) {
                        video_stats.total_round_trip_time = stat->total_round_trip_time.ValueOrDefault(0ul);
                    }
                    if (stat->round_trip_time_measurements.is_defined()) {
                        video_stats.round_trip_time_measurements = stat->round_trip_time_measurements.ValueOrDefault(0ul);
                    } 
                    if (stat->packets_sent.is_defined()) {
                        video_stats.packets_sent = stat->packets_sent.ValueOrDefault(0ul);
                    } 
                }
            }
        }

        if (audio_stats.packets_sent != 0) {
            audio_stats.franction_loss = (audio_stats.packets_sent - audio_stats.packets_received) / audio_stats.packets_sent;
        }
        if (video_stats.packets_sent != 0) {
            video_stats.franction_loss = (video_stats.packets_sent - video_stats.packets_received) / video_stats.packets_sent;
        }

        if (callback_thread_ && callback_) {
            auto msg = new StatsMsg(stream_id_, audio_stats, video_stats);
            callback_thread_->Post(RTC_FROM_HERE, callback_, MSG_REMOTE_STATS, msg);
        }
    }
}

}  // namespace vewtn
