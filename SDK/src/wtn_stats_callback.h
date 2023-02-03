/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#pragma once

#include <rtc_base/thread.h>
#include "wtn_interface.h"
#include "api/stats/rtc_stats_collector_callback.h"
#include "api/stats/rtcstats_objects.h"
#include "wtn_event_handler.h"

namespace vewtn {

class RTCStatsCallback : public webrtc::RTCStatsCollectorCallback {
public:
    RTCStatsCallback(rtc::Thread* callback_thread, EventHandler* callback, bool is_pub, const std::string& stream_id);

protected:
    void OnStatsDelivered(const rtc::scoped_refptr<const webrtc::RTCStatsReport>& report) override;

private:
    rtc::Thread* callback_thread_ = nullptr;
    EventHandler* callback_ = nullptr;
    bool is_publisher_; 
    std::string stream_id_;
};

}  // namespace vewtn

