/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.internal;

import android.content.Context;
import android.content.pm.PackageManager;
import android.media.AudioManager;
import android.os.Handler;
import android.os.Looper;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;

import com.vewtn.jni.NativeFunctions;
import com.vewtn.Subscriber;

import org.webrtc.ContextUtils;
import org.webrtc.EglBase;
import org.webrtc.GlRectDrawer;
import org.webrtc.SurfaceViewRenderer;

public class SubscriberImpl implements Subscriber {
    public static final long NATIVE_POINTER_INVALID_VALUE = 0L;
    private final Context applicationContext;
    private long nativeSubscriberPtr = NATIVE_POINTER_INVALID_VALUE;
    private SurfaceViewRenderer remoteRenderer;
    private EglBase eglBase;
    private static @Nullable
    AudioManager audioManager;

    public SubscriberImpl(Context context, long nativePtr) {
        this.applicationContext = context;
        nativeSubscriberPtr = nativePtr;
        eglBase = EglBase.create(null, EglBase.CONFIG_PLAIN);
    }

    @Override
    public int setRemoteView(View view) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (remoteRenderer != null && remoteRenderer != view) {
            remoteRenderer.release();
            remoteRenderer = null;
        }
        if (view == null || remoteRenderer == view) {
            return 0;
        }
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                //this runs on the UI thread
                remoteRenderer = (SurfaceViewRenderer) view;
                remoteRenderer.release();
                remoteRenderer.init(eglBase.getEglBaseContext(), null, EglBase.CONFIG_PLAIN,
                        new GlRectDrawer());
                NativeFunctions.nativeSetRemoteView(nativeSubscriberPtr, remoteRenderer);
            }
        });
        return 0;
    }

    @Override
    public int setEnableSpeaker(Boolean enable) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetEnableSpeaker(nativeSubscriberPtr, enable);
        return 0;
    }

    @Override
    public int startSubscribe(String url) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE || url.equals("")) {
            return -1;
        }
        NativeFunctions.nativeStartSubscribe(nativeSubscriberPtr, url);
        return 0;
    }

    @Override
    public int stopSubscribe() {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStopSubscribe(nativeSubscriberPtr);
        return 0;
    }

    @Override
    public boolean isSubscribed() {
        return NativeFunctions.nativeIsSubscribed(nativeSubscriberPtr);
    }

    @Override
    public int setRemoteAudioMute(boolean mute) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetRemoteAudioMute(nativeSubscriberPtr, mute);
        return 0;
    }

    @Override
    public int setRemoteVideoMute(boolean mute) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetRemoteVideoMute(nativeSubscriberPtr, mute);
        return 0;
    }

    public static void SetSpeakerphoneOn(boolean enable) {
        if (audioManager == null) {
            // default speaker
            audioManager = ((AudioManager) ContextUtils.getApplicationContext().getSystemService(Context.AUDIO_SERVICE));
//            savedAudioMode = audioManager.getMode();
//            savedIsSpeakerOn = audioManager.isMicrophoneMute();
        }

        if ((audioManager.isSpeakerphoneOn() && enable)
                || (!enable && !audioManager.isSpeakerphoneOn())) {
            return;
        }

        if (enable) {
            // speaker
            audioManager.setMode(AudioManager.MODE_NORMAL);
            audioManager.setSpeakerphoneOn(true);
        } else {
            // earphone
            audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);  // for change volume
            audioManager.setSpeakerphoneOn(false);
        }
    }
}
