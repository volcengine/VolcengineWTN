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
import android.os.HandlerThread;
import android.view.View;
import androidx.core.content.ContextCompat;
import com.vewtn.jni.NativeFunctions;
import com.vewtn.Subscriber;
import com.vewtn.SubscriberEventHandler;
import org.jetbrains.annotations.Nullable;
import org.webrtc.ContextUtils;
import org.webrtc.EglBase;
import org.webrtc.GlRectDrawer;
import org.webrtc.HardwareVideoDecoderFactory;
import org.webrtc.HardwareVideoEncoderFactory;
import org.webrtc.Logging;
import org.webrtc.SurfaceViewRenderer;
import org.webrtc.VideoDecoderFactory;
import org.webrtc.VideoEncoderFactory;

public class SubscriberImpl extends Subscriber {
    static {
        System.loadLibrary("veWTN");
    }

    public static final long NATIVE_POINTER_INVALID_VALUE = 0L;
    private final Context applicationContext;
    private long nativeSubscriberPtr = NATIVE_POINTER_INVALID_VALUE;
    private @Nullable
    EglBase eglBase;
    private @Nullable
    SurfaceViewRenderer remoteRenderer;
    private boolean isSubscribed = false;
    private SubscriberEventHandlerImpl eventHandlerImpl;
    private final HandlerThread thread;
    private final Handler handler;
    private @Nullable
    AudioManager audioManager;
    private int savedAudioMode = AudioManager.MODE_NORMAL;
    private boolean savedIsSpeakerOn;
    private VideoDecoderFactory hardwareVideoDecoderFactory;
    private VideoEncoderFactory hardwareVideoEncoderFactory;

    public SubscriberImpl(Context context, SubscriberEventHandler eventHandler) {
        this.applicationContext = context;
        ContextUtils.initialize(context);

        thread = new HandlerThread("SubscriberThread");
        thread.start();
        handler = new Handler(thread.getLooper());

        Logging.enableLogToDebugOutput(Logging.Severity.LS_VERBOSE);  // need load *.so

        eventHandlerImpl = new SubscriberEventHandlerImpl(eventHandler);
        eglBase = EglBase.create(null, EglBase.CONFIG_PLAIN);
        boolean enableIntelVp8Encoder = true;
        boolean enableH264HighProfile = false;
        this.hardwareVideoEncoderFactory =
                new HardwareVideoEncoderFactory(eglBase.getEglBaseContext(), enableIntelVp8Encoder, enableH264HighProfile);
        this.hardwareVideoDecoderFactory = new HardwareVideoDecoderFactory(eglBase.getEglBaseContext());
        nativeSubscriberPtr = NativeFunctions.nativeCreateSubscriber(context, eventHandlerImpl,
                this.hardwareVideoEncoderFactory, this.hardwareVideoDecoderFactory);
    }

    public int destroySubscriber() {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }

        handler.post(() -> {
            if (audioManager != null) {
                audioManager.setSpeakerphoneOn(savedIsSpeakerOn);
                audioManager.setMode(savedAudioMode);
                audioManager = null;
            }

            NativeFunctions.nativeDestroySubscriber(nativeSubscriberPtr);
            nativeSubscriberPtr = NATIVE_POINTER_INVALID_VALUE;

            remoteRenderer.release();
            eglBase.release();
            remoteRenderer = null;
            eglBase = null;
        });
        thread.quitSafely();
        return 0;
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

        remoteRenderer = (SurfaceViewRenderer) view;
        ((SurfaceViewRenderer) view).init(eglBase.getEglBaseContext(), null, EglBase.CONFIG_PLAIN,
                new GlRectDrawer());
        NativeFunctions.nativeSetRemoteCanvas(nativeSubscriberPtr, remoteRenderer);

        return 0;
    }

    @Override
    public int setEnableSpeaker(Boolean enable) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }

        handler.post(() -> {
            if (audioManager == null) {
                // default speaker
                audioManager = ((AudioManager) this.applicationContext.getSystemService(Context.AUDIO_SERVICE));
                savedAudioMode = audioManager.getMode();
                savedIsSpeakerOn = audioManager.isMicrophoneMute();
            }

            if ((audioManager.isSpeakerphoneOn() && enable)
                    || (!enable && !audioManager.isSpeakerphoneOn())) {
                return;
            }

            if (enable) {
                // speaker
                audioManager.setSpeakerphoneOn(true);
                audioManager.setMode(AudioManager.MODE_NORMAL);
            } else {
                // earphone
                audioManager.setSpeakerphoneOn(false);
                audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
            }
        });
        return 0;
    }

    @Override
    public int startSubscribe(String url) {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE || url.equals("")) {
            return -1;
        }
        if (checkRecordPermission() == false) {
            handler.post(() -> {
                eventHandlerImpl.onSubscribeStateChanged(1, 8);  // no device permission
            });
            return 0;
        }
        isSubscribed = true;
        handler.post(() -> {
            NativeFunctions.nativeStartSubscribe(nativeSubscriberPtr, url);
            setEnableSpeaker(true);
        });
        return 0;
    }

    @Override
    public int stopSubscribe() {
        if (nativeSubscriberPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        isSubscribed = false;
        handler.post(() -> {
            NativeFunctions.nativeStopSubscribe(nativeSubscriberPtr);
        });
        return 0;
    }

    @Override
    public boolean isSubscribed() {
        return isSubscribed;
    }

    private boolean checkRecordPermission() {
        String recordPermission = android.Manifest.permission.RECORD_AUDIO;
        int record_res = ContextCompat.checkSelfPermission(applicationContext, recordPermission);
        return (record_res == PackageManager.PERMISSION_GRANTED);
    }
}
