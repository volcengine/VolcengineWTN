/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#import <Foundation/Foundation.h>
#import "VEWTNPublisher.h"
#import "VEWTNSubscriber.h"
#include "wtn_interface.h"

@interface VEWTNPublisher(internal)
- (void)setNativePublisher:(vewtn::IPublisher*)publisher;
- (vewtn::IPublisher*)getNativePublisher;
@end

@interface VEWTNSubscriber(internal)
- (void)setNativeSubscriber:(vewtn::ISubscriber*)subscriber;
- (vewtn::ISubscriber*)getNativeSubscriber;
@end
