/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include <jni.h>
#include <memory>

#include "vewtn_interface.h"
#include "sdk/android/src/jni/jni_helpers.h"

namespace webrtc {
namespace jni {

class SubscriberEventHandler: public vewtn::ISubscriberEventHandler {
public:
    SubscriberEventHandler(JNIEnv *env, const webrtc::JavaParamRef<jobject>& j_event_handler);
    ~SubscriberEventHandler();

    void OnSubscribeStateChanged(vewtn::SubscribeState state,
                               vewtn::Reason reason) override;

private:
    webrtc::ScopedJavaGlobalRef<jobject> j_handler_;
};

}
}

