/*
*  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
*
*  SPDX-license-identifier: BSD-3-Clause
*/

#include <com/vewtn/internal/InternalRtcAudioStats_jni.h>
#include <com/vewtn/internal/InternalRtcVideoStats_jni.h>
#include <com/vewtn/internal/InternalEncodedVideoFrame_jni.h>
#include <com/vewtn/internal/InternalVideoFrame_jni.h>
#include <com/vewtn/internal/InternalAudioFrame_jni.h>
#include <com/vewtn/internal/InternalStallInfo_jni.h>
#include "rtc_event_handler_jni.h"
#include "sdk/android/native_api/jni/jvm.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"
#include "vewtn_jni_common.h"
#include "com/vewtn/internal/RtcEventHandlerImpl_jni.h"

using webrtc::AttachCurrentThreadIfNeeded;

namespace webrtc {
namespace jni {

RtcEventHandler::RtcEventHandler(JNIEnv *env,
                                 const webrtc::JavaParamRef<jobject> &j_publisher_event_handler)
        : j_handler_(env, j_publisher_event_handler) {
}

RtcEventHandler::~RtcEventHandler() {
}

void
RtcEventHandler::OnPublishStateChanged(const char *stream_id, vewtn::State state,
                                       vewtn::Reason reason) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onPublishStateChanged(env, j_handler_,
                                                   webrtc::NativeToJavaString(env,
                                                                              stream_id),
                                                   (int) state, (int) reason);
}

void
RtcEventHandler::OnLocalStats(const char *stream_id, vewtn::RTCAudioStats audio_stats,
                              vewtn::RTCVideoStats video_stats) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onLocalStats(env, j_handler_,
                                          webrtc::NativeToJavaString(env, stream_id),
                                          Java_InternalRtcAudioStats_create(env,
                                                                            audio_stats.bytes_sent,
                                                                            audio_stats.bytes_received,
                                                                            audio_stats.bytes_sent_per_second,
                                                                            audio_stats.bytes_received_per_second,
                                                                            audio_stats.audio_level,
                                                                            audio_stats.total_audio_energy,
                                                                            audio_stats.round_trip_time_measurements,
                                                                            audio_stats.total_round_trip_time,
                                                                            audio_stats.packets_sent,
                                                                            audio_stats.packets_received,
                                                                            audio_stats.packets_lost,
                                                                            audio_stats.franction_loss,
                                                                            audio_stats.jitter),
                                          Java_InternalRtcVideoStats_create(env,
                                                                            video_stats.bytes_sent,
                                                                            video_stats.bytes_received,
                                                                            video_stats.bytes_sent_per_second,
                                                                            video_stats.bytes_received_per_second,
                                                                            video_stats.frame_width,
                                                                            video_stats.frame_height,
                                                                            video_stats.frames_per_second,
                                                                            video_stats.round_trip_time_measurements,
                                                                            video_stats.total_round_trip_time,
                                                                            video_stats.packets_sent,
                                                                            video_stats.packets_received,
                                                                            video_stats.packets_lost,
                                                                            video_stats.franction_loss,
                                                                            video_stats.jitter));
}

void RtcEventHandler::OnMuteAudioSuccess(const char *stream_id) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onMuteAudioSuccess(env, j_handler_,
                                                webrtc::NativeToJavaString(env, stream_id));
}

void RtcEventHandler::OnMuteAudioFailed(const char *stream_id, vewtn::Reason reason) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onMuteAudioFailed(env, j_handler_,
                                               webrtc::NativeToJavaString(env, stream_id),
                                               (int) reason);
}

void RtcEventHandler::OnMuteVideoSuccess(const char *stream_id) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onMuteVideoSuccess(env, j_handler_,
                                                webrtc::NativeToJavaString(env, stream_id));
}

void RtcEventHandler::OnMuteVideoFailed(const char *stream_id, vewtn::Reason reason) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onMuteVideoFailed(env, j_handler_,
                                               webrtc::NativeToJavaString(env, stream_id),
                                               (int) reason);
}

void RtcEventHandler::OnSubscribeStateChanged(const char *stream_id,
                                              vewtn::State state,
                                              vewtn::Reason reason) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onSubscribeStateChanged(env, j_handler_,
                                                     webrtc::NativeToJavaString(env,
                                                                                stream_id),
                                                     (int) state, (int) reason);
}

void
RtcEventHandler::OnRemoteStats(const char *stream_id, vewtn::RTCAudioStats audio_stats,
                               vewtn::RTCVideoStats video_stats) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onRemoteStats(env, j_handler_,
                                           webrtc::NativeToJavaString(env, stream_id),
                                           Java_InternalRtcAudioStats_create(env,
                                                                             audio_stats.bytes_sent,
                                                                             audio_stats.bytes_received,
                                                                             audio_stats.bytes_sent_per_second,
                                                                             audio_stats.bytes_received_per_second,
                                                                             audio_stats.audio_level,
                                                                             audio_stats.total_audio_energy,
                                                                             audio_stats.round_trip_time_measurements,
                                                                             audio_stats.total_round_trip_time,
                                                                             audio_stats.packets_sent,
                                                                             audio_stats.packets_received,
                                                                             audio_stats.packets_lost,
                                                                             audio_stats.franction_loss,
                                                                             audio_stats.jitter),
                                           Java_InternalRtcVideoStats_create(env,
                                                                             video_stats.bytes_sent,
                                                                             video_stats.bytes_received,
                                                                             video_stats.bytes_sent_per_second,
                                                                             video_stats.bytes_received_per_second,
                                                                             video_stats.frame_width,
                                                                             video_stats.frame_height,
                                                                             video_stats.frames_per_second,
                                                                             video_stats.round_trip_time_measurements,
                                                                             video_stats.total_round_trip_time,
                                                                             video_stats.packets_sent,
                                                                             video_stats.packets_received,
                                                                             video_stats.packets_lost,
                                                                             video_stats.franction_loss,
                                                                             video_stats.jitter));
}

void RtcEventHandler::OnFirstRemoteVideoFrameRendered(const char *stream_id) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onFirstRemoteVideoFrameRendered(env, j_handler_,
                                                             webrtc::NativeToJavaString(env,
                                                                                        stream_id));
}

void RtcEventHandler::OnStallInfo(const char *stream_id, vewtn::StallInfo stall_info) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    Java_RtcEventHandlerImpl_onStallInfo(env, j_handler_,
                                         webrtc::NativeToJavaString(env, stream_id),
                                         Java_InternalStallInfo_create(env,
                                                                       stall_info.stall_count_500,
                                                                       stall_info.stall_duration_500,
                                                                       stall_info.stall_count_200,
                                                                       stall_info.stall_duration_200,
                                                                       stall_info.frame_rate));
}

void RtcEventHandler::OnEncodedVideoFrame(const char *stream_id,
                                          vewtn::EncodedVideoFrame encodedFrame) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    jbyteArray byte_array = env->NewByteArray(encodedFrame.size);
    env->SetByteArrayRegion(byte_array, 0, encodedFrame.size,
                            reinterpret_cast<const jbyte *>(encodedFrame.data));
    Java_RtcEventHandlerImpl_onEncodedVideoFrame(env, j_handler_,
                                                 webrtc::NativeToJavaString(env, stream_id),
                                                 Java_InternalEncodedVideoFrame_create(env,
                                                                                       encodedFrame.codec_type,
                                                                                       ScopedJavaLocalRef<jbyteArray>(
                                                                                               env,
                                                                                               byte_array),
                                                                                       encodedFrame.size,
                                                                                       encodedFrame.width,
                                                                                       encodedFrame.height,
                                                                                       encodedFrame.timestamp_ms,
                                                                                       encodedFrame.rotation,
                                                                                       encodedFrame.is_key_frame));
}

void RtcEventHandler::OnVideoFrame(const char *stream_id,
                                   vewtn::VideoFrame video_frame) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    int width = video_frame.width;
    int height = video_frame.height;
    int pixels = width * height;
    int uvPixels = (width * height) / 4;
    jbyteArray y_byte_array = env->NewByteArray(pixels);
    env->SetByteArrayRegion(y_byte_array, 0, pixels,
                            reinterpret_cast<const jbyte *>(video_frame.y_buffer));
    jbyteArray u_byte_array = env->NewByteArray(uvPixels);
    env->SetByteArrayRegion(u_byte_array, 0, uvPixels,
                            reinterpret_cast<const jbyte *>(video_frame.u_buffer));
    jbyteArray v_byte_array = env->NewByteArray(uvPixels);
    env->SetByteArrayRegion(v_byte_array, 0, uvPixels,
                            reinterpret_cast<const jbyte *>(video_frame.v_buffer));
    Java_RtcEventHandlerImpl_onVideoFrame(env, j_handler_,
                                          webrtc::NativeToJavaString(env, stream_id),
                                          Java_InternalVideoFrame_create(env,
                                                                         video_frame.type,
                                                                         video_frame.width,
                                                                         video_frame.height,
                                                                         ScopedJavaLocalRef<jbyteArray>(
                                                                                 env,
                                                                                 y_byte_array),
                                                                         ScopedJavaLocalRef<jbyteArray>(
                                                                                 env,
                                                                                 u_byte_array),
                                                                         ScopedJavaLocalRef<jbyteArray>(
                                                                                 env,
                                                                                 v_byte_array),
                                                                         video_frame.y_stride,
                                                                         video_frame.u_stride,
                                                                         video_frame.v_stride,
                                                                         video_frame.timestamp_ms,
                                                                         video_frame.rotation));
}

void RtcEventHandler::OnAudioFrame(const char *stream_id,
                                   vewtn::AudioFrame audio_frame) {
    JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(j_handler_);
    JNIEnv *env = AttachCurrentThreadIfNeeded();
    jbyteArray byte_array = env->NewByteArray(audio_frame.sample_length);
    env->SetByteArrayRegion(byte_array, 0, audio_frame.sample_length,
                            reinterpret_cast<const jbyte *>(audio_frame.data));
    Java_RtcEventHandlerImpl_onAudioFrame(env, j_handler_,
                                          webrtc::NativeToJavaString(env, stream_id),
                                          Java_InternalAudioFrame_create(env,
                                                                         audio_frame.type,
                                                                         ScopedJavaLocalRef<jbyteArray>(
                                                                                 env,
                                                                                 byte_array),
                                                                         audio_frame.sample_length,
                                                                         audio_frame.timestamp_ms,
                                                                         audio_frame.sample_rate,
                                                                         audio_frame.channels));
}

}
}
