/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "platform/android_platform_helper.h"

#include <jni.h>
#include <pthread.h>
#include <stddef.h>
#include <memory>
#include <modules/audio_device/include/audio_device.h>
#include "modules/utility/include/jvm_android.h"
#include <sdk/android/native_api/audio_device_module/audio_device_android.h>
#include <api/video/video_sink_interface.h>
#include "rtc_base/checks.h"
#include <sdk/android/native_api/video/wrapper.h>
#include <sdk/android/native_api/video/video_source.h>
#include <sdk/android/src/jni/android_video_track_source.h>
#include "sdk/android/native_api/codecs/wrapper.h"
#include "sdk/android/native_api/jni/class_loader.h"
#include "sdk/android/native_api/jni/jvm.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "sdk/android/src/jni/jvm.h"

namespace vewtn {

std::unique_ptr<webrtc::VideoEncoderFactory> CreateAndroidEncoderFactory() {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    webrtc::ScopedJavaLocalRef<jclass> factory_class =
            webrtc::GetClass(env, "org/webrtc/HardwareVideoEncoderFactory");
    jmethodID factory_constructor = env->GetMethodID(
            factory_class.obj(), "<init>", "(Lorg/webrtc/EglBase$Context;ZZ)V");
    webrtc::ScopedJavaLocalRef<jobject> factory_object(
            env, env->NewObject(factory_class.obj(), factory_constructor,
                                nullptr /* shared_context */,
                                false /* enable_intel_vp8_encoder */,
                                true /* enable_h264_high_profile */));
    return webrtc::JavaToNativeVideoEncoderFactory(env, factory_object.obj());
}

std::unique_ptr<webrtc::VideoDecoderFactory> CreateAndroidDecoderFactory() {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    webrtc::ScopedJavaLocalRef<jclass> factory_class =
            webrtc::GetClass(env, "org/webrtc/HardwareVideoDecoderFactory");
    jmethodID factory_constructor = env->GetMethodID(
            factory_class.obj(), "<init>", "(Lorg/webrtc/EglBase$Context;)V");
    webrtc::ScopedJavaLocalRef<jobject> factory_object(
            env, env->NewObject(factory_class.obj(), factory_constructor,
                                nullptr /* shared_context */));
    return webrtc::JavaToNativeVideoDecoderFactory(env, factory_object.obj());
}

std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>> JavaToNativeVideoSink(void *remote_view) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    auto sink = webrtc::JavaToNativeVideoSink(env, (jobject)remote_view);
    return std::move(sink);
};

rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateAndroidVideoTrackSource(rtc::Thread *signaling_thread) {
    JNIEnv *env = webrtc::AttachCurrentThreadIfNeeded();
    return rtc::make_ref_counted<webrtc::jni::AndroidVideoTrackSource>(
            signaling_thread, env, /*is_screencast=*/false,
            /*align_timestamps=*/true);
}

void StartAndroidVideoCapturer(
        rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> source,
        bool is_front, int width, int height, int fps) {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/PublisherImpl");
    jmethodID link_camera_method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "StartCamera",
            "(JZIII)V");
    env->CallStaticVoidMethod(pc_factory_class.obj(), link_camera_method,
                              (jlong) source.get(), (jboolean)is_front, (jint) width, (jint) height,
                              (jint) fps);
    CHECK_EXCEPTION(env);
}

void StopAndroidVideoCaptuer() {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/PublisherImpl");
    jmethodID stop_camera_method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "StopCamera", "()V");
    env->CallStaticVoidMethod(pc_factory_class.obj(), stop_camera_method);
    CHECK_EXCEPTION(env);
}

void SwitchAndroidVideoCaptuer() {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/PublisherImpl");
    jmethodID stop_camera_method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "SwitchCamera", "()V");

    env->CallStaticVoidMethod(pc_factory_class.obj(), stop_camera_method);
    CHECK_EXCEPTION(env);
}

void SetAndroidEnableSpeaker(bool enable) {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/SubscriberImpl");
    jmethodID link_camera_method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "SetSpeakerphoneOn",
            "(Z)V");
    env->CallStaticVoidMethod(pc_factory_class.obj(), link_camera_method, (jboolean)enable);
    CHECK_EXCEPTION(env);
}

bool CheckRecordPermission() {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/PublisherImpl");
    jmethodID method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "checkRecordPermission", "()Z");
    jboolean ret = env->CallStaticBooleanMethod(pc_factory_class.obj(), method);
    CHECK_EXCEPTION(env);
    return ret;
}

bool CheckCameraPermission() {
    JNIEnv *env = webrtc::jni::GetEnv();
    webrtc::ScopedJavaLocalRef<jclass> pc_factory_class = webrtc::GetClass(env, "com/vewtn/internal/PublisherImpl");
    jmethodID method = webrtc::GetStaticMethodID(
            env, pc_factory_class.obj(), "checkCameraPermission", "()Z");
    jboolean ret = env->CallStaticBooleanMethod(pc_factory_class.obj(), method);
    CHECK_EXCEPTION(env);
    return ret;
}

bool CheckPermission() {
    return CheckCameraPermission() && CheckRecordPermission();
}

}  // namespace vewtn
