/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import com.vewtn.EncodedVideoFrame;
import com.vewtn.AudioFrame;
import com.vewtn.RTCAudioStats;
import com.vewtn.RTCVideoStats;
import com.vewtn.RtcDefines;
import com.vewtn.RtcEventHandler;
import com.vewtn.StallInfo;
import com.vewtn.VideoFrame;

import org.webrtc.CalledByNative;

public class RtcEventHandlerImpl {
    private final RtcEventHandler eventHandlerExternal;

    public RtcEventHandlerImpl(RtcEventHandler eventHandlerExternal) {
        this.eventHandlerExternal = eventHandlerExternal;
    }

    @CalledByNative
    public void onPublishStateChanged(String streamId, int state, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onPublishStateChanged(streamId, RtcDefines.State.values()[state], RtcDefines.Reason.values()[reason]);
        }
    }

    @CalledByNative
    public void onSubscribeStateChanged(String streamId, int state, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onSubscribeStateChanged(streamId, RtcDefines.State.values()[state], RtcDefines.Reason.values()[reason]);
        }
    }

    @CalledByNative
    public void onLocalStats(String streamId, InternalRtcAudioStats internalRtcAudioStats, InternalRtcVideoStats internalRtcVideoStats) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onLocalStats(streamId, getRTCAudioStats(internalRtcAudioStats), getRTCVideoStats(internalRtcVideoStats));
        }
    }

    @CalledByNative
    public void onMuteAudioSuccess(String streamId) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onMuteAudioSuccess(streamId);
        }
    }

    @CalledByNative
    public void onMuteAudioFailed(String streamId, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onMuteAudioFailed(streamId, RtcDefines.Reason.values()[reason]);
        }
    }

    @CalledByNative
    public void onMuteVideoSuccess(String streamId) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onMuteVideoSuccess(streamId);
        }
    }

    @CalledByNative
    public void onMuteVideoFailed(String streamId, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onMuteVideoFailed(streamId, RtcDefines.Reason.values()[reason]);
        }
    }

    @CalledByNative
    public void onRemoteStats(String streamId, InternalRtcAudioStats internalRtcAudioStats, InternalRtcVideoStats internalRtcVideoStats) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onRemoteStats(streamId, getRTCAudioStats(internalRtcAudioStats), getRTCVideoStats(internalRtcVideoStats));
        }
    }

    RTCVideoStats getRTCVideoStats(InternalRtcVideoStats internalRtcVideoStats) {
        RTCVideoStats rtcVideoStats = new RTCVideoStats();
        rtcVideoStats.bytesSent = internalRtcVideoStats.bytesSent;
        rtcVideoStats.bytesReceived = internalRtcVideoStats.bytesReceived;
        rtcVideoStats.bytesSentPerSecond = internalRtcVideoStats.bytesSentPerSecond;
        rtcVideoStats.bytesReceivedPerSecond = internalRtcVideoStats.bytesReceivedPerSecond;
        rtcVideoStats.frameWidth = internalRtcVideoStats.frameWidth;
        rtcVideoStats.frameHeight = internalRtcVideoStats.frameHeight;
        rtcVideoStats.framesPerSecond = internalRtcVideoStats.framesPerSecond;
        rtcVideoStats.roundTripTimeMeasurements = internalRtcVideoStats.roundTripTimeMeasurements;
        rtcVideoStats.totalRoundTripTime = internalRtcVideoStats.totalRoundTripTime;
        rtcVideoStats.packetsSent = internalRtcVideoStats.packetsSent;
        rtcVideoStats.packetsReceived = internalRtcVideoStats.packetsReceived;
        rtcVideoStats.packetsLost = internalRtcVideoStats.packetsLost;
        rtcVideoStats.franctionLoss = internalRtcVideoStats.franctionLoss;
        rtcVideoStats.jitter = internalRtcVideoStats.jitter;
        return rtcVideoStats;
    }

    RTCAudioStats getRTCAudioStats(InternalRtcAudioStats internalRtcAudioStats) {
        RTCAudioStats rtcAudioStats = new RTCAudioStats();
        rtcAudioStats.bytesSent = internalRtcAudioStats.bytesSent;
        rtcAudioStats.bytesReceived = internalRtcAudioStats.bytesReceived;
        rtcAudioStats.bytesSentPerSecond = internalRtcAudioStats.bytesSentPerSecond;
        rtcAudioStats.bytesReceivedPerSecond = internalRtcAudioStats.bytesReceivedPerSecond;
        rtcAudioStats.audioLevel = internalRtcAudioStats.audioLevel;
        rtcAudioStats.totalAudioEnergy = internalRtcAudioStats.totalAudioEnergy;
        rtcAudioStats.roundTripTimeMeasurements = internalRtcAudioStats.roundTripTimeMeasurements;
        rtcAudioStats.totalRoundTripTime = internalRtcAudioStats.totalRoundTripTime;
        rtcAudioStats.packetsSent = internalRtcAudioStats.packetsSent;
        rtcAudioStats.packetsReceived = internalRtcAudioStats.packetsReceived;
        rtcAudioStats.packetsLost = internalRtcAudioStats.packetsLost;
        rtcAudioStats.franctionLoss = internalRtcAudioStats.franctionLoss;
        rtcAudioStats.jitter = internalRtcAudioStats.jitter;
        return rtcAudioStats;
    }

    @CalledByNative
    public void onFirstRemoteVideoFrameRendered(String streamId) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onFirstRemoteVideoFrameRendered(streamId);
        }
    }

    @CalledByNative
    public void onStallInfo(String streamId, InternalStallInfo internalStallInfo) {
        if (eventHandlerExternal != null) {
            StallInfo stallInfo = new StallInfo();
            stallInfo.stallCount500 = internalStallInfo.stallCount500;
            stallInfo.stallDuration500 = internalStallInfo.stallDuration500;
            stallInfo.stallCount200 = internalStallInfo.stallCount200;
            stallInfo.stallDuration200 = internalStallInfo.stallDuration200;
            stallInfo.frameRate = internalStallInfo.frameRate;
            eventHandlerExternal.onStallInfo(streamId, stallInfo);
        }
    }

    @CalledByNative
    public void onEncodedVideoFrame(String streamId, InternalEncodedVideoFrame internalEncodedVideoFrame) {
        if (eventHandlerExternal != null) {
            EncodedVideoFrame encodedFrame = new EncodedVideoFrame();
            encodedFrame.codecType = internalEncodedVideoFrame.codecType;
            encodedFrame.isKeyFrame = internalEncodedVideoFrame.isKeyFrame;
            encodedFrame.rotation = internalEncodedVideoFrame.rotation;
            encodedFrame.timestampMs = internalEncodedVideoFrame.timestampMs;
            encodedFrame.height = internalEncodedVideoFrame.height;
            encodedFrame.width = internalEncodedVideoFrame.width;
            encodedFrame.data = internalEncodedVideoFrame.data;
            encodedFrame.size = internalEncodedVideoFrame.size;
            eventHandlerExternal.onEncodedVideoFrame(streamId, encodedFrame);
        }
    }

    @CalledByNative
    public void onVideoFrame(String streamId, InternalVideoFrame internalVideoFrame) {
        if (eventHandlerExternal != null) {
            VideoFrame videoFrame = new VideoFrame();
            videoFrame.type = internalVideoFrame.type;
            videoFrame.yBuffer = internalVideoFrame.yBuffer;
            videoFrame.uBuffer = internalVideoFrame.uBuffer;
            videoFrame.vBuffer = internalVideoFrame.vBuffer;
            videoFrame.yStride = internalVideoFrame.yStride;
            videoFrame.uStride = internalVideoFrame.uStride;
            videoFrame.vStride = internalVideoFrame.vStride;
            videoFrame.rotation = internalVideoFrame.rotation;
            videoFrame.timestampMs = internalVideoFrame.timestampMs;
            videoFrame.height = internalVideoFrame.height;
            videoFrame.width = internalVideoFrame.width;
            eventHandlerExternal.onVideoFrame(streamId, videoFrame);
        }
    }

    @CalledByNative
    public void onAudioFrame(String streamId, InternalAudioFrame internalAudioFrame) {
        if (eventHandlerExternal != null) {
            AudioFrame audioFrame = new AudioFrame();
            audioFrame.type = internalAudioFrame.type;
            audioFrame.timestampMs = internalAudioFrame.timestampMs;
            audioFrame.sampleRate = internalAudioFrame.sampleRate;
            audioFrame.channels = internalAudioFrame.channels;
            audioFrame.data = internalAudioFrame.data;
            audioFrame.sampleLength = internalAudioFrame.sampleLength;
            eventHandlerExternal.onAudioFrame(streamId, audioFrame);
        }
    }
}