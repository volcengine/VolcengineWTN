/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "vewtn_interface.h"

@protocol SubscriberDelegate;
@class Subscriber;

class SubscriberEventHandler : public vewtn::ISubscriberEventHandler {
public:
    SubscriberEventHandler(Subscriber * Subscriber, id<SubscriberDelegate> delegate);
    ~SubscriberEventHandler();
    
    void OnSubscribeStateChanged(vewtn::SubscribeState state, vewtn::Reason reason) override;
    
private:
    __weak Subscriber * _subscriber;
    __weak id<SubscriberDelegate> _delegate;
};

