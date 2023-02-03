/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
package com.vewtn.internal;

import org.webrtc.CalledByNative;

public class InternalStallInfo {
    public int stallCount500;
    public int stallDuration500;
    public int stallCount200;
    public int stallDuration200;
    public int frameRate;

    public InternalStallInfo(int stallCount500, int stallDuration500,
                             int stallCount200, int stallDuration200, int frameRate) {
        this.stallCount500 = stallCount500;
        this.stallDuration500 = stallDuration500;
        this.stallCount200 = stallCount200;
        this.stallDuration200 = stallDuration200;
        this.frameRate = frameRate;
    }

    @CalledByNative
    private static InternalStallInfo create(int stallCount500, int stallDuration500,
                                            int stallCount200, int stallDuration200, int frameRate) {
        return new InternalStallInfo(stallCount500, stallDuration500, stallCount200, stallDuration200, frameRate);
    }
}
