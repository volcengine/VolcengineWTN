/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import com.vewtn.RtcDefines;

import org.webrtc.CalledByNative;

public class InternalEncodedVideoFrame {
    public RtcDefines.EncodedVideoFrameType codecType;
    public byte[] data;
    public int size;
    public int width;
    public int height;
    public long timestampMs;
    public int rotation;
    public boolean isKeyFrame;

    public InternalEncodedVideoFrame(RtcDefines.EncodedVideoFrameType codecType, byte[] data, int size,
                                     int width, int height, long timestampMs, int rotation, boolean isKeyFrame) {
        this.codecType = codecType;
        this.data = data;
        this.size = size;
        this.width = width;
        this.height = height;
        this.timestampMs = timestampMs;
        this.rotation = rotation;
        this.isKeyFrame = isKeyFrame;
    }

    @CalledByNative
    private static InternalEncodedVideoFrame create(int codecType, byte[] data, int size,
                                                    int width, int height, long timestampMs, int rotation, boolean isKeyFrame) {
        return new InternalEncodedVideoFrame(RtcDefines.EncodedVideoFrameType.values()[codecType], data, size, width, height, timestampMs, rotation, isKeyFrame);
    }
}
