/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include <chrono>
#include <memory>
#include <array>
#include <string>
#include "com/vewtn/jni/NativeFunctions_jni.h"
#include "publisher_event_handler_jni.h"
#include "subscriber_event_handler_jni.h"
#include "vewtn_jni_common.h"
#include "vewtn_interface.h"

#include "sdk/android/native_api/video/wrapper.h"
#include "sdk/android/native_api/video/video_source.h"
#include "api/peer_connection_interface.h"
#include "api/scoped_refptr.h"
#include "api/sequence_checker.h"
#include "rtc_base/synchronization/mutex.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "api/rtc_event_log/rtc_event_log_factory.h"
#include "api/task_queue/default_task_queue_factory.h"
#include "media/engine/internal_decoder_factory.h"
#include "media/engine/internal_encoder_factory.h"
#include "media/engine/webrtc_media_engine.h"
#include "media/engine/webrtc_media_engine_defaults.h"
#include "sdk/android/native_api/jni/java_types.h"
#include "api/audio_codecs/audio_decoder_factory.h"
#include "api/audio_codecs/audio_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "api/video_codecs/video_encoder_factory.h"

#include "modules/utility/include/jvm_android.h"
#include "rtc_base/checks.h"
#include "sdk/android/native_api/codecs/wrapper.h"
#include "sdk/android/native_api/jni/class_loader.h"
#include "sdk/android/native_api/jni/jvm.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "sdk/android/src/jni/jvm.h"

namespace webrtc {
namespace jni {

rtc::scoped_refptr <webrtc::JavaVideoTrackSourceInterface> pub_video_source;
rtc::scoped_refptr <webrtc::JavaVideoTrackSourceInterface> sub_video_source;
std::unique_ptr <PublisherEventHandler> publisher_handler;
std::unique_ptr <SubscriberEventHandler> subscriber_handler;

static jlong JNI_NativeFunctions_CreatePublisher(JNIEnv *env, const
                                                 base::android::JavaParamRef <jobject> &context,
                                                 const base::android::JavaParamRef <jobject> &eventHandler,
                                                 const base::android::JavaParamRef <jobject> &hardwareVideoEncoderFactory,
                                                 const base::android::JavaParamRef <jobject> &hardwareVideoDecoderFactory) {
    publisher_handler = std::make_unique<PublisherEventHandler>(env,
                                                                eventHandler);
    std::unique_ptr <webrtc::VideoEncoderFactory> encoder_factory = JavaToNativeVideoEncoderFactory(
            env, hardwareVideoEncoderFactory.obj());
    std::unique_ptr <webrtc::VideoDecoderFactory> decoder_factory = JavaToNativeVideoDecoderFactory(
            env, hardwareVideoDecoderFactory.obj());

    long nativePublisherPtr = webrtc::NativeToJavaPointer(
            vewtn::CreatePublisher(publisher_handler.get(),
                                   nullptr,
                                   std::move(encoder_factory), std::move(decoder_factory)));
    return nativePublisherPtr;
}

static void JNI_NativeFunctions_DestroyPublisher(JNIEnv *env, jlong nativePublisherPtr) {
    JNI_CHECK_NULL_AND_RETURN(nativePublisherPtr);
    vewtn::DestroyPublisher(reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr));
    publisher_handler.reset();
    pub_video_source = nullptr;
}

static void JNI_NativeFunctions_SetLocalCanvas(JNIEnv *env, jlong nativePublisherPtr,
                                               const base::android::JavaParamRef <jobject> &localSink) {
    JNI_CHECK_NULL_AND_RETURN(nativePublisherPtr);
    std::unique_ptr <rtc::VideoSinkInterface<VideoFrame>> local_sink = webrtc::JavaToNativeVideoSink(
            env, localSink.obj());
    reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetLocalCanvas(local_sink);
}

static webrtc::ScopedJavaLocalRef<jobject> JNI_NativeFunctions_GetJavaVideoCapturerObserver(
        JNIEnv *env, jlong nativePublisherPtr) {
    JNI_CHECK_NULL_AND_RETURN_NULLPTR(nativePublisherPtr);
    if (!pub_video_source) {
        rtc::Thread *thread = reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->GetSignalingThread();
        pub_video_source = webrtc::CreateJavaVideoSource(env, thread, false, true);
        reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetVideoSource(
                pub_video_source);
    }
    return pub_video_source->GetJavaVideoCapturerObserver(env);
}

static jint JNI_NativeFunctions_StartPublish(JNIEnv *env, jlong nativePublisherPtr,
                                             const base::android::JavaParamRef <jstring> &url) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    if (!pub_video_source) {
        rtc::Thread *thread = reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->GetSignalingThread();
        pub_video_source = webrtc::CreateJavaVideoSource(env, thread, false, true);
        reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->SetVideoSource(
                pub_video_source);
    }
    std::string strUrl = webrtc::JavaToNativeString(env, url);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StartPublish(
            strUrl.c_str());
}

static jint JNI_NativeFunctions_StopPublish(JNIEnv *env, jlong nativePublisherPtr) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativePublisherPtr);
    return reinterpret_cast<vewtn::IPublisher *>(nativePublisherPtr)->StopPublish();
}

// subscribe
static jlong JNI_NativeFunctions_CreateSubscriber(JNIEnv *env,
                                                  const base::android::JavaParamRef <jobject> &context,
                                                  const base::android::JavaParamRef <jobject> &eventHandler,
                                                  const base::android::JavaParamRef <jobject> &hardwareVideoEncoderFactory,
                                                  const base::android::JavaParamRef <jobject> &hardwareVideoDecoderFactory) {
    subscriber_handler = std::make_unique<SubscriberEventHandler>(env,
                                                                  eventHandler);
    std::unique_ptr <webrtc::VideoEncoderFactory> encoder_factory = JavaToNativeVideoEncoderFactory(
            env, hardwareVideoEncoderFactory.obj());
    std::unique_ptr <webrtc::VideoDecoderFactory> decoder_factory = JavaToNativeVideoDecoderFactory(
            env, hardwareVideoDecoderFactory.obj());

    long nativeSubscriberPtr = webrtc::NativeToJavaPointer(
            vewtn::CreateSubscriber(subscriber_handler.get(), nullptr,
                                    std::move(encoder_factory), std::move(decoder_factory)));
    return nativeSubscriberPtr;
}

static void JNI_NativeFunctions_DestroySubscriber(JNIEnv *env, jlong nativeSubscriberPtr) {
    JNI_CHECK_NULL_AND_RETURN(nativeSubscriberPtr);
    vewtn::DestroySubscriber(reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr));
    subscriber_handler.reset();
    sub_video_source = nullptr;
}

static void JNI_NativeFunctions_SetRemoteCanvas(JNIEnv *env, jlong nativeSubscriberPtr,
                                                const base::android::JavaParamRef <jobject> &remoteSink) {
    JNI_CHECK_NULL_AND_RETURN(nativeSubscriberPtr);
    std::unique_ptr <rtc::VideoSinkInterface<VideoFrame>> remote_sink = webrtc::JavaToNativeVideoSink(
            env, remoteSink.obj());
    reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->SetRemoteCanvas(
            remote_sink);
}

static jint JNI_NativeFunctions_StartSubscribe(JNIEnv *env, jlong nativeSubscriberPtr,
                                               const base::android::JavaParamRef <jstring> &url) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    if (!sub_video_source) {
        rtc::Thread *thread = reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->GetSignalingThread();
        sub_video_source = webrtc::CreateJavaVideoSource(env, thread, false, true);
        reinterpret_cast<vewtn::IPublisher *>(nativeSubscriberPtr)->SetVideoSource(
                sub_video_source);
    }
    std::string strUrl = webrtc::JavaToNativeString(env, url);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->StartSubscribe(
            strUrl.c_str());
}

static jint JNI_NativeFunctions_StopSubscribe(JNIEnv *env, jlong nativeSubscriberPtr) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(nativeSubscriberPtr);
    return reinterpret_cast<vewtn::ISubscriber *>(nativeSubscriberPtr)->StopSubscribe();
}

} // namespace jni
} // namespace vewtn