/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import com.vewtn.RtcDefines;
import org.webrtc.CalledByNative;

public class InternalVideoFrame {
    public RtcDefines.VideoFrameType type;
    public int width;
    public int height;
    public byte[] yBuffer;
    public byte[] uBuffer;
    public byte[] vBuffer;
    public int yStride;
    public int uStride;
    public int vStride;
    public long timestampMs;
    public int rotation;

    public InternalVideoFrame(RtcDefines.VideoFrameType type, int width, int height,
                              byte[] yBuffer, byte[] uBuffer, byte[] vBuffer,
                              int yStride, int uStride, int vStride,
                              long timestampMs, int rotation) {
        this.type = type;
        this.width = width;
        this.height = height;
        this.yBuffer = yBuffer;
        this.uBuffer = uBuffer;
        this.vBuffer = vBuffer;
        this.yStride = yStride;
        this.uStride = uStride;
        this.vStride = vStride;
        this.timestampMs = timestampMs;
        this.rotation = rotation;
    }

    @CalledByNative
    private static InternalVideoFrame create(int type, int width, int height,
                                             byte[] yBuffer, byte[] uBuffer, byte[] vBuffer,
                                             int yStride, int uStride, int vStride,
                                             long timestampMs, int rotation) {
        return new InternalVideoFrame(RtcDefines.VideoFrameType.values()[type], width, height, yBuffer, uBuffer, vBuffer,
                yStride, uStride, vStride, timestampMs, rotation);
    }
}
