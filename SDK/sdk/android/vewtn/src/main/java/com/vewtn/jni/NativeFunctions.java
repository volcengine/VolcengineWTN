/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.jni;

import android.content.Context;
import android.view.View;
import com.vewtn.internal.RtcEventHandlerImpl;

public class NativeFunctions {
    static {
        System.loadLibrary("veWTN");
    }

    public static native long nativeCreateRtcEngine(Context context, RtcEventHandlerImpl eventHandler);

    public static native void nativeDestroyRtcEngine(long nativePtr);

    // publisher
    public static native long nativeCreatePublisher(long nativePtr, int pubAudioSource, int pubVideoSource);

    public static native void nativeDestroyPublisher(long nativePtr, long nativePublisherPtr);

    public static native void nativeSetVideoConfig(long nativePublisherPtr, int width, int height, int maxFPS);

    public static native void nativeSetLocalView(long nativePublisherPtr, View localSink);

    public static native void nativeStartVideoCapture(long nativePublisherPtr);

    public static native void nativeStopVideoCapture(long nativePublisherPtr);

    public static native void nativeSwitchCamera(long nativePublisherPtr);

    public static native void nativeStartAudioCapture(long nativePublisherPtr);

    public static native void nativeStopAudioCapture(long nativePublisherPtr);

    public static native boolean nativeIsPublishing(long nativePublisherPtr);

    public static native int nativeStartPublish(long nativePublisherPtr, String url);

    public static native int nativeStopPublish(long nativePublisherPtr);

    public static native int nativeSetLocalAudioMute(long nativePublisherPtr, boolean mute);

    public static native int nativeSetLocalVideoMute(long nativePublisherPtr, boolean mute);

    public static native boolean nativePushEncodedVideoFrame(long nativePublisherPtr, int codec_type,
                                                             byte[] data, int size, int width, int height,
                                                             long timestamp_ms, int rotation, boolean is_key_frame);

    public static native boolean nativePushVideoFrame(long nativePublisherPtr, int type, int width, int height,
                                                      byte[] y_buffer, byte[] u_buffer, byte[] v_buffer,
                                                      int y_stride, int u_stride, int v_stride,
                                                      long timestamp_ms, int rotation);

    public static native boolean nativePushAudioFrame(long nativePublisherPtr, int type,
                                                      byte[] data, int sample_length, long timestamp_us,
                                                      int sample_rate, int channels);

    // subscriber
    public static native long nativeCreateSubscriber(long nativePtr, int subAudioSource, int subVideoSource);

    public static native void nativeDestroySubscriber(long nativePtr, long nativeSubscriberPtr);

    public static native void nativeSetRemoteView(long nativeSubscriberPtr, View remoteSink);

    public static native void nativeSetEnableSpeaker(long nativeSubscriberPtr, boolean enable);

    public static native boolean nativeIsSubscribed(long nativeSubscriberPtr);

    public static native int nativeStartSubscribe(long nativeSubscriberPtr, String url);

    public static native int nativeStopSubscribe(long nativeSubscriberPtr);

    public static native int nativeSetRemoteAudioMute(long nativeSubscriberPtr, boolean mute);

    public static native int nativeSetRemoteVideoMute(long nativeSubscriberPtr, boolean mute);
}
