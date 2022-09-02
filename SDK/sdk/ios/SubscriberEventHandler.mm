/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
 
#include "SubscriberEventHandler.h"
#import "Subscriber.h"

SubscriberEventHandler::SubscriberEventHandler(Subscriber * subscriber, id<SubscriberDelegate> delegate) {
    _subscriber = subscriber;
    _delegate = delegate;
}

SubscriberEventHandler::~SubscriberEventHandler() {
    _delegate = nil;
    _subscriber = nil;
}

void SubscriberEventHandler::OnSubscribeStateChanged(vewtn::SubscribeState state, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(subscriber:onSubscribeStateChanged:reason:)]) {
        [_delegate subscriber:_subscriber onSubscribeStateChanged:(SubscribeState)state reason:(Reason)reason];
    }
}
    
