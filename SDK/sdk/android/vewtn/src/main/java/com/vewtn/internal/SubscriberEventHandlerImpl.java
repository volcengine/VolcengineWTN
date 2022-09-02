/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn.internal;
import org.webrtc.CalledByNative;
import com.vewtn.PublisherEventHandler;
import com.vewtn.SubscriberEventHandler;

public class SubscriberEventHandlerImpl {
    private SubscriberEventHandler eventHandlerExternal;

    public SubscriberEventHandlerImpl(SubscriberEventHandler eventHandlerExternal) {
        this.eventHandlerExternal = eventHandlerExternal;
    }

    @CalledByNative
    public void onSubscribeStateChanged(int state, int reason) {
        if (eventHandlerExternal != null) {
            eventHandlerExternal.onSubscribeStateChanged(getSubscribeState(state), getSubscribeStateReason(reason));
        }
    }

    private SubscriberEventHandler.SubscribeState getSubscribeState(int x) {
        switch(x) {
            case 0:
                return SubscriberEventHandler.SubscribeState.StartSubscribeSuccess;
            case 1:
                return SubscriberEventHandler.SubscribeState.StartSubscribeFailed;
            case 2:
                return SubscriberEventHandler.SubscribeState.StopSubscribeSuccess;
            case 3:
                return SubscriberEventHandler.SubscribeState.StopSubscribeFailed;
        }
        return null;
    }

    private SubscriberEventHandler.Reason getSubscribeStateReason(int x) {
        SubscriberEventHandler.Reason reason = SubscriberEventHandler.Reason.UnknowError;
        switch(x) {
            case 0:
                reason = SubscriberEventHandler.Reason.Success;
                break;
            case 1:
                reason = SubscriberEventHandler.Reason.ParameterError;
                break;
            case 2:
                reason = SubscriberEventHandler.Reason.TokenError;
                break;
            case 3:
                reason = SubscriberEventHandler.Reason.ResourcesNotFound;
                break;
            case 4:
                reason = SubscriberEventHandler.Reason.ServerInternalError;
                break;
            case 5:
                reason = SubscriberEventHandler.Reason.SDKInternalError;
                break;
            case 6:
                reason = SubscriberEventHandler.Reason.NetworkError;
                break;
            case 7:
                reason = SubscriberEventHandler.Reason.UnknowError;
                break;
            case 8:
                reason = SubscriberEventHandler.Reason.DeviceNoPermission;
                break;
            default:
                break;
        }
        return reason;
    }
}
