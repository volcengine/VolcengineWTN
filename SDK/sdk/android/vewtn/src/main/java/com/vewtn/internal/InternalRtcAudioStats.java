/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import org.webrtc.CalledByNative;

public class InternalRtcAudioStats {
    public long bytesSent;
    public long bytesReceived;
    public double bytesSentPerSecond;
    public double bytesReceivedPerSecond;
    public double audioLevel;
    public double totalAudioEnergy;
    public long roundTripTimeMeasurements;
    public double totalRoundTripTime;
    public long packetsSent;
    public long packetsReceived;
    public long packetsLost;  // only pub
    public double franctionLoss;
    public double jitter;

    public InternalRtcAudioStats(long bytesSent,
                                 long bytesReceived,
                                 double bytesSentPerSecond,
                                 double bytesReceivedPerSecond,
                                 double audioLevel,
                                 double totalAudioEnergy,
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
        this.audioLevel = audioLevel;
        this.totalAudioEnergy = totalAudioEnergy;
        this.roundTripTimeMeasurements = roundTripTimeMeasurements;
        this.totalRoundTripTime = totalRoundTripTime;
        this.packetsSent = packetsSent;
        this.packetsReceived = packetsReceived;
        this.packetsLost = packetsLost;
        this.franctionLoss = franctionLoss;
        this.jitter = jitter;
    }

    @CalledByNative
    private static InternalRtcAudioStats create(long bytesSent,
                                                long bytesReceived,
                                                double bytesSentPerSecond,
                                                double bytesReceivedPerSecond,
                                                double audioLevel,
                                                double totalAudioEnergy,
                                                long roundTripTimeMeasurements,
                                                double totalRoundTripTime,
                                                long packetsSent,
                                                long packetsReceived,
                                                long packetsLost,
                                                double franctionLoss,
                                                double jitter) {
        return new InternalRtcAudioStats(bytesSent, bytesReceived, bytesSentPerSecond, bytesReceivedPerSecond, audioLevel, totalAudioEnergy,
                roundTripTimeMeasurements, totalRoundTripTime, packetsSent, packetsReceived, packetsLost, franctionLoss, jitter);
    }
}
