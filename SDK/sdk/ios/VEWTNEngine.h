/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#import <Foundation/Foundation.h>

#import "VEWTNDefines.h"

NS_ASSUME_NONNULL_BEGIN

@class VEWTNPublisher;
@class VEWTNSubscriber;

/**
 * {zh}
 * @type callback
 * @brief 事件回调接口
 */
/** {en}
 * @type callback
 * @brief Event handler for engine (include pub & sub)
 */
@protocol VEWTNEngineDelegate <NSObject>
/** {zh}
 * @type callback
 * @brief 音视频流的发布状态发生改变时，收到此回调。
 * @param stream_id 流 ID
 * @param state 当前发布状态，参看 VEWTNState{@link #VEWTNState}。
 * @param reason 发布状态发生改变的原因，参看 VEWTNReason{@link #VEWTNReason}。
 */
/** {en}
 * @type callback
 * @brief Callback of media stream publishing state change.
 * @param stream_id stream ID
 * @param state The current state. See VEWTNState{@link #VEWTNState}.
 * @param reason Error code for the change. See VEWTNReason{@link #VEWTNReason}.
 */
- (void)onPublishStateChanged:(NSString*)stream_id publishState:(VEWTNState)state reason:(VEWTNReason)reason;
/** {zh}
 * @type callback
 * @brief 发布流状态的周期回调，周期1秒。
 * @param stream_id 流 ID
 * @param audio_stats 音频流数据，参看 VEWTNRTCAudioStats{@link #VEWTNRTCAudioStats}。
 * @param video_stats 视频流数据，参看 VEWTNRTCVideoStats{@link #VEWTNRTCVideoStats}。
 */
/** {en}
 * @type callback
 * @brief Periodic callback of the stats of locally published stream in every second.
 * @param stream_id Stream ID
 * @param audio_stats Stats of audio stream. See VEWTNRTCAudioStats{@link #VEWTNRTCAudioStats}.
 * @param video_stats Stats of video stream. See VEWTNRTCVideoStats{@link #VEWTNRTCVideoStats}.
 */
- (void)onLocalStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
/** {zh}
 * @type callback
 * @brief Mute 音频成功回调。
 * @param stream_id 流 ID
 */
/** {en}
 * @type callback
 * @brief Success to mute audio callback.
 * @param stream_id Stream ID
 */
- (void)onMuteAudioSuccess:(NSString*)stream_id;
/** {zh}
 * @type callback
 * @brief Mute 音频失败回调。
 * @param stream_id 流 ID
 * @param reason 参看 VEWTNReason{@link #VEWTNReason}。
 */
/** {en}
 * @type callback
 * @brief Failure to mute audio callback.
 * @param stream_id Stream ID
 * @param reason See VEWTNReason{@link #VEWTNReason}.
 */
- (void)onMuteAudioFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
/** {zh}
 * @type callback
 * @brief Mute 视频成功回调。
 * @param stream_id 流 ID
 */
/** {en}
 * @type callback
 * @brief Success to mute video callback.
 * @param stream_id Stream ID
 */
- (void)onMuteVideoSuccess:(NSString*)stream_id;
/** {zh}
 * @type callback
 * @brief Mute 视频失败回调。
 * @param stream_id 流 ID
 * @param reason 参看 VEWTNReason{@link #VEWTNReason}。
 */
/** {en}
 * @type callback
 * @brief Failure to mute video callback.
 * @param stream_id Stream ID
 * @param reason See VEWTNReason{@link #VEWTNReason}.
 */
- (void)onMuteVideoFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
/** {zh}
 * @type callback
 * @brief 音视频流的订阅状态发生改变时，收到此回调。
 * @param stream_id 流 ID
 * @param state 当前订阅状态，参看 VEWTNState{@link #VEWTNState}。
 * @param reason 订阅状态发生改变的原因。参看 VEWTNReason{@link #VEWTNReason}。
 */
/** {en}
 * @type callback
 * @brief Callback of media stream subscribing state change.
 * @param stream_id stream ID
 * @param state The current subscribing state. See VEWTNState{@link #VEWTNState}.
 * @param reason Error code when subscribing state changes. See VEWTNReason{@link #VEWTNReason}.
 */
- (void)onSubscribeStateChanged:(NSString*)stream_id subscribeState:(VEWTNState)state reason:(VEWTNReason)reason;
/** {zh}
 * @type callback
 * @brief 订阅流状态的周期回调，周期1秒。
 * @param stream_id 流 ID
 * @param audio_stats 音频流数据，参看 VEWTNRTCAudioStats{@link #VEWTNRTCAudioStats}。
 * @param video_stats 视频流数据，参看 VEWTNRTCVideoStats{@link #VEWTNRTCVideoStats}。
 */
/** {en}
 * @type callback
 * @brief Periodic callback of the stats of the subscribed remote stream in every second.
 * @param stream_id Stream ID
 * @param audio_stats Audio stream information. See VEWTNRTCAudioStats{@link #VEWTNRTCAudioStats}.
 * @param video_stats Video stream information. See VEWTNRTCVideoStats{@link #VEWTNRTCVideoStats}.
 */
- (void)onRemoteStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
/** {zh}
 * @type callback
 * @brief 订阅的远端视频流的首帧完成渲染时，收到此回调。
 * @param stream_id 流 ID
 */
/** {en}
 * @type callback
 * @brief Receive the callback when the first frame of the subscribed remote frame is rendered.
 * @param stream_id Stream ID
 */
- (void)onFirstRemoteVideoFrameRendered:(NSString*)stream_id;
/** {zh}
 * @type callback
 * @brief 卡顿信息回调，周期1秒。
 * @param stream_id 流 ID
 * @param stall_info 卡顿信息，参看 VEWTNStallInfo{@link #VEWTNStallInfo}。
 */
/** {en}
 * @type callback
 * @brief Periodic callback of the stall information in every second.
 * @param stream_id Stream ID
 * @param stall_info The stall information. See VEWTNStallInfo{@link #VEWTNStallInfo}.
 */
- (void)onStallInfo:(NSString*)stream_id stallInfo:(VEWTNStallInfo*)stall_info;
/** {zh}
 * @type callback
 * @brief 编码视频帧周期回调。周期是帧率的倒数。
 * @param stream_id 流 ID
 * @param encoded_frame 视频帧信息，参看 VEWTNEncodedVideoFrame{@link #VEWTNEncodedVideoFrame}。
 */
/** {en}
 * @type callback
 * @brief Periodic encoded video frame callback. The period is the reciprocal of the framerate.
 * @param stream_id Stream ID
 * @param encoded_frame Video frame information, see VEWTNEncodedVideoFrame{@link #VEWTNEncodedVideoFrame}.
 */
- (void)onEncodedVideoFrame:(NSString*)stream_id frame:(VEWTNEncodedVideoFrame*)encoded_frame;
/** {zh}
 * @type callback
 * @brief 编码视频帧周期回调。周期是帧率的倒数。
 * @param stream_id 流 ID
 * @param video_frame 视频帧信息，参看 VEWTNVideoFrame{@link #VEWTNVideoFrame}。
 */
/** {en}
 * @type callback
 * @brief Periodic encoded video frame callback. The period is the reciprocal of the framerate.
 * @param stream_id Stream ID
 * @param video_frame Video frame information, see VEWTNVideoFrame{@link #VEWTNVideoFrame}.
 */
- (void)onVideoFrame:(NSString*)stream_id frame:(VEWTNVideoFrame*)video_frame;
/** {zh}
 * @type callback
 * @brief 音频帧回调。周期是 10ms。
 * @param stream_id 流 ID
 * @param audio_frame 音频帧信息，参看 VEWTNAudioFrame{@link #VEWTNAudioFrame}。
 */
/** {en}
 * @type callback
 * @brief Periodic audio frame callback in every 10ms.
 * @param stream_id Stream ID
 * @param audio_frame Audio frame information, see VEWTNAudioFrame{@link #VEWTNAudioFrame}.
 */
- (void)onAudioFrame:(NSString*)stream_id frame:(VEWTNAudioFrame*)audio_frame;
@end

/**
 * @type api
 * @brief VEWTNEngine Class
 */
VEWTN_IOS_EXPORT @interface VEWTNEngine : NSObject
/** {zh}
 * @type api
 * @brief 创建引擎实例
 * @param delegate 参看 VEWTNEngineDelegate{@link #VEWTNEngineDelegate}。
 * @return 创建成功时。返回一个可用的引擎实例
 * @notes 至多只能创建一个引擎。
 */
/** {en}
 * @type api
 * @brief Create the engine instance.
 * @param delegate See VEWTNEngineDelegate{@link #VEWTNEngineDelegate}.
 * @return A successfully created engine instance.
 * @notes You can create no more than one engine instance.
 */
- (instancetype)initWithDelegate:(id<VEWTNEngineDelegate>)delegate;
/** {zh}
 * @type api
 * @brief 创建 Publisher 实例。
 * @param pub_extra_param 参看 VEWTNPubExtraParam{@link #VEWTNPubExtraParam}。
 * @return  <br>
 *        + Publisher：创建成功。返回一个可用的 VEWTNPublisher{@link #VEWTNPublisher} 实例  <br>
 *        + Null：创建失败。可能原因有：已经有一个内部源 Publisher。
 * @notes 在发布时，你可以创建任意个外部源，但至多只能创建一个内部源。
 */
/** {en}
 * @type api
 * @brief Create the Publisher instance.
 * @param pub_extra_param See VEWTNPubExtraParam{@link #VEWTNPubExtraParam}.
 * @return  <br>
 *         + Publisher: A successfully created VEWTNPublisher{@link #VEWTNPublisher} instance.  <br>
 *         + Null: Failed to create. Possible reasons are: Creating an internal source Publisher when an internal one exists.
 * @notes You may create multiple Publisher with external sources, but no more than one Publisher with internal source.
 */
- (VEWTNPublisher *)createPublisher:(VEWTNPubExtraParam*)pub_extra_param;
/** {zh}
 * @type api
 * @brief 销毁由 createPublisher:{@link #VEWTNEngine#createPublisher:} 所创建的 Publisher 实例，并释放所有相关资源
 * @notes  <br>
 *      + 请确保和需要销毁的 VEWTNPublisher{@link #VEWTNPublisher} 实例相关的业务场景全部结束后，才调用此方法  <br>
 *      + 该方法在调用之后，会销毁所有和此 VEWTNPublisher{@link #VEWTNPublisher} 实例相关的内存，并且停止与媒体服务器的任何交互  <br>
 */
/** {en}
 * @type api
 * @brief Destroy the Publisher instance created by createPublisher:{@link #VEWTNEngine#createPublisher:}, and release all related resources. <br>
 * @notes  <br>
 *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
 *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
 */
- (void)destroyPublisher:(VEWTNPublisher*)publisher;
/** {zh}
 * @type api
 * @brief 创建 VEWTNSubscriber 实例。
 * @param sub_extra_param 参看 VEWTNSubExtraParam{@link #VEWTNSubExtraParam}。
 * @return  <br>
 *        + Subscriber: 创建成功。返回一个可用的 VEWTNSubscriber{@link #VEWTNSubscriber} 实例  <br>
 *        + Null: 创建失败。可能原因有：已经有一个内部源 Subscriber。
 * @notes 在发布时，你可以创建任意个外部源，但至多只能创建一个内部源。
 */
/** {en}
 * @type api
 * @brief Create the Subscriber instance.
 * @param sub_extra_param See VEWTNSubExtraParam{@link #VEWTNSubExtraParam}.
 * @return  <br>
 *         + Subscriber: A successfully created VEWTNSubscriber{@link #VEWTNSubscriber} instance.  <br>
 *         + Null: Failed to create. Possible reasons are: Creating an internal source Subscriber when an internal one exists.
 * @notes You may create multiple Subscriber with external sources, but no more than one Subscriber with internal source.
 */
- (VEWTNSubscriber *)createSubscriber:(VEWTNSubExtraParam*)sub_extra_param;
/** {zh}
 * @type api
 * @brief 销毁由 createSubscriber:{@link #VEWTNEngine#createSubscriber:} 所创建的 VEWTNSubscriber 实例，并释放所有相关资源。
 * @notes  <br>
 *      + 请确保和需要销毁的 VEWTNSubscriber{@link #VEWTNSubscriber} 实例相关的业务场景全部结束后，才调用此方法  <br>
 *      + 该方法在调用之后，会销毁所有和此 VEWTNSubscriber{@link #VEWTNSubscriber} 实例相关的内存，并且停止与媒体服务器的任何交互  <br>
 */
/** {en}
 * @type api
 * @brief Destroy the Subscriber instance created by createSubscriber:{@link #VEWTNEngine#createSubscriber:}, and release all related resources. <br>
 * @notes  <br>
 *       + Call this API after all business scenarios related to the VEWTNSubscriber{@link #VEWTNSubscriber} instance are destroyed.  <br>
 *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
 */
- (void)destroySubscriber:(VEWTNSubscriber*)subscriber;
@end

NS_ASSUME_NONNULL_END
