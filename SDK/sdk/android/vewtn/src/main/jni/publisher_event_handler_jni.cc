/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "publisher_event_handler_jni.h"
#include "sdk/android/native_api/jni/jvm.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "vewtn_jni_common.h"
#include "com/vewtn/internal/PublisherEventHandlerImpl_jni.h"

using webrtc::AttachCurrentThreadIfNeeded;

namespace webrtc {
namespace jni {

PublisherEventHandler::PublisherEventHandler(JNIEnv *env, const webrtc::JavaParamRef<jobject>& j_publisher_event_handler)
        : j_handler_(env, j_publisher_event_handler) {
}

PublisherEventHandler::~PublisherEventHandler() {
}

void PublisherEventHandler::OnPublishStateChanged(vewtn::PublishState state, vewtn::Reason reason) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_PublisherEventHandlerImpl_onPublishStateChanged(env, j_handler_, (int)state, (int)reason);
}

}
}
