/*
*  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
*
*  SPDX-license-identifier: BSD-3-Clause
*/

#include <chrono>
#include <memory>
#include <array>
#include <string>
#include <sdk/android/native_api/audio_device_module/audio_device_android.h>
#include "com/vewtn/jni/NativeFunctions_jni.h"
#include "vewtn_jni_common.h"
#include "wtn_interface.h"
#include "modules/utility/include/jvm_android.h"
#include "rtc_base/checks.h"
#include "sdk/android/native_api/codecs/wrapper.h"
#include "sdk/android/native_api/jni/class_loader.h"
#include "sdk/android/native_api/jni/jvm.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "sdk/android/src/jni/jvm.h"
#include "sdk/android/native_api/jni/java_types.h"
#include "rtc_event_handler_jni.h"

namespace webrtc {
namespace jni {

std::unique_ptr<RtcEventHandler> event_handler;

static jlong JNI_NativeFunctions_CreateRtcEngine(JNIEnv *env, const
base::android::JavaParamRef<jobject> &context,
                                                 const base::android::JavaParamRef<jobject> &eventHandler) {
    event_handler = std::make_unique<RtcEventHandler>(env, eventHandler);
    long nativePtr = webrtc::NativeToJavaPointer(
            vewtn::CreateRtcEngine(event_handler.get()));
    return nativePtr;
}

static void JNI_NativeFunctions_DestroyRtcEngine(JNIEnv *env, jlong nativePtr) {
    JNI_CHECK_NULL_AND_RETURN(nativePtr);
    vewtn::DestroyRtcEngine(reinterpret_cast<vewtn::IRtcEngine *>(nativePtr));
}

static jlong JNI_NativeFunctions_CreatePublisher(JNIEnv *env, jlong nativePtr,
                                                 jint pubAudioSource,
                                                 jint pubVideoSource) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePtr);
    vewtn::PubExtraParam pubExtraParam;
    pubExtraParam.pub_audio_source = (vewtn::PubAudioSource) pubAudioSource;
    pubExtraParam.pub_video_source = (vewtn::PubVideoSource) pubVideoSource;
    return webrtc::NativeToJavaPointer(
            reinterpret_cast<vewtn::IRtcEngine *>(nativePtr)->CreatePublisher(
                    pubExtraParam));
}

static void JNI_NativeFunctions_DestroyPublisher(JNIEnv *env, jlong nativePtr,
                                                 jlong nativePublisherPtr) {
    JNI_CHECK_NULL_AND_RETURN(nativePtr);
    reinterpret_cast<vewtn::IRtcEngine *>(nativePtr)->DestroyPublisher(
            reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr));
}

static void JNI_NativeFunctions_SetVideoConfig(JNIEnv *env, jlong nativePublisherPtr,
                                               jint width,
                                               jint height,
                                               jint maxFPS) {
    vewtn::VideoConfig videoConfig;
    videoConfig.width = width;
    videoConfig.height = height;
    videoConfig.max_fps = maxFPS;
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetVideoConfig(
            videoConfig);
}

static void JNI_NativeFunctions_SetLocalView(JNIEnv *env, jlong nativePublisherPtr,
                                             const base::android::JavaParamRef<jobject> &localSink) {
    JNI_CHECK_NULL_AND_RETURN(nativePublisherPtr);
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetLocalView(
            env->NewGlobalRef(localSink.obj()));
}

static void JNI_NativeFunctions_StartVideoCapture(JNIEnv *env, jlong nativePublisherPtr) {
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StartVideoCapture();
}

static void JNI_NativeFunctions_StopVideoCapture(JNIEnv *env, jlong nativePublisherPtr) {
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StopVideoCapture();
}

static void JNI_NativeFunctions_SwitchCamera(JNIEnv *env, jlong nativePublisherPtr) {
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SwitchCamera();
}

static void JNI_NativeFunctions_StartAudioCapture(JNIEnv *env, jlong nativePublisherPtr) {
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StartAudioCapture();
}

static void JNI_NativeFunctions_StopAudioCapture(JNIEnv *env, jlong nativePublisherPtr) {
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StopAudioCapture();
}

static jboolean JNI_NativeFunctions_IsPublishing(JNIEnv *env, jlong nativePublisherPtr) {
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->IsPublishing();
}

static jint JNI_NativeFunctions_StartPublish(JNIEnv *env, jlong nativePublisherPtr,
                                             const base::android::JavaParamRef<jstring> &url) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    std::string strUrl = webrtc::JavaToNativeString(env, url);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StartPublish(
            strUrl.c_str());
}

static jint JNI_NativeFunctions_StopPublish(JNIEnv *env, jlong nativePublisherPtr) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StopPublish();
}

static jint JNI_NativeFunctions_SetLocalAudioMute(JNIEnv *env, jlong nativePublisherPtr,
                                                  jboolean mute) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetLocalAudioMute(
            mute);
}

static jint JNI_NativeFunctions_SetLocalVideoMute(JNIEnv *env, jlong nativePublisherPtr,
                                                  jboolean mute) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetLocalVideoMute(
            mute);
}

static jboolean
JNI_NativeFunctions_PushEncodedVideoFrame(JNIEnv *env, jlong nativePublisherPtr,
                                          jint codec_type,
                                          const base::android::JavaParamRef<jbyteArray> &data,
                                          jint size,
                                          jint width,
                                          jint height,
                                          jlong timestamp_ms,
                                          jint rotation,
                                          jboolean is_key_frame) {
    jbyte *bytes = env->GetByteArrayElements(data.obj(), 0);
    vewtn::EncodedVideoFrame encodedFrame;
    encodedFrame.codec_type = (vewtn::EncodedVideoFrameType) codec_type;
    encodedFrame.data = (uint8_t *) bytes;
    encodedFrame.size = size;
    encodedFrame.width = width;
    encodedFrame.height = height;
    encodedFrame.timestamp_ms = timestamp_ms;
    encodedFrame.rotation = rotation;
    encodedFrame.is_key_frame = is_key_frame;
    jboolean ret = reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->PushEncodedVideoFrame(
            encodedFrame);
    env->ReleaseByteArrayElements(data.obj(), bytes, 0);
    return ret;
}


static jboolean JNI_NativeFunctions_PushVideoFrame(JNIEnv *env, jlong nativePublisherPtr,
                                                   jint type,
                                                   jint width,
                                                   jint height,
                                                   const base::android::JavaParamRef<jbyteArray> &y_buffer,
                                                   const base::android::JavaParamRef<jbyteArray> &u_buffer,
                                                   const base::android::JavaParamRef<jbyteArray> &v_buffer,
                                                   jint y_stride,
                                                   jint u_stride,
                                                   jint v_stride,
                                                   jlong timestamp_ms,
                                                   jint rotation) {

    vewtn::VideoFrame videoFrame;
    videoFrame.type = (vewtn::VideoFrameType) type;
    jbyte *y_bytes = env->GetByteArrayElements(y_buffer.obj(), 0);
    videoFrame.y_buffer = (uint8_t *) y_bytes;
    videoFrame.y_stride = y_stride;
    jbyte *u_bytes = env->GetByteArrayElements(u_buffer.obj(), 0);
    videoFrame.u_buffer = (uint8_t *) u_bytes;
    videoFrame.u_stride = u_stride;
    jbyte *v_bytes = env->GetByteArrayElements(v_buffer.obj(), 0);
    videoFrame.v_buffer = (uint8_t *) v_bytes;
    videoFrame.v_stride = v_stride;
    videoFrame.width = width;
    videoFrame.height = height;
    videoFrame.timestamp_ms = timestamp_ms;
    videoFrame.rotation = rotation;
    jboolean ret = reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->PushVideoFrame(
            videoFrame);
    env->ReleaseByteArrayElements(y_buffer.obj(), y_bytes, 0);
    env->ReleaseByteArrayElements(u_buffer.obj(), u_bytes, 0);
    env->ReleaseByteArrayElements(v_buffer.obj(), v_bytes, 0);
    return ret;
}

static jboolean JNI_NativeFunctions_PushAudioFrame(JNIEnv *env, jlong nativePublisherPtr,
                                                   jint type,
                                                   const base::android::JavaParamRef<jbyteArray> &data,
                                                   jint sample_length,
                                                   jlong timestamp_us,
                                                   jint sample_rate,
                                                   jint channels) {
    jbyte *bytes = env->GetByteArrayElements(data.obj(), 0);
    vewtn::AudioFrame audioFrame;
    audioFrame.type = (vewtn::AudioFrameType) type;
    audioFrame.data = (uint8_t *) bytes;
    audioFrame.sample_length = sample_length;
    audioFrame.timestamp_ms = timestamp_us;
    audioFrame.sample_rate = sample_rate;
    audioFrame.channels = channels;
    jboolean ret = reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->PushAudioFrame(
            audioFrame);
    env->ReleaseByteArrayElements(data.obj(), bytes, 0);
    return ret;
}

// subscribe
static jlong JNI_NativeFunctions_CreateSubscriber(JNIEnv *env, jlong nativePtr,
                                                  jint subAudioSource,
                                                  jint subVideoSource) {
    vewtn::SubExtraParam subExtraParam;
    subExtraParam.sub_audio_source = (vewtn::SubAudioSource) subAudioSource;
    subExtraParam.sub_video_source = (vewtn::SubVideoSource) subVideoSource;
    return webrtc::NativeToJavaPointer(
            reinterpret_cast<vewtn::IRtcEngine *>(nativePtr)->CreateSubscriber(
                    subExtraParam));
}

static void JNI_NativeFunctions_DestroySubscriber(JNIEnv *env, jlong nativePtr,
                                                  jlong nativeSubscriberPtr) {
    JNI_CHECK_NULL_AND_RETURN(nativePtr);
    reinterpret_cast<vewtn::IRtcEngine *>(nativePtr)->DestroySubscriber(
            reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr));
}

static void JNI_NativeFunctions_SetRemoteView(JNIEnv *env, jlong nativeSubscriberPtr,
                                              const base::android::JavaParamRef<jobject> &remoteSink) {
    JNI_CHECK_NULL_AND_RETURN(nativeSubscriberPtr);
    reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->SetRemoteView(
            env->NewGlobalRef(remoteSink.obj()));
}

static void JNI_NativeFunctions_SetEnableSpeaker(JNIEnv *env, jlong nativeSubscriberPtr,
                                                 jboolean enable) {
    reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->SetEnableSpeaker(
            enable);
}

static jboolean JNI_NativeFunctions_IsSubscribed(JNIEnv *env, jlong nativeSubscriberPtr) {
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->IsSubscribed();
}

static jint JNI_NativeFunctions_StartSubscribe(JNIEnv *env, jlong nativeSubscriberPtr,
                                               const base::android::JavaParamRef<jstring> &url) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    std::string strUrl = webrtc::JavaToNativeString(env, url);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->StartSubscribe(
            strUrl.c_str());
}

static jint JNI_NativeFunctions_StopSubscribe(JNIEnv *env, jlong nativeSubscriberPtr) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->StopSubscribe();
}

static jint JNI_NativeFunctions_SetRemoteAudioMute(JNIEnv *env, jlong nativeSubscriberPtr,
                                                   jboolean mute) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->SetRemoteAudioMute(
            mute);
}

static jint JNI_NativeFunctions_SetRemoteVideoMute(JNIEnv *env, jlong nativeSubscriberPtr,
                                                   jboolean mute) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->SetRemoteVideoMute(
            mute);
}

} // namespace jni
} // namespace vewtn