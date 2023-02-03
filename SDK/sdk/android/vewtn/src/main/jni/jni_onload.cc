/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
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
    return JNI_VERSION_1_6;
}

extern "C" void JNIEXPORT JNICALL JNI_OnUnLoad(JavaVM* jvm, void* reserved) {
    RTC_CHECK(rtc::CleanupSSL()) << "Failed to CleanupSSL()";
}

}  // namespace jni
}  // namespace webrtc
