/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.internal;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;

import com.vewtn.EncodedVideoFrame;
import com.vewtn.AudioFrame;
import com.vewtn.VideoFrame;
import com.vewtn.jni.NativeFunctions;
import com.vewtn.Publisher;
import com.vewtn.VideoConfig;

import org.webrtc.Camera1Enumerator;
import org.webrtc.Camera2Enumerator;
import org.webrtc.CameraEnumerator;
import org.webrtc.CameraVideoCapturer;
import org.webrtc.ContextUtils;
import org.webrtc.EglBase;
import org.webrtc.GlRectDrawer;
import org.webrtc.SurfaceTextureHelper;
import org.webrtc.SurfaceViewRenderer;
import org.webrtc.VideoSource;

public class PublisherImpl implements Publisher {
    public static final long NATIVE_POINTER_INVALID_VALUE = 0L;
    private final Context applicationContext;
    private long nativePublisherPtr = NATIVE_POINTER_INVALID_VALUE;
    private static EglBase eglBase;
    private static @Nullable CameraVideoCapturer capturer = null;
    private static @Nullable SurfaceTextureHelper surfaceTextureHelper;
    private SurfaceViewRenderer localRenderer;

    public PublisherImpl(Context context, long nativePtr) {
        this.applicationContext = context;
        nativePublisherPtr = nativePtr;
        eglBase = EglBase.create(null /* sharedContext */, EglBase.CONFIG_PLAIN);
    }

    @Override
    public int setVideoConfig(VideoConfig videoConfig) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetVideoConfig(nativePublisherPtr, videoConfig.width, videoConfig.height, videoConfig.maxFPS);
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
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                //this runs on the UI thread
                localRenderer = (SurfaceViewRenderer) view;
                localRenderer.release();
                localRenderer.init(eglBase.getEglBaseContext(), null, EglBase.CONFIG_PLAIN,
                        new GlRectDrawer());
                NativeFunctions.nativeSetLocalView(nativePublisherPtr, localRenderer);
            }
        });
        return 0;
    }

    @Override
    public int startVideoCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStartVideoCapture(nativePublisherPtr);
        return 0;
    }

    @Override
    public int stopVideoCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStopVideoCapture(nativePublisherPtr);
        return 0;
    }

    @Override
    public int switchCamera() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSwitchCamera(nativePublisherPtr);
        return 0;
    }

    @Override
    public int startAudioCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStartAudioCapture(nativePublisherPtr);
        return 0;
    }

    @Override
    public int stopAudioCapture() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStopAudioCapture(nativePublisherPtr);
        return 0;
    }

    @Override
    public int startPublish(String url) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStartPublish(nativePublisherPtr, url);
        return 0;
    }

    @Override
    public int stopPublish() {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeStopPublish(nativePublisherPtr);
        return 0;
    }

    @Override
    public boolean isPublishing() {
        return NativeFunctions.nativeIsPublishing(nativePublisherPtr);
    }

    @Override
    public int setLocalAudioMute(boolean mute) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetLocalAudioMute(nativePublisherPtr, mute);
        return 0;
    }

    @Override
    public int setLocalVideoMute(boolean mute) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return -1;
        }
        NativeFunctions.nativeSetLocalVideoMute(nativePublisherPtr, mute);
        return 0;
    }

    @Override
    public boolean pushEncodedVideoFrame(EncodedVideoFrame encodedFrame) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return false;
        }
        return NativeFunctions.nativePushEncodedVideoFrame(nativePublisherPtr, encodedFrame.codecType.ordinal(),
                encodedFrame.data, encodedFrame.size, encodedFrame.width, encodedFrame.height,
                encodedFrame.timestampMs, encodedFrame.rotation, encodedFrame.isKeyFrame);
    }

    @Override
    public boolean pushVideoFrame(VideoFrame videoFrame) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return false;
        }
        // android hardware encoder need
        if (videoFrame.width % 16 != 0 || videoFrame.height % 16 != 0) {
            return false;
        }
        return NativeFunctions.nativePushVideoFrame(nativePublisherPtr, videoFrame.type.ordinal(), videoFrame.width, videoFrame.height,
                videoFrame.yBuffer, videoFrame.uBuffer, videoFrame.vBuffer,
                videoFrame.yStride, videoFrame.uStride, videoFrame.vStride,
                videoFrame.timestampMs, videoFrame.rotation);
    }

    @Override
    public boolean pushAudioFrame(AudioFrame audioFrame) {
        if (nativePublisherPtr == NATIVE_POINTER_INVALID_VALUE) {
            return false;
        }
        return NativeFunctions.nativePushAudioFrame(nativePublisherPtr, audioFrame.type.ordinal(),
                audioFrame.data, audioFrame.sampleLength, audioFrame.timestampMs,
                audioFrame.sampleRate, audioFrame.channels);
    }

    public static boolean checkRecordPermission() {
        String recordPermission = android.Manifest.permission.RECORD_AUDIO;
        int record_res = ContextCompat.checkSelfPermission(ContextUtils.getApplicationContext(), recordPermission);
        return (record_res == PackageManager.PERMISSION_GRANTED);
    }

    public static boolean checkCameraPermission() {
        String cameraPermission = Manifest.permission.CAMERA;
        int camera_res = ContextCompat.checkSelfPermission(ContextUtils.getApplicationContext(), cameraPermission);
        return (camera_res == PackageManager.PERMISSION_GRANTED);
    }

    private static @Nullable
    CameraVideoCapturer createVideoCapturer(Context context, boolean isFront) {
        CameraEnumerator enumerator = Camera2Enumerator.isSupported(context)
                ? new Camera2Enumerator(context)
                : new Camera1Enumerator();
        // default front camera
        int index = isFront ? 1 : 0;
        return enumerator.createCapturer(enumerator.getDeviceNames()[index], null /* eventsHandler */);
    }

    public static void StartCamera(long nativeTrackSource, boolean isFront, int width, int height, int fps) {
        if (capturer == null) {
            surfaceTextureHelper =
                    SurfaceTextureHelper.create("AndroidVideoCapturerThread", eglBase.getEglBaseContext());
            capturer = createVideoCapturer(ContextUtils.getApplicationContext(), isFront);
            VideoSource videoSource = new VideoSource(nativeTrackSource);
            capturer.initialize(surfaceTextureHelper, ContextUtils.getApplicationContext(),
                    videoSource.getCapturerObserver());
        }
        capturer.startCapture(width, height, fps);
    }

    public static void StopCamera() throws InterruptedException {
        if (capturer != null) {
            capturer.stopCapture();
            capturer.dispose();
            capturer = null;
            surfaceTextureHelper.dispose();
            surfaceTextureHelper = null;
        }
    }

    public static void SwitchCamera() throws InterruptedException {
        if (capturer != null) {
            capturer.switchCamera(null);
        }
    }
}
