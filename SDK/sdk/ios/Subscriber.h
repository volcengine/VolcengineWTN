/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIView.h>
#import "veWTNDefines.h"

NS_ASSUME_NONNULL_BEGIN

@class Subscriber;
/** {zh}
 * @type callback
 * @brief 订阅端事件回调接口
 */
/** {en}
 * @type callback
 * @brief Event handler for Subscriber
 */
@protocol SubscriberDelegate <NSObject>
@optional
/** {zh}
 * @type callback
 * @brief 音视频流的订阅状态发生改变时，收到此回调。
 * @param subscriber 参见 Subscriber{@link #Subscriber}。
 * @param state 当前订阅状态，参看 SubscribeState{@link #SubscribeState}。
 * @param reason 订阅状态发生改变的原因，参看 Reason{@link #Reason}。
 */
/** {en}
 * @type callback
 * @brief Receive this event when the subscribing state changes.
 * @param subscriber See Subscriber{@link #Subscriber}.
 * @param state The current subscribing state. See SubscribeState{@link #SubscribeState}.
 * @param reason Error code when subscribing state changes. See Reason{@link #Reason}
 */
- (void)subscriber:(Subscriber *)subscriber onSubscribeStateChanged:(SubscribeState)state reason:(Reason)reason;

@end
/** {zh}
 * @type api
 * @brief Subscriber Class
 */
/** {en}
 * @type api
 * @brief Subscriber Class
 */
VEWTN_EXPORT @interface Subscriber : NSObject
/** {zh}
 * @type api
 * @brief 创建 Subscriber 实例。
 * @param delegate 参见 SubscriberDelegate{@link #SubscriberDelegate}。
 * @return Subscriber：创建成功。返回一个可用的 Subscriber{@link #Subscriber} 实例  <br>
 * @notes  <br>
 *        + 你应注意保持 SubscriberDelegate 的生命周期必须大于 Subscriber 的生命周期，即 handler 必须在调用 create 之前创建，在调用 destroy{@link #Subscriber#destroy} 之后销毁。  <br>
 *        + 你可以创建多个 Subscriber 实例，以订阅多个音视频流。<br>
 */
/** {en}
 * @type api
 * @brief Create the Subscriber instance.   <br>
 *        If there is no instance in current thread, you must call this API to subscribe a media stream on WTN.
 * @param context Android application context
 * @param handler See SubscriberEventHandler{@link #SubscriberEventHandler}.
 * @return Subscriber: A successfully created Subscriber instance.  <br>
 * @notes  <br>
 *         + You can create multiple Subscriber instances to subscribe multiple media streams. <br>
 *         + The lifecycle of SubscriberDelegate must be longer than that of Subscriber. The handler must be created before calling this API and destroyed after calling destroy{@link #Subscriber#destroy}.  <br>
 */
+ (instancetype)create:(id<SubscriberDelegate> _Nullable)delegate;
/** {zh}
 * @type api
 * @brief 销毁由 create:{@link #Subscriber#create:} 所创建的 Subscriber 实例，并释放所有相关资源
 * @notes  <br>
 *      + 请确保需要销毁的 Subscriber{@link #Subscriber} 实例相关的业务场景全部结束后，才调用此方法  <br>
 *      + 该方法在调用之后，会销毁所有和此 Subscriber{@link #Subscriber} 实例相关的内存，并且停止与服务端的任何交互  <br>
 *      + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API；也不要在回调中等待主线程的执行。不然会造成死锁。
 */    
/** {en}
 * @type api
 * @brief Destroy the Subscriber instance created by create:{@link #Subscriber#create:}, and release all related resources. <br>
 * @notes  <br>
 *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
 *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
 *       + Calling this API will start the SDK exit logic. The thread is held until the exit logic is complete. The thread is retained until the exit logic is complete.  <br>
 *         Therefore, do not call this API directly in the callback thread, or wait for the execution of the main thread in the callback and call this API in the main thread at the same time. Otherwise, it will cause a deadlock.
 */
- (void)destroy;
/** {zh}
 * @type api
 * @brief 设置渲染远端视频流时使用的视图
 * @param view 视图对象。必须保证此对象处于有效的生命周期内。
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes <br>
 *        + 你必须在订阅前，设置渲染视图。 <br>
 *        + 如果需要解除绑定，你可以调用本方法传入空视图。
 */
/** {en}
 * @type api
 * @brief Binds the view to be used for remote video rendering. <br>
 * @param view View object. The object must be of effective lifecycle.
 * @return   <br>
 *         + 0: Success <br>
 *         + -1: Failure. <br>
 * @notes  <br>
 *       + You must bind the view before subscribe to the video stream. <br>
 *       + To unbind the view, call this API with the view set to `null`.
 */
- (int)setRemoteView:(UIView*)view;
/** {zh}
 * @type api
 * @brief 切换播放音频流时使用的听筒/扬声器 <br>
 * @param enable <br>
 *        + true：扬声器 <br>
 *        + false：听筒 <br>
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes <br>
 *        + 默认使用扬声器。 <br>
 *        + 此设置在播放音频流前后都可以生效。
 */
/** {en}
 * @type api
 * @brief Switch the rendering device for audio stream: earpiece or speakerphone.
 * @param enable <br>
 *        + true: earpiece <br>
 *        + false：speakerphone <br>
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *         + Speakerphone is the default device. <br>
 *         + You can switch the audio rendering device before or after the audio rendering.
 */
- (int)setEnableSpeaker:(BOOL)enable;
/** {zh}
 * @type api
 * @brief 开启订阅音视频流。  <br>
 *        调用该方法后，本地用户会收到 subscriber:onSubscribeStateChanged:reason:{@link #SubscriberDelegate#subscriber:onSubscribeStateChanged:reason:} 的回调。  <br>
 * @param url WHEP 请求的 url。<br>
 *        url 由 WTN 固定域名 `https://wtn.volcvideo.com`，POST/PULL 方法，appID，和你设定的 streamID 拼接得到。 <br>
 *        形如：`https://wtn.volcvideo.com/pull/<appID>/<streamID>`
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes  <br>
 *       + 一个 Subscriber 实例仅可以订阅一路音视频流。<br>
 *       + 订阅音视频流前，请先确保此流已发布。如果未发布，会订阅失败。 <br>
 *       + 订阅前需要先调用 setRemoteView:{@link #Subscriber#setRemoteView:} 设置视图。 <br>
 *       + 调用 stopSubscribe{@link #Subscriber#stopSubscribe} 可以停止订阅。 <br>
 */
/** {en}
 * @type api
 * @brief Subscribe the remote media streams. <br>
 *        After calling the API, you will receive subscriber:onSubscribeStateChanged:reason:{@link #SubscriberDelegate#subscriber:onSubscribeStateChanged:reason:}.
 * @param url The url for WHEP request. <br>
 *        Concatenate the fixed url `https://wtn.volcvideo.com`, POST/PULL method, appID, and your specified streamID into the value of url. <br>
 *        The url is like `https://wtn.volcvideo.com/pull/<appID>/<streamID>`.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *        + You can only subscribe to one media stream with one Subscriber instance. <br>
 *        + Ensure the stream is published before subscribe to the stream. <br>
 *        + Call setRemoteView:{@link #Subscriber#setRemoteView:} to set the remote view for the media stream before subscribing. <br>
 *        + Call stopSubscribe{@link #Subscriber#stopSubscribe} to stop subscribing.
 */
- (int)startSubscribe:(NSString*)url;
/** {zh}
 * @type api
 * @brief 停止订阅音视频流
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败  <br>
 * @notes <br>
 *        + 调用 startSubscribe:{@link #Subscriber#startSubscribe:} 发布音视频流。
 *        + 调用该方法后，会收到 subscriber:onSubscribeStateChanged:reason:{@link #SubscriberDelegate#subscriber:onSubscribeStateChanged:reason:} 的回调。
 */
/** {en}
 * @type api
 * @brief Stop subscribing to the media stream.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *         + Call startSubscribe:{@link #Subscriber#startSubscribe:} to start subscribing. <br>
 *         + Receive subscriber:onSubscribeStateChanged:reason:{@link #SubscriberDelegate#subscriber:onSubscribeStateChanged:reason:} after calling this API.
 */
- (int)stopSubscribe;
/** {zh}
 * @type api
 * @brief 查询音视频流订阅状态
 * @return  <br>
 *        + true：正在订阅 <br>
 *        + false：未订阅 <br>
 */
/** {en}
 * @type api
 * @brief Inspect the subscribing state.
 * @return   <br>
 *         + true: Subscribing <br>
 *         + false: Not subscribing <br>
 */
- (bool)isSubscribed;

@end

NS_ASSUME_NONNULL_END
