/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "VEWTNDefines.h"

NS_ASSUME_NONNULL_BEGIN

/** {zh}
 * @type api
 * @brief VEWTNPublisher Class
 */
/** {en}
 * @type api
 * @brief VEWTNPublisher Class
 */
VEWTN_IOS_EXPORT @interface VEWTNPublisher : NSObject
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
 *        + 对于一个 VEWTNPublisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。<br>
 *        + 无论是否开启音视频采集，你都可以调用此接口开启发布。<br>
 *        + 调用此接口后，会收到 onPublishStateChanged:publishState:reason:{@link #VEWTNEngineDelegate#onPublishStateChanged:publishState:reason:}。<br>
 *        + 调用 stopPublish{@link #VEWTNPublisher#stopPublish} 取消发布。
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
 *        + You can only publish one media stream with one VEWTNPublisher instance. Calling this API during publishing will fail. <br>
 *        + You can call this API to start publishing media streams with or without media capture. <br>
 *        + After calling this API, you will receive onPublishStateChanged:publishState:reason:{@link #VEWTNEngineDelegate#onPublishStateChanged:publishState:reason:}. <br>
 *        + Call stopPublish{@link #VEWTNPublisher#stopPublish} to stop publishing the media stream.
 */
- (int)startPublish:(NSString*)url;
/** {zh}
 * @type api
 * @brief 停止发布音视频流
 * @return  <br>
 *        + 0：方法调用成功  <br>
 *        + < 0：方法调用失败
 * @notes <br>
 *        + 调用 startPublish:{@link #VEWTNPublisher#startPublish:} 发布音视频流。<br>
 *        + 调用此接口后，会收到 onPublishStateChanged:publishState:reason:{@link #VEWTNEngineDelegate#onPublishStateChanged:publishState:reason:}。
 */
/** {en}
 * @type api
 * @brief Stops publishing the media stream.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes  <br>
 *         + Call startPublish:{@link #VEWTNPublisher#startPublish:} to publish the media stream. <br>
 *         + After calling this API, you will receive onPublishStateChanged:publishState:reason:{@link #VEWTNEngineDelegate#onPublishStateChanged:publishState:reason:}.
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
- (BOOL)isPublishing;
/** {zh}
 * @type api
 * @brief 设置本地视频渲染时使用的视图。
 * @param view 视图对象。必须保证此对象处于有效的生命周期内。
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes <br>
 *        + 如果需要解除绑定，你可以调用本方法传入空视图。<br>
 *        + 必须在调用 startVideoCapture{@link #VEWTNPublisher#startVideoCapture} 前调用此接口。
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
 *       + Bind the view before calling startVideoCapture{@link #VEWTNPublisher#startVideoCapture}. <br>
 */
- (void)setLocalView:(UIView*)view;
/** {zh}
* @type api
* @brief 设置视频采集参数
* @param video_config 视频采集参数。参看 VEWTNVideoConfig{@link #VEWTNVideoConfig}。
* @return  <br>
*        + 0: 成功  <br>
*        + < 0: 失败  <br>
* @notes  <br>
*     + 必须在调用 startVideoCapture{@link #VEWTNPublisher#startVideoCapture} 前调用此接口。<br>
*/
/** {en}
* @type api
* @brief Set the video capture parameters.
* @param video_config See VEWTNVideoConfig{@link #VEWTNVideoConfig}.
* @return   <br>
*         + 0: Success; <br>
*         + < 0: Failure; <br>
* @notes   <br>
*         + You must call this API before calling startVideoCapture{@link #VEWTNPublisher#startVideoCapture}. <br>
*/
- (int)setVideoConfig:(VEWTNVideoConfig*)video_config;
/** {zh}
 * @type api
 * @brief 开启摄像头采集。默认为关闭状态。  <br>
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes  <br>
 *       + 发布前后都可调用。 <br>
 *       + 调用 stopVideoCapture{@link #VEWTNPublisher#stopVideoCapture} 可以停止摄像头采集。 <br>
 *       + 采集使用的摄像头由 switchCamera{@link #VEWTNPublisher#switchCamera} 指定。
 */
/** {en}
 * @type api
 * @brief Enable video capture. The default setting is off.   <br>
 * @return   <br>
 *         + 0: Success <br>
 *         + -1: Failure. The view is empty. <br>
 * @notes   <br>
 *        + You can call this API before or after publishing the stream. <br>
 *        + Call stopVideoCapture{@link #VEWTNPublisher#stopVideoCapture} to stop video capture. <br>
 *        + The camera used for video capture is set by switchCamera{@link #VEWTNPublisher#switchCamera}.
 */
- (int)startVideoCapture;
/** {zh}
 * @type api
 * @brief 关闭摄像头采集。
 * @return  <br>
 *        + 0: 成功  <br>
 *        + -1: 失败。 <br>
 * @notes  <br>
 *       + 调用 startVideoCapture{@link #VEWTNPublisher#startVideoCapture} 可以开启摄像头采集。  <br>
 *       + 如果不调用本方法停止采集，则只有当销毁 Publisher 实例时，摄像头采集才会停止。   <br>
 */
/** {en}
 * @type api
 * @brief Stop video capture.
 * @return   <br>
 *         + 0: Success <br>
 *         + -1: Failure. The view is empty. <br>
 * @notes  <br>
 *        + Call startVideoCapture{@link #VEWTNPublisher#startVideoCapture} to enable video capture. <br>
 *        + Without calling this API, the video capture sustains until the Publisher instance is destroyed.
 */
- (int)stopVideoCapture;
/** {zh}
 * @type api
 * @brief 切换摄像头采集时使用的前置/后置摄像头。
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
 *       + 调用 stopAudioCapture{@link #VEWTNPublisher#stopAudioCapture} 可以关闭音频采集设备。 <br>
 *       + 无论是否发布音频数据，你都可以调用此方法开启麦克风采集。
 */
/** {en}
 * @type api
 * @brief Start audio capture. The default is off.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes   <br>
 *        + Call stopAudioCapture{@link #VEWTNPublisher#stopAudioCapture} to stop audio capture. <br>
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
 *       + 调用 startAudioCapture{@link #VEWTNPublisher#startAudioCapture} 可以开启麦克风采集。  <br>
 *       + 如果不调用此方法停止麦克风采集，则只有当销毁 Publisher 实例时，麦克风采集才会停止。
 */
/** {en}
 * @type api
 * @brief Stop audio capture.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 * @notes   <br>
 *        + Call startAudioCapture{@link #VEWTNPublisher#startAudioCapture} to enable audio capture. <br>
 *        + Without calling this API, the audio capture will sustain until the Publisher instance is destroyed.
 */
- (int)stopAudioCapture;
/** {zh}
 * @type api
 * @brief mute 本地音频，不发布。默认值为发布。
 * @param mute 是否发布音频数据。
 * @notes 发布前后都可调用，独立于采集。
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 */
/** {en}
 * @type api
 * @brief Mute the local audio. Do not publish the captured audio.
 * @param mute Whether to publish the locally captured audio.
 * @notes <br>
 *       + You can call this API before or after the media stream is published. <br>
 *       + You can call this API before or after the media is captured.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 */
- (int)setLocalAudioMute:(BOOL)mute;
/** {zh}
 * @type api
 * @brief mute 本地视频，不发布。默认值为发布。
 * @param mute 是否发布视频数据。
 * @notes 发布前后都可调用，独立于采集。
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 */
/** {en}
 * @type api
 * @brief Mute the local video. Do not publish the captured video.
 * @param mute Whether to publish the locally captured video.
 * @notes <br>
 *       + You can call this API before or after the media stream is published. <br>
 *       + You can call this API before or after the media is captured.
 * @return   <br>
 *         + 0: Success <br>
 *         + < 0: Failure <br>
 */
- (int)setLocalVideoMute:(BOOL)mute;
/** {zh}
 * @type api
 * @brief 推送外部编码帧。
 * @param encoded_frame 外部编码帧。参看 VEWTNEncodedVideoFrame{@link #VEWTNEncodedVideoFrame}。在推送过程中，帧的分辨率应保持一致。
 * @notes <br>
 *      + 不支持编码反馈和请求关键帧反馈。<br>
 *      + 根据实际帧率，周期推送外部编码帧。
 * @return <br>
 *      + 0：调用成功；<br>
 *      + <0：调用失败
 */
/** {en}
 * @type api
 * @brief Push external encoded frame.
 * @param encoded_frame Encoded frame. See VEWTNEncodedVideoFrame{@link #VEWTNEncodedVideoFrame}. During the process, the resolution of the frames must remain the same.
 * @notes <br>
 *      + Do not support encoding feedback and request keyframe feedback. <br>
 *      + Push externally encoded frames periodically according to the actual frame rate.
 * @return <br>
 *      + 0: Success <br>
 *      + <0: Failure
 */
- (BOOL)pushEncodedVideoFrame:(VEWTNEncodedVideoFrame*)encoded_frame;
/** {zh}
 * @type api
 * @brief 推送外部视频帧。
 * @param video_frame 视频帧。参看 VEWTNVideoFrame{@link #VEWTNVideoFrame}。在推送过程中，帧的分辨率应保持一致，帧的宽高应为 16 的倍数。
 * @notes 根据实际帧率，周期推送外部编码帧。
 * @return <br> 
 *      + 0：调用成功；<br>
 *      + <0：调用失败
 */
/** {en}
 * @brief Push external video frame.
 * @param video_frame Encoded frame. See VEWTNVideoFrame{@link #VEWTNVideoFrame}. During the process, the resolution of the frames must remain the same. The frame width and the frame height must be multiples of 16.
 * @notes Push external frames periodically according to the actual frame rate.
 * @return <br>
 *      + 0: Success <br>
 *      + <0: Failure
 */
- (BOOL)pushVideoFrame:(VEWTNVideoFrame*)video_frame;
/** {zh}
 * @type api
 * @brief 推送外部音频帧
 * @param audio_frame 音频帧。参看 VEWTNAudioFrame{@link #VEWTNAudioFrame}。在推送过程中，帧的参数应保持一致。
 * @notes 周期推送外部编码帧。建议周期为 10 ms。
 * @return <br>
 *      + 0：调用成功；<br>
 *      + <0：调用失败
 */
/** {en}
 * @type api
 * @brief Push external audio data.
 * @param audio_frame See VEWTNAudioFrame{@link #VEWTNAudioFrame}. During the process, the parameters of the frames must remain the same. 
 * @notes Push external frames periodically. The period is recommended to be 10 ms.
 * @return <br>
 *      + 0: Success <br>
 *      + <0: Failure
 */
- (BOOL)pushAudioFrame:(VEWTNAudioFrame*)audio_frame;
@end

NS_ASSUME_NONNULL_END
