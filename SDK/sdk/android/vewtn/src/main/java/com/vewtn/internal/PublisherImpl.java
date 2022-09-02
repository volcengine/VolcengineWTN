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
import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;
import com.vewtn.PublisherEventHandler;
import com.vewtn.jni.NativeFunctions;
import com.vewtn.Publisher;
import com.vewtn.VideoConfig;
import org.webrtc.Camera1Enumerator;
import org.webrtc.Camera2Enumerator;
import org.webrtc.CameraEnumerator;
import org.webrtc.CameraVideoCapturer;
import org.webrtc.EglBase;
import org.webrtc.HardwareVideoDecoderFactory;
import org.webrtc.HardwareVideoEncoderFactory;
import org.webrtc.Logging;
import org.webrtc.SurfaceTextureHelper;
import org.webrtc.SurfaceViewRenderer;
import org.webrtc.GlRectDrawer;
import org.webrtc.ContextUtils;
import org.webrtc.VideoDecoderFactory;
import org.webrtc.VideoEncoderFactory;

public class PublisherImpl extends Publisher {
    static {
        System.loadLibrary("veWTN");
    }

    public static final long NATIVE_POINTER_INVALID_VALUE = 0L;
    private final Context applicationContext;
    private long nativePublisherPtr = NATIVE_POINTER_INVALID_VALUE;
    private @Nullable
    EglBase eglBase;
    private @Nullable
    SurfaceViewRenderer localRenderer;
    private @Nullable
    SurfaceTextureHelper videoCapturerSurfaceTextureHelper;
    private @Nullable
    CameraVideoCapturer videoCapturer;
    private boolean isPublishing = false;
    private VideoConfig videoConfig;
    private PublisherEventHandlerImpl eventHandlerImpl;
    private final HandlerThread thread;
    private final Handler handler;
    private @Nullable
    AudioManager audioManager;
    private int savedAudioMode = AudioManager.MODE_NORMAL;
    private boolean savedIsMicrophoneMute;
    private boolean isStartAudioCapture = false;
    private VideoDecoderFactory hardwareVideoDecoderFactory;
    private VideoEncoderFactory hardwareVideoEncoderFactory;

    public PublisherImpl(Context context, PublisherEventHandler eventHandler) {
        this.applicationContext = context;
        ContextUtils.initialize(context);

        thread = new HandlerThread("PublisherThread");
        thread.start();
        handler = new Handler(thread.getLooper());

        Logging.enableLogToDebugOutput(Logging.Severity.LS_VERBOSE);  // need load *.so

        eglBase = EglBase.create(null /* sharedContext */, EglBase.CONFIG_PLAIN);
        eventHandlerImpl = new PublisherEventHandlerImpl(eventHandler);
        boolean enableIntelVp8Encoder = true;
        boolean enableH264HighProfile = false;
        this.hardwareVideoEncoderFactory =
                new HardwareVideoEncoderFactory(eglBase.getEglBaseContext(), enableIntelVp8Encoder, enableH264HighProfile);
        this.hardwareVideoDecoderFactory = new HardwareVideoDecoderFactory(eglBase.getEglBaseContext());
        nativePublisherPtr = NativeFunctions.nativeCreatePublisher(context, eventHandlerImpl,
                this.hardwareVideoEncoderFactory, this.hardwareVideoDecoderFactory);

        videoConfig = new VideoConfig();
    }

    public int destroyPublisher() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        handler.post(() -> {
            if (audioManager != null) {
                setMicrophoneMute(savedIsMicrophoneMute);
                audioManager.setMode(savedAudioMode);
                audioManager = null;
            }
            if (videoCapturer != null) {
                try {
                    videoCapturer.stopCapture();
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                videoCapturer.dispose();
                videoCapturer = null;
                videoCapturerSurfaceTextureHelper.dispose();
                videoCapturerSurfaceTextureHelper = null;
            }

            NativeFunctions.nativeDestroyPublisher(nativePublisherPtr);
            nativePublisherPtr = NATIVE_POINTER_INVALID_VALUE;

            if (localRenderer != null) {
                localRenderer.release();
                localRenderer = null;
            }
            if (eglBase != null) {
                eglBase.release();
                eglBase = null;
            }
        });
        thread.quitSafely();
        return 0;
    }

    @Override
    public int setVideoConfig(VideoConfig videoConfig) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        this.videoConfig = videoConfig;
        return 0;
    }

    @Override
    public int setLocalView(View view) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }

        if (localRenderer != null && localRenderer != view) {
            localRenderer.release();
            localRenderer = null;
        }

        if (view == null || localRenderer == view) {
            return 0;
        }

        localRenderer = (SurfaceViewRenderer) view;
        ((SurfaceViewRenderer) view).init(eglBase.getEglBaseContext(), null, EglBase.CONFIG_PLAIN,
                new GlRectDrawer());
        NativeFunctions.nativeSetLocalCanvas(nativePublisherPtr, localRenderer);

        return 0;
    }

    @Override
    public int startVideoCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        handler.post(() -> {
            if (videoCapturer == null) {
                videoCapturerSurfaceTextureHelper =
                        SurfaceTextureHelper.create("VideoCapturerThread", eglBase.getEglBaseContext());
                videoCapturer = createVideoCapturer(applicationContext);
                videoCapturer.initialize(videoCapturerSurfaceTextureHelper, applicationContext,
                        NativeFunctions.nativeGetJavaVideoCapturerObserver(nativePublisherPtr));
            }
            videoCapturer.startCapture(videoConfig.width, videoConfig.height, videoConfig.maxFPS);
        });
        return 0;
    }

    @Override
    public int stopVideoCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (videoCapturer == null) {
            return -2;
        }
        handler.post(() -> {
            try {
                videoCapturer.stopCapture();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            videoCapturer.dispose();
            videoCapturer = null;
            videoCapturerSurfaceTextureHelper.dispose();
            videoCapturerSurfaceTextureHelper = null;
        });
        return 0;
    }

    @Override
    public int switchCamera() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (videoCapturer == null) {
            return -2;
        }
        handler.post(() -> {
            videoCapturer.switchCamera(null);
        });
        return 0;
    }

    @Override
    public int startAudioCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (checkRecordPermission() == false) {
            return -2;  // no device permission
        }
        isStartAudioCapture = true;
        handler.post(() -> {
            setMicrophoneMute(false);
        });
        return 0;
    }

    @Override
    public int stopAudioCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (checkRecordPermission() == false) {
            return -2;  // no device permission
        }
        isStartAudioCapture = false;
        handler.post(() -> {
            setMicrophoneMute(true);
        });
        return 0;
    }

    private void setMicrophoneMute(boolean on) {
        if (audioManager == null) {
            audioManager = ((AudioManager) this.applicationContext.getSystemService(Context.AUDIO_SERVICE));
            savedAudioMode = audioManager.getMode();
            savedIsMicrophoneMute = audioManager.isMicrophoneMute();
            audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
        }
        boolean wasMuted = audioManager.isMicrophoneMute();
        if (wasMuted == on) {
            return;
        }
        audioManager.setMicrophoneMute(on);
    }

    @Override
    public int startPublish(String url) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        if (checkRecordPermission() == false) {
            handler.post(() -> {
                eventHandlerImpl.onPublishStateChanged(1, 8);  // no device permission
            });
            return 0;
        }

        isPublishing = true;
        handler.post(() -> {
            NativeFunctions.nativeStartPublish(nativePublisherPtr, url);
            if (!isStartAudioCapture) {
                stopAudioCapture();  // 默认关闭
            }
        });
        return 0;
    }

    @Override
    public int stopPublish() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        isPublishing = false;
        handler.post(() -> {
            NativeFunctions.nativeStopPublish(nativePublisherPtr);
        });
        return 0;
    }

    @Override
    public boolean isPublishing() {
        return isPublishing;
    }

    private CameraVideoCapturer createVideoCapturer(Context context) {
        CameraEnumerator enumerator = Camera2Enumerator.isSupported(context)
                ? new Camera2Enumerator(context)
                : new Camera1Enumerator();
        // default front camera
        return enumerator.createCapturer(enumerator.getDeviceNames()[1], null /* eventsHandler */);
    }

    private boolean checkRecordPermission() {
        String recordPermission = android.Manifest.permission.RECORD_AUDIO;
        int record_res = ContextCompat.checkSelfPermission(applicationContext, recordPermission);
        return (record_res == PackageManager.PERMISSION_GRANTED);
    }
}
