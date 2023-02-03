/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include <map>
#include <rtc_base/synchronization/mutex.h>

namespace vewtn {

class RtcEngine : public IRtcEngine {
public:
    static IRtcEngine* GetInstance(IRtcEventHandler* event_handler);
    static void ReleaseInstance(const IRtcEngine* engine);

    IPublisher* CreatePublisher(const PubExtraParam& pub_extra_param) override;
    void DestroyPublisher(const IPublisher* publisher_ptr) override;

    ISubscriber* CreateSubscriber(const SubExtraParam& sub_extra_param) override;
    void DestroySubscriber(const ISubscriber* subscriber_ptr) override;

private:
    RtcEngine(IRtcEventHandler* event_handler);
    virtual ~RtcEngine();
    bool CheckInternalDeviceIsUsed(const PubExtraParam& pub_extra_param);

private:
    static RtcEngine* rtc_instance_;
    webrtc::Mutex mutex_;
    IRtcEventHandler* event_handler_;
    std::map<const IPublisher*, PubExtraParam> pub_maps_;
    std::map<const ISubscriber*, SubExtraParam> sub_maps_;
};

}    // namespace vewtn
