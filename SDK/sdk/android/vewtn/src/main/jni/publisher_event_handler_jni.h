/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#ifndef WHITEBOARDDEMO_BOARD_EVENT_HANDLER_JNI_H
#define WHITEBOARDDEMO_BOARD_EVENT_HANDLER_JNI_H

#include <jni.h>
#include <memory>

#include "vewtn_interface.h"
#include "sdk/android/src/jni/jni_helpers.h"

namespace webrtc {
namespace jni {

class PublisherEventHandler: public vewtn::IPublisherEventHandler {
public:
    PublisherEventHandler(JNIEnv *env, const webrtc::JavaParamRef<jobject>& j_board_event_handler);
    ~PublisherEventHandler();

    void OnPublishStateChanged(vewtn::PublishState state,
                               vewtn::Reason reason) override;

private:
    webrtc::ScopedJavaGlobalRef<jobject> j_handler_;
};

}
}

#endif
