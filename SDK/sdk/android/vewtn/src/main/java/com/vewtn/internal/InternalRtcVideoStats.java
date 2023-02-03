/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import org.webrtc.CalledByNative;

public class InternalRtcVideoStats {
    public long bytesSent;
    public long bytesReceived;
    public double bytesSentPerSecond;
    public double bytesReceivedPerSecond;
    public long frameWidth;
    public long frameHeight;
    public long framesPerSecond;
    public long roundTripTimeMeasurements;
    public double totalRoundTripTime;
    public long packetsSent;
    public long packetsReceived;
    public long packetsLost;  // only pub
    public double franctionLoss;
    public double jitter;

    public InternalRtcVideoStats(long bytesSent,
                                 long bytesReceived,
                                 double bytesSentPerSecond,
                                 double bytesReceivedPerSecond,
                                 long frameWidth,
                                 long frameHeight,
                                 long framesPerSecond,
                                 long roundTripTimeMeasurements,
                                 double totalRoundTripTime,
                                 long packetsSent,
                                 long packetsReceived,
                                 long packetsLost,
                                 double franctionLoss,
                                 double jitter) {
        this.bytesSent = bytesSent;
        this.bytesReceived = bytesReceived;
        this.bytesSentPerSecond = bytesSentPerSecond;
        this.bytesReceivedPerSecond = bytesReceivedPerSecond;
        this.frameWidth = frameWidth;
        this.frameHeight = frameHeight;
        this.framesPerSecond = framesPerSecond;
        this.roundTripTimeMeasurements = roundTripTimeMeasurements;
        this.totalRoundTripTime = totalRoundTripTime;
        this.packetsSent = packetsSent;
        this.packetsReceived = packetsReceived;
        this.packetsLost = packetsLost;
        this.franctionLoss = franctionLoss;
        this.jitter = jitter;
    }

    @CalledByNative
    private static InternalRtcVideoStats create(long bytesSent,
                                                long bytesReceived,
                                                double bytesSentPerSecond,
                                                double bytesReceivedPerSecond,
                                                long frameWidth,
                                                long frameHeight,
                                                long framesPerSecond,
                                                long roundTripTimeMeasurements,
                                                double totalRoundTripTime,
                                                long packetsSent,
                                                long packetsReceived,
                                                long packetsLost,
                                                double franctionLoss,
                                                double jitter) {
        return new InternalRtcVideoStats(bytesSent, bytesReceived, bytesSentPerSecond, bytesReceivedPerSecond, frameWidth, frameHeight, framesPerSecond,
                roundTripTimeMeasurements, totalRoundTripTime, packetsSent, packetsReceived, packetsLost, franctionLoss, jitter);
    }
}
