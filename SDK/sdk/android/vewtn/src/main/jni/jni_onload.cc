/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <jni.h>
#undef JNIEXPORT
#define JNIEXPORT __attribute__((visibility("default")))

#include "sdk/android/src/jni/class_reference_holder.h"
#include "sdk/android/src/jni/jni_helpers.h"
#include <android/log.h>
#include "modules/utility/include/jvm_android.h"
#include "rtc_base/ssl_adapter.h"
#include "sdk/android/native_api/base/init.h"

namespace webrtc {
namespace jni {

extern "C" jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
    webrtc::InitAndroid(jvm);
    webrtc::JVM::Initialize(jvm);
    RTC_CHECK(rtc::InitializeSSL()) << "Failed to InitializeSSL()";

    __android_log_print(ANDROID_LOG_ERROR, "base rtc_jni JNI_ONLOAD", "jni onload success");
    return JNI_VERSION_1_6;
}

extern "C" void JNIEXPORT JNICALL JNI_OnUnLoad(JavaVM* jvm, void* reserved) {
    RTC_CHECK(rtc::CleanupSSL()) << "Failed to CleanupSSL()";
}

}  // namespace jni
}  // namespace webrtc
