/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.internal;

import org.webrtc.CalledByNative;
import com.vewtn.PublisherEventHandler;

public class PublisherEventHandlerImpl {
    private PublisherEventHandler eventHandlerExternal;

    public PublisherEventHandlerImpl(PublisherEventHandler eventHandlerExternal) {
        this.eventHandlerExternal = eventHandlerExternal;
    }

    @CalledByNative
    public void onPublishStateChanged(int state, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onPublishStateChanged(getPublishState(state), getPublishStateReason(reason));
        }
    }

    private PublisherEventHandler.PublishState getPublishState(int x) {
        switch(x) {
            case 0:
                return PublisherEventHandler.PublishState.StartPublishSuccess;
            case 1:
                return PublisherEventHandler.PublishState.StartPublishFailed;
            case 2:
                return PublisherEventHandler.PublishState.StopPublishSuccess;
            case 3:
                return PublisherEventHandler.PublishState.StopPublishFailed;
        }
        return null;
    }

    private PublisherEventHandler.Reason getPublishStateReason(int x) {
        PublisherEventHandler.Reason reason = PublisherEventHandler.Reason.UnknowError;
        switch(x) {
            case 0:
                reason = PublisherEventHandler.Reason.Success;
                break;
            case 1:
                reason = PublisherEventHandler.Reason.ParameterError;
                break;
            case 2:
                reason = PublisherEventHandler.Reason.TokenError;
                break;
            case 3:
                reason = PublisherEventHandler.Reason.ResourcesNotFound;
                break;
            case 4:
                reason = PublisherEventHandler.Reason.ServerInternalError;
                break;
            case 5:
                reason = PublisherEventHandler.Reason.SDKInternalError;
                break;
            case 6:
                reason = PublisherEventHandler.Reason.NetworkError;
                break;
            case 7:
                reason = PublisherEventHandler.Reason.UnknowError;
                break;
            case 8:
                reason = PublisherEventHandler.Reason.DeviceNoPermission;
                break;
            default:
                break;
        }
        return reason;
    }
}
