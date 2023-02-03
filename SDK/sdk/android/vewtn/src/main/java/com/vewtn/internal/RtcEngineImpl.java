/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import android.content.Context;

import androidx.annotation.Nullable;

import com.vewtn.PubExtraParam;
import com.vewtn.Publisher;
import com.vewtn.RtcEngine;
import com.vewtn.RtcEventHandler;
import com.vewtn.SubExtraParam;
import com.vewtn.Subscriber;
import com.vewtn.jni.NativeFunctions;

import org.webrtc.ContextUtils;
import org.webrtc.Logging;

import java.util.HashMap;

public class RtcEngineImpl extends RtcEngine {
    public static final long NATIVE_POINTER_INVALID_VALUE = 0L;
    private RtcEventHandlerImpl eventHandlerImpl;
    private final Context applicationContext;
    private long nativePtr = NATIVE_POINTER_INVALID_VALUE;
    HashMap<Publisher, Long> publisherPtrMaps;
    HashMap<Subscriber, Long> subscriberPtrMaps;

    public RtcEngineImpl(Context context, RtcEventHandler eventHandler) {
        this.applicationContext = context;
        ContextUtils.initialize(context);
        eventHandlerImpl = new RtcEventHandlerImpl(eventHandler);
        publisherPtrMaps = new HashMap<Publisher, Long>();
        subscriberPtrMaps = new HashMap<Subscriber, Long>();
        nativePtr = NativeFunctions.nativeCreateRtcEngine(this.applicationContext, eventHandlerImpl);
    }

    public int destroy() {
        if (nativePtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeDestroyRtcEngine(nativePtr);
        nativePtr = NATIVE_POINTER_INVALID_VALUE;
        publisherPtrMaps.clear();
        subscriberPtrMaps.clear();
        return 0;
    }

    @Override
    public Publisher createPublisher(PubExtraParam pubExtraParam) {
        if (nativePtr == NATIVE_POINTER_INVALID_VALUE) {
            return null;
        }
        long nativePublisherPtr = NativeFunctions.nativeCreatePublisher(nativePtr,
                pubExtraParam.pubAudioSource.ordinal(), pubExtraParam.pubVideoSource.ordinal());
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return null;
        }

        PublisherImpl publisher = new PublisherImpl(applicationContext, nativePublisherPtr);
        publisherPtrMaps.put(publisher, nativePublisherPtr);
        return publisher;
    }

    @Override
    public void destroyPublisher(Publisher publisher) {
        if (nativePtr == NATIVE_POINTER_INVALID_VALUE ||
            publisher == null) {
            return;
        }
        long nativePublisherPtr = publisherPtrMaps.get(publisher).longValue();
        NativeFunctions.nativeDestroyPublisher(nativePtr, nativePublisherPtr);
    }

    @Override
    public Subscriber createSubscriber(SubExtraParam subExtraParam) {
        if (nativePtr == NATIVE_POINTER_INVALID_VALUE) {
            return null;
        }
        long nativeSubscriberPtr = NativeFunctions.nativeCreateSubscriber(nativePtr,
                subExtraParam.subAudioSource.ordinal(), subExtraParam.subVideoSource.ordinal());
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return null;
        }
        SubscriberImpl subscriber = new SubscriberImpl(applicationContext, nativeSubscriberPtr);
        subscriberPtrMaps.put(subscriber, nativeSubscriberPtr);
        return subscriber;
    }

    @Override
    public void destroySubscriber(Subscriber subscriber) {
        if (nativePtr == NATIVE_POINTER_INVALID_VALUE ||
            subscriber == null) {
            return;
        }
        long nativeSubscriberPtr = subscriberPtrMaps.get(subscriber).longValue();
        NativeFunctions.nativeDestroySubscriber(nativePtr, nativeSubscriberPtr);
    }
}