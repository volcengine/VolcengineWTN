/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#ifndef VEWTN_INTERFACE_H__
#define VEWTN_INTERFACE_H__

#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"
#include "api/media_stream_interface.h"
#include "rtc_base/thread.h"
#include "modules/audio_device/include/audio_device.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "api/video_codecs/video_decoder_factory.h"

#define VEWTN_API __attribute__((__visibility__("default"))) extern "C"

namespace vewtn {

// 发布状态
enum PublishState {
    StartPublishSuccess,  // 开始发布成功
    StartPublishFailed,   // 开始发布失败
    StopPublishSuccess,   // 停止发布成功
    StopPublishFailed,    // 停止发布失败
};

// 订阅状态
enum SubscribeState {
    StartSubscribeSuccess,  // 开始订阅成功
    StartSubscribeFailed,   // 开始订阅失败
    StopSubscribeSuccess,   // 停止订阅成功
    StopSubscribeFailed,    // 停止订阅失败
};

// 发布或订阅失败原因
enum Reason {
    Success,              // 成功
    ParameterError,       // 参数错误
    TokenError,           // token错误(包括token错误、token过期、token鉴权失败)
    ResourcesNotFound,    // 未找到资源
    ServerInternalError,  // 服务器内部错误
    SDKInternalError,     // SDK 内部错误(包括设置sdp失败)
    NetworkError,         // 网络错误
    UnknowError,          // 未知错误
};

class IPublisher {
public:
    virtual ~IPublisher() = default;
    virtual int StartPublish(const char* url) = 0;
    virtual int StopPublish() = 0;
    virtual bool IsPublishing() = 0;
    virtual void SetLocalCanvas(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& local_sink) = 0;
    virtual void SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) = 0;
    virtual rtc::Thread* GetSignalingThread() = 0;
    virtual rtc::Thread* GetWorkerThread() = 0;
};

class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual int StartSubscribe(const char* url) = 0;
    virtual int StopSubscribe() = 0;
    virtual bool IsSubscribed() = 0;
    virtual void SetRemoteCanvas(std::unique_ptr<rtc::VideoSinkInterface<webrtc::VideoFrame>>& local_sink) = 0;
    virtual void SetVideoSource(rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> video_source) = 0;
    virtual rtc::Thread* GetSignalingThread() = 0;
    virtual rtc::Thread* GetWorkerThread() = 0;
};

class IPublisherEventHandler {
public:
    // 发布状态
    virtual void OnPublishStateChanged(PublishState state,
                                       Reason reason) = 0;
};

class ISubscriberEventHandler {
public:
    // 发布状态
    virtual void OnSubscribeStateChanged(SubscribeState state,
                                         Reason reason) = 0;
};

VEWTN_API IPublisher* CreatePublisher(IPublisherEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                      std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                      std::unique_ptr<webrtc::VideoDecoderFactory> dec);
VEWTN_API void DestroyPublisher(IPublisher* publisherPtr);

VEWTN_API ISubscriber* CreateSubscriber(ISubscriberEventHandler* eventHandler, rtc::scoped_refptr<webrtc::AudioDeviceModule> audioDeviceModule,
                                        std::unique_ptr<webrtc::VideoEncoderFactory> enc,
                                        std::unique_ptr<webrtc::VideoDecoderFactory> dec);
VEWTN_API void DestroySubscriber(ISubscriber* subscriberPtr);

}

#endif
