/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.jni;
import android.content.Context;
import com.vewtn.internal.SubscriberEventHandlerImpl;
import com.vewtn.internal.PublisherEventHandlerImpl;
import org.webrtc.CapturerObserver;
import org.webrtc.VideoDecoderFactory;
import org.webrtc.VideoEncoderFactory;
import org.webrtc.VideoSink;
import org.webrtc.VideoDecoderFactory;
import org.webrtc.VideoEncoderFactory;

public class NativeFunctions {
    static {
        System.loadLibrary("veWTN");
    }

    // publisher
    public static native long nativeCreatePublisher(Context context, PublisherEventHandlerImpl eventHandler,
                                                    VideoEncoderFactory hardwareVideoEncoderFactory,
                                                    VideoDecoderFactory hardwareVideoDecoderFactory);

    public static native void nativeDestroyPublisher(long nativePublisherPtr);

    public static native void nativeSetLocalCanvas(long nativePublisherPtr, VideoSink localSink);

    public static native int nativeStartPublish(long nativePublisherPtr, String url);

    public static native int nativeStopPublish(long nativePublisherPtr);

    public static native CapturerObserver nativeGetJavaVideoCapturerObserver(long nativePublisherPtr);

    // subscriber
    public static native long nativeCreateSubscriber(Context context, SubscriberEventHandlerImpl eventHandler,
                                                     VideoEncoderFactory hardwareVideoEncoderFactory,
                                                     VideoDecoderFactory hardwareVideoDecoderFactory);

    public static native void nativeDestroySubscriber(long nativeSubscriberPtr);

    public static native void nativeSetRemoteCanvas(long nativeSubscriberPtr, VideoSink remoteSink);

    public static native int nativeStartSubscribe(long nativeSubscriberPtr, String url);

    public static native int nativeStopSubscribe(long nativeSubscriberPtr);
}
