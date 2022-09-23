/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.publish;

import com.vewtn.PublisherEventHandler;

public class PublishEvent {
    public final PublisherEventHandler.PublishState state;
    public final PublisherEventHandler.Reason reason;

    public PublishEvent(PublisherEventHandler.PublishState state, PublisherEventHandler.Reason reason) {
        this.state = state;
        this.reason = reason;
    }

    public boolean isStartPublishSuccess() {
        return state == PublisherEventHandler.PublishState.StartPublishSuccess;
    }
}
