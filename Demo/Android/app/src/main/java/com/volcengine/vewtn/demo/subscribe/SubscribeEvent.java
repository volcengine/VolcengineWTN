/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.subscribe;

import com.vewtn.SubscriberEventHandler;

public class SubscribeEvent {

    public final SubscriberEventHandler.SubscribeState state;
    public final SubscriberEventHandler.Reason reason;

    public SubscribeEvent(SubscriberEventHandler.SubscribeState state, SubscriberEventHandler.Reason reason) {
        this.state = state;
        this.reason = reason;
    }
}