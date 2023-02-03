/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import com.vewtn.RtcDefines;

import org.webrtc.CalledByNative;

public class InternalAudioFrame {
    public RtcDefines.AudioFrameType type;
    public byte[] data;
    public int sampleLength;
    public long timestampMs;
    public int sampleRate;
    public int channels;

    public InternalAudioFrame(RtcDefines.AudioFrameType type, byte[] data, int sampleLength,
                              long timestampMs, int sampleRate, int channels) {
        this.type = type;
        this.data = data;
        this.sampleLength = sampleLength;
        this.timestampMs = timestampMs;
        this.sampleRate = sampleRate;
        this.channels = channels;
    }

    @CalledByNative
    private static InternalAudioFrame create(int type, byte[] data, int sampleLength,
                                             long timestampMs, int sampleRate, int channels) {
        return new InternalAudioFrame(RtcDefines.AudioFrameType.values()[type], data, sampleLength, timestampMs, sampleRate, channels);
    }
}