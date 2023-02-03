/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#import "VEWTNEngine.h"

#import "VEWTNEventHandler.h"
#import "VEWTNPublisher.h"
#import "VEWTNSubscriber.h"
#import "VEWTNPubSubInternal.h"
#include "wtn_interface.h"

@interface VEWTNEngine()
@property (nonatomic, weak) id<VEWTNEngineDelegate> delegate;
@property (nonatomic, assign) VEWTNEventHandler * eventHandler;
@property (nonatomic, assign) vewtn::IRtcEngine * engine;


@property (nonatomic, strong) NSLock * pubSubsLock;
@property (nonatomic, strong) NSMutableArray * pubSubs;
@end

@implementation VEWTNEngine
- (instancetype)init {
    if (self = [super init]) {
        _engine = nullptr;
    }
    return self;
}

- (instancetype)initWithDelegate:(id<VEWTNEngineDelegate>)delegate {
    if (self = [super init]) {
        _eventHandler = new VEWTNEventHandler(delegate);
        _engine = vewtn::CreateRtcEngine(_eventHandler);
        if (_engine == nullptr) {
            delete _eventHandler;
            return nil;
        }
        _pubSubsLock = [[NSLock alloc] init];
        _pubSubs = @[].mutableCopy;
    }
    return self;
}

- (void)dealloc {
    [self _releaseNativeResource];
}

- (void)_releaseNativeResource {
    if (_engine != nullptr) {
        [_pubSubsLock lock];
        for (int i = 0; i < _pubSubs.count; i++) {
            if ([_pubSubs[i] isKindOfClass:[VEWTNPublisher class]]) {
                VEWTNPublisher * per = (VEWTNPublisher*)_pubSubs[i];
                if ([per getNativePublisher] != nullptr) {
                    _engine->DestroyPublisher([per getNativePublisher]);
                    [per setNativePublisher:nullptr];
                }
            } else if ([_pubSubs[i] isKindOfClass:[VEWTNSubscriber class]]) {
                VEWTNSubscriber * ser = (VEWTNSubscriber*)_pubSubs[i];
                if ([ser getNativeSubscriber] != nullptr) {
                    _engine->DestroySubscriber([ser getNativeSubscriber]);
                    [ser setNativeSubscriber:nullptr];
                }
            }
        }
        [_pubSubsLock unlock];
    }
    if (_engine != nullptr) {
        vewtn::DestroyRtcEngine(_engine);
    }
    if (_eventHandler != nullptr) {
        delete _eventHandler;
    }
}

- (VEWTNPublisher *)createPublisher:(VEWTNPubExtraParam*)pub_extra_param {
    if (_engine != nullptr) {
        vewtn::PubExtraParam param;
        param.pub_audio_source = pub_extra_param.pub_audio_source == VEWTNPubAudioSourceInternalRecord ? vewtn::PubAudioSource::InternalRecord : vewtn::PubAudioSource::PushExternalPCM;
        param.pub_video_source = pub_extra_param.pub_video_source == VEWTNPubVideoSourceInternalCamera ? vewtn::PubVideoSource::InternalCamera : pub_extra_param.pub_video_source == VEWTNPubVideoSourcePushExternalYUV ? vewtn::PubVideoSource::PushExternalYUV : vewtn::PubVideoSource::PushExternalH264;
        vewtn::IPublisher * np = _engine->CreatePublisher(param);
        if (np == nullptr) {
            return nil;
        }
        VEWTNPublisher * publisher = [[VEWTNPublisher alloc] init];
        [publisher setNativePublisher:np];
        [_pubSubsLock lock];
        [_pubSubs addObject:publisher];
        [_pubSubsLock unlock];
        return publisher;
    }
    return nil;
}

- (void)destroyPublisher:(VEWTNPublisher*)publisher {
    if (_engine != nullptr) {
        vewtn::IPublisher * np = [publisher getNativePublisher];
        if (np != nullptr) {
            _engine->DestroyPublisher(np);
            [publisher setNativePublisher:nullptr];
        }
        [_pubSubsLock lock];
        [_pubSubs removeObject:publisher];
        [_pubSubsLock unlock];
    }
}

- (VEWTNSubscriber *)createSubscriber:(VEWTNSubExtraParam*)sub_extra_param {
    if (_engine != nullptr) {
        vewtn::SubExtraParam param;
        param.sub_audio_source = sub_extra_param.sub_audio_source == VEWTNSubAudioSourceInternalPlay ? vewtn::SubAudioSource::InternalPlay : vewtn::SubAudioSource::CallbackPCM;
        param.sub_video_source = sub_extra_param.sub_video_source == VEWTNSubVideoSourceInternalRender ? vewtn::SubVideoSource::InternalRender : sub_extra_param.sub_video_source == VEWTNSubVideoSourceCallbackYUV ? vewtn::SubVideoSource::CallbackYUV : vewtn::SubVideoSource::CallbackH264;
        vewtn::ISubscriber * ns = _engine->CreateSubscriber(param);
        if (ns == nullptr) {
            return nil;
        }
        VEWTNSubscriber * subscriber = [[VEWTNSubscriber alloc] init];
        [subscriber setNativeSubscriber:ns];
        [_pubSubsLock lock];
        [_pubSubs addObject:subscriber];
        [_pubSubsLock unlock];
        return subscriber;
    }
}

- (void)destroySubscriber:(VEWTNSubscriber*)subscriber {
    if (_engine != nullptr) {
        vewtn::ISubscriber * ns = [subscriber getNativeSubscriber];
        if (ns != nullptr) {
            _engine->DestroySubscriber(ns);
            [subscriber setNativeSubscriber:nullptr];
        }
        [_pubSubsLock lock];
        [_pubSubs removeObject:subscriber];
        [_pubSubsLock unlock];
    }
}

@end



@implementation VEWTNPubExtraParam
@end

@implementation VEWTNSubExtraParam
@end

@implementation VEWTNEncodedVideoFrame
@end

@implementation VEWTNVideoFrame
@end

@implementation VEWTNAudioFrame
@end


@implementation VEWTNVideoConfig
@end

@implementation VEWTNStallInfo
@end


@implementation VEWTNRTCAudioStats
@end

@implementation VEWTNRTCVideoStats
@end

