/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIView.h>
#import "veWTNDefines.h"

NS_ASSUME_NONNULL_BEGIN

@class Publisher;
/** {zh}
 * @type callback
 * @brief 发布端事件回调接口
 */
/** {en}
 * @type callback
 * @brief Event handler for Publisher
 */
@protocol PublisherDelegate <NSObject>
@optional
/** {zh}
 * @type callback
 * @brief 音视频流的发布状态发生改变时，收到此回调。
 * @param publisher Publisher 实例
 * @param state 当前发布状态，参看 PublishState{@link #PublishState}。
 * @param reason 发布状态发生改变的原因，参看 Reason{@link #Reason}。
 */
/** {en}
 * @type callback
 * @brief Receive this event when the publish state of the media stream changes.
 * @param publisher Publisher instance
 * @param state The current publishing state. See PublishState{@link #PublishState}.
 * @param reason Error code for the change. See Reason{@link #Reason}.
 */
- (void)publisher:(Publisher *)publisher onPublishStateChanged:(PublishState)state reason:(Reason)reason;

@end
/** {zh}
 * @type api
 * @brief Publisher Class
 */
/** {en}
 * @type api
 * @brief Publisher Class
 */
VEWTN_EXPORT @interface Publisher : NSObject
/** {zh}
 * @type api
 * @brief 创建 Publisher 实例。<br>
 *        如果当前线程中未创建实例，那么你必须先调用此 API，以使用 WTN 提供的各种音视频能力。  <br>
 * @param delegate 参见 PublisherDelegate{@link #PublisherDelegate}
 * @return Publisher：创建成功。返回一个可用的 Publisher{@link #Publisher} 实例  <br>
 * @notes  <br>
 *        + 你可以创建多个 Publisher 实例，以发布多个音视频流。<br>
 *        + 你应注意保持 PublisherDelegate 的生命周期必须大于 Publisher 的生命周期，即 PublisherDelegate 必须在调用 create 之前创建，在调用 destroy{@link #Publisher#destroy} 之后销毁。
 */
/** {en}
 * @type api
 * @brief Create the Publisher instance.   <br>
 *        If there is no instance in current thread, you must call this API to publish a media stream on WTN.
 * @param delegate See PublisherDelegate{@link #PublisherDelegate}.
 * @return Publisher: A successfully created Publisher instance.  <br>
 * @notes  <br>
 *         + You can create multiple Publisher instances to publish multiple media streams. <br>
 *         + The lifecycle of PublisherDelegate must be longer than that of Publisher. The PublisherDelegate must be created before calling this API and destroyed after calling destroy{@link #Publisher#destroy}.  <br>
 */
+ (instancetype)create:(id<PublisherDelegate> _Nullable)delegate;
/** {zh}
 * @type api
 * @brief 销毁由 create:{@link #Publisher#create:} 所创建的 Publisher 实例，并释放所有相关资源
 * @notes  <br>
 *      + 请确保需要销毁的 Publisher{@link #Publisher} 实例相关的业务场景全部结束后，才调用此方法。  <br>
 *      + 该方法在调用之后，会销毁所有和此 Publisher{@link #Publisher} 实例相关的内存，并且停止与服务端的任何交互。  <br>
 *      + 调用本方法会启动 SDK 退出逻辑。线程会保留，直到退出逻辑完成。
 */
/** {en}
 * @type api
 * @brief Destroy the Publisher instance created by create:{@link #Publisher#create:}, and release all related resources. <br>
 * @notes  <br>
 *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
 *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
 *       + Calling this API will start the SDK exit logic. The thread is held until the exit logic is complete. The engine thread is retained until the exit logic is complete.  <br>
 *         Therefore, do not call this API directly in the callback thread, or wait for the execution of the main thread in the callback and call this API in the main thread at the same time. Otherwise, it will cause a deadlock.
 */
- (void)destroy;
/** {zh}
 * @type api
 * @brief 设置视频采集参数
 * @param videoConfig 视频采集参数。参看 VideoConfig{@link #VideoConfig}。
 * @return  <br>
 *        + 0: 成功  <br>
 *        + < 0: 失败  <br>
 * @notes  <br>
 *     + 必须在调用 startVideoCapture{@link #Publisher#startVideoCapture} 前调用此接口。<br>
 *     + 建议同一设备上的不同 Publisher 实例使用相同的视频采集参数。
 */
/** {en}
 * @type api
 * @brief Set the video capture parameters.
 * @param videoConfig See VideoConfig{@link #VideoConfig}.
 * @return   <br>
 *         + 0: Success; <br>
 *         + < 0: Failure; <br>
 * @notes   <br>
 *         + You must call this API before calling startVideoCapture{@link #Publisher#startVideoCapture}. <br>
 *         + It is recommended that the Publisher instances of the same device use the same parameters.
 */
- (int)setVideoConfig:(VideoConfig * _Nonnull)videoConfig;
/** {zh}
     * @type api
     * @brief 设置本地视频渲染时使用的视图。
     * @param view 视图对象。必须保证此对象处于有效的生命周期内。
     * @return  <br>
     *        + 0: 成功  <br>
     *        + -1: 失败。 <br>
     * @notes <br>
     *        + 如果需要解除绑定，你可以调用本方法传入空视图。<br>
     *        + 必须在调用 startVideoCapture{@link #Publisher#startVideoCapture} 前调用此接口。
     */
    /** {en}
     * @type api
     * @brief Binds the view to be used for local video rendering. <br>
     * @param view View object. The object must be of effective lifecycle.
     * @return   <br>
     *         + 0: Success <br>
     *         + -1: Failure. The view is empty. <br>
     * @notes  <br>
     *       + To unbind the view, call this API with the view set to `null`. <br>
     *       + Bind the view before calling startVideoCapture{@link #Publisher#startVideoCapture}. <br>
     */
- (int)setLocalView:(UIView * _Nullable)view;
/** {zh}
 * @type api
 * @brief 开启摄像头采集。默认为关闭状态。  <br>
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes  <br>
 *       + 发布前后都可调用。 <br>
 *       + 调用 stopVideoCapture{@link #Publisher#stopVideoCapture} 可以停止摄像头采集。 <br>
 *       + 采集使用的摄像头由 switchCamera{@link #Publisher#switchCamera} 指定。
 */
/** {en}
 * @type api
 * @brief Enable video capture. The default setting is off.   <br>
 * @return   <br>
 *         + 0: Success <br>
 *         + -1: Failure. The view is empty. <br>
 * @notes   <br>
 *        + You can call this API before or after publishing the stream. <br>
 *        + Call stopVideoCapture{@link #Publisher#stopVideoCapture} to stop video capture. <br>
 *        + The camera used for video capture is set by switchCamera{@link #Publisher#switchCamera}.
 */
- (int)startVideoCapture;
/** {zh}
 * @type api
 * @brief 关闭摄像头采集。
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes  <br>
 *       + 调用 startVideoCapture{@link #Publisher#startVideoCapture} 可以开启摄像头采集。  <br>
 *       + 如果不调用本方法停止采集，则只有当销毁 Publisher 实例时，摄像头采集才会停止。   <br>
 */
/** {en}
 * @type api
 * @brief Stop video capture.
 * @return   <br>
 *         + 0: Success <br>
 *         + -1: Failure. The view is empty. <br>
 * @notes  <br>
 *        + Call startVideoCapture {@link #RTCVideo#startVideoCapture} to enable video capture. <br>
 *        + Without calling this API, the video capture sustains until the Publisher instance is destroyed.
 */
- (int)stopVideoCapture;
/** {zh}
 * @type api
 * @brief 使用内部采集时，切换使用的摄像头：前置或后置摄像头。
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败  <br>
 * @notes <br>
 *        + 默认使用前置摄像头。<br>
 *        + 如果你正在使用相机进行视频采集，切换操作当即生效；如果未处于采集状态，后续开启摄像头采集时，会打开设定的摄像头。
 */
/** {en}
 * @type api
 * @brief Switch the camera for video capture between front-facing/back-facing cameras.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *         + Front-facing camera is the default camera. <br>
 *         + If the video capture is on-going, the switch is effective immediately. If the video capture is off, the set camera is used when video capture starts.
 */
- (int)switchCamera;
/** {zh}
 * @type api
 * @brief 开启麦克风采集。默认为关闭状态。  <br>
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败  <br>
 * @notes  <br>
 *       + 调用 stopAudioCapture{@link #Publisher#stopAudioCapture} 可以关闭音频采集设备。 <br>
 *       + 无论是否发布音频数据，你都可以调用此方法开启麦克风采集。
 */
/** {en}
 * @type api
 * @brief Start audio capture. The default is off.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes   <br>
 *        + Call stopAudioCapture{@link #Publisher#stopAudioCapture} to stop audio capture. <br>
 *        + You can call this API to start video capture whether to publish the audio stream.
 */
- (int)startAudioCapture;
/** {zh}
 * @type api
 * @brief 关闭麦克风采集。
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败  <br>
 * @notes  <br>
 *       + 调用 startAudioCapture{@link #Publisher#startAudioCapture} 可以开启麦克风采集。  <br>
 *       + 如果不调用此方法停止麦克风采集，则只有当销毁 Publisher 实例时，麦克风采集才会停止。
 */
/** {en}
 * @type api
 * @brief Stop audio capture.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes   <br>
 *        + Call startAudioCapture{@link #Publisher#startAudioCapture} to enable audio capture. <br>
 *        + Without calling this API, the audio capture will sustain until the Publisher instance is destroyed.
 */
- (int)stopAudioCapture;
/** {zh}
 * @type api
 * @brief 发布音视频流
 * @param url WHIP 请求的 url。<br>
 *        url 由 WTN 固定域名 `https://wtn.volcvideo.com`，PUB 方法，appID，你设定的 streamID, token 和 TokenType 拼接得到。 <br>
 *        形如：`https://wtn.volcvideo.com/pub/<appID>/<streamID>?Token=<token>&TokenType=Bearer`
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败  <br>
 * @notes <br>
 *        + 对于一个 Publisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。<br>
 *        + 无论是否开启音视频采集，你都可以调用此接口开启发布。<br>
 *        + 调用此接口后，会收到 publisher:onPublishStateChanged:reason:{@link #PublisherDelegate#publisher:onPublishStateChanged:reason:}。<br>
 *        + 调用 stopPublish{@link #Publisher#stopPublish} 取消发布。
 */
/** {en}
 * @type api
 * @brief Publish the captured media stream.
 * @param url The url for WHIP request. <br>
 *        Concatenate the fixed url `https://wtn.volcvideo.com`, PUB method, appID, your specified streamID, token and TokenType into the value of url. <br>
 *        The url is like `https://wtn.volcvideo.com/pub/<appID>/<streamID>?Token=<token>&TokenType=Bearer`.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *        + You can only publish one media stream with one Publisher instance. Calling this API during publishing will fail. <br>
 *        + You can call this API to start publishing media streams with or without media capture. <br>
 *        + After calling this API, you will receive publisher:onPublishStateChanged:reason:{@link #PublisherDelegate#publisher:onPublishStateChanged:reason:}. <br>
 *        + Call stopPublish{@link #Publisher#stopPublish} to stop publishing the media stream.
 */
- (int)startPublish:(NSString*)url;
/** {zh}
 * @type api
 * @brief 停止发布音视频流
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败
 * @notes <br>
 *        + 调用 startPublish:{@link #Publisher#startPublish:} 发布音视频流。<br>
 *        + 调用此接口后，会收到 publisher:onPublishStateChanged:reason:{@link #PublisherDelegate#publisher:onPublishStateChanged:reason:}。
 */
/** {en}
 * @type api
 * @brief Stops publishing the media stream.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *         + Call startPublish:{@link #Publisher#startPublish:} to publish the media stream. <br>
 *         + After calling this API, you will receive publisher:onPublishStateChanged:reason:{@link #PublisherDelegate#publisher:onPublishStateChanged:reason:}.
 */
- (int)stopPublish;
/** {zh}
 * @type api
 * @brief 查询音视频流发布状态
 * @return  <br>
 *        + true：发布中 <br>
 *        + false：未发布 <br>
 */
/** {en}
 * @type api
 * @brief Inspect the media stream publishing status.
 * @return  <br>
 *        + true: Publishing. <br>
 *        + false：Not publishing. <br>
 */
- (bool)isPublishing;

@end

NS_ASSUME_NONNULL_END
