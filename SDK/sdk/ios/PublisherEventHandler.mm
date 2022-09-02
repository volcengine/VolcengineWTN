/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
 
#include "PublisherEventHandler.h"
#import "Publisher.h"

PublisherEventHandler::PublisherEventHandler(Publisher * publisher, id<PublisherDelegate> delegate) {
    _publisher = publisher;
    _delegate = delegate;
}

PublisherEventHandler::~PublisherEventHandler() {
    _delegate = nil;
    _publisher = nil;
}

void PublisherEventHandler::OnPublishStateChanged(vewtn::PublishState state, vewtn::Reason reason) {
    if ([_delegate respondsToSelector:@selector(publisher:onPublishStateChanged:reason:)]) {
        [_delegate publisher:_publisher onPublishStateChanged:(PublishState)state reason:(Reason)reason];
    }
}

