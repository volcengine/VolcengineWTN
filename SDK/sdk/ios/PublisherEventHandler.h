/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

#include "vewtn_interface.h"

@protocol PublisherDelegate;
@class Publisher;

class PublisherEventHandler : public vewtn::IPublisherEventHandler {
public:
    PublisherEventHandler(Publisher * publisher, id<PublisherDelegate> delegate);
    ~PublisherEventHandler();
    
    void OnPublishStateChanged(vewtn::PublishState state, vewtn::Reason reason) override;
    
private:
    __weak Publisher * _publisher;
    __weak id<PublisherDelegate> _delegate;
};

