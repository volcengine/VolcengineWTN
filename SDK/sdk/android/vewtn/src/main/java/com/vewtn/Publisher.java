/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

import android.view.View;
/** {zh}
 * @type api
 * @brief Publisher Class
 */
/** {en}
 * @type api
 * @brief Publisher Class
 */
public interface Publisher {
   /** {zh}
    * @type api
    * @brief 设置视频采集参数
    * @param videoConfig 视频采集参数。参看 VideoConfig{@link #VideoConfig}。
    * @return  <br>
    *        + 0: 成功  <br>
    *        + < 0: 失败  <br>
    * @notes  <br>
    *     + 必须在调用 startVideoCapture{@link #Publisher#startVideoCapture} 前调用此接口。<br>
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
    */
    int setVideoConfig(VideoConfig videoConfig);
    /** {zh}
     * @type api
     * @brief 设置本地视频渲染时使用的视图。
     * @param view 视图对象。必须保证此对象处于有效的生命周期内。
     * @return  <br>
     *        + 0: 成功  <br>
     *        + -1: 失败。 <br>
     * @notes <br>
     *        + 如果需要解除绑定，你可以调用本方法传入空视图。<br>
     *        + 必须在调用 startVideoCapture{@link #Publisher#startVideoCapture} 前调用此接口。 <br>
     *        + 通过此接口，仅可以渲染通过 startVideoCapture{@link #Publisher#startVideoCapture} 采集到的视频数据。
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
     *       + With this API, you can only render the video data captured by startVideoCapture{@link #Publisher#startVideoCapture}.
     */
    int setLocalView(View view);
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
    int startVideoCapture();
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
     *        + Call startVideoCapture startVideoCapture{@link #Publisher#startVideoCapture} to enable video capture. <br>
     *        + Without calling this API, the video capture sustains until the Publisher instance is destroyed.
     */
    int stopVideoCapture();
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
    int switchCamera();
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
    int startAudioCapture();
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
    int stopAudioCapture();
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
     *        + 调用此接口后，会收到 onPublishStateChanged{@link #RtcEventHandler#onPublishStateChanged}。<br>
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
     *        + After calling this API, you will receive onPublishStateChanged{@link #RtcEventHandler#onPublishStateChanged}. <br>
     *        + Call stopPublish{@link #Publisher#stopPublish} to stop publishing the media stream.
     */
    int startPublish(String url);
    /** {zh}
     * @type api
     * @brief 停止发布音视频流
     * @return  <br>
     *        + 0：方法调用成功  <br>
     *        + < 0：方法调用失败
     * @notes <br>
     *        + 调用 startPublish{@link #Publisher#startPublish} 发布音视频流。<br>
     *        + 调用此接口后，会收到 onPublishStateChanged{@link #RtcEventHandler#onPublishStateChanged}。
     */
    /** {en}
     * @type api
     * @brief Stops publishing the media stream.
     * @return   <br>
     *         + 0: Success <br>
     *         + < 0: Failure <br>
     * @notes  <br>
     *         + Call startPublish{@link #Publisher#startPublish} to publish the media stream. <br>
     *         + After calling this API, you will receive onPublishStateChanged{@link #RtcEventHandler#onPublishStateChanged}.
     */
    int stopPublish();
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
    boolean isPublishing();
    /** {zh}
     * @type api
     * @brief 设置本地音频发送状态。
     * @param mute 发送音频数据的状态。默认值为发送。<br>
     *      + ture：不发送音频数据 <br>
     *      + false：正常发送音频数据
     * @notes 发布前后，采集前后都可调用。
     * @return <br>
     *      + 0：调用成功；<br>
     *      + <0：调用失败
     */
    /** {en}
     * @type api
     * @brief Mute local audio or not.
     * @param mute The mute status of the local audio. Not muted by default. <br>
     *      + ture: do not send audio data. <br>
     *      + false: send audio data.
     * @notes You can call this API regardless of the capture status or publishing status of the audio.
     * @return <br>
     *      + 0: Success <br>
     *      + <0: Failure
     */
    int setLocalAudioMute(boolean mute);
    /** {zh}
     * @type api
     * @brief 设置本地视频发送状态。
     * @param mute 发送视频数据的状态。默认值为发送。<br>
     *      + ture：不发送视频数据 <br>
     *      + false：正常发送视频数据
     * @notes 发布前后，采集前后都可调用。
     * @return <br>
     *      + 0：调用成功；<br>
     *      + <0：调用失败
     */
    /** {en}
     * @type api
     * @brief Mute local video or not.
     * @param mute The mute status of the local video. Not muted by default. <br>
     *      + ture: do not send video data. <br>
     *      + false: send video data.
     * @notes You can call this API regardless of the capture status or publishing status of the video.
     * @return <br>
     *      + 0: Success <br>
     *      + <0: Failure
     */
    int setLocalVideoMute(boolean mute);
    /** {zh}
     * @type api
     * @brief 推送外部编码帧。
     * @param encodedFrame 外部编码帧。参看 EncodedVideoFrame{@link #EncodedVideoFrame}。在推送过程中，帧的分辨率应保持一致。
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
     * @param encodedFrame Encoded frame. See EncodedVideoFrame{@link #EncodedVideoFrame}. During the process, the resolution of the frames must remain the same.
     * @notes <br>
     *      + Do not support encoding feedback and request keyframe feedback. <br>
     *      + Push externally encoded frames periodically according to the actual frame rate.
     * @return <br>
     *      + 0: Success <br>
     *      + <0: Failure
     */
    boolean pushEncodedVideoFrame(EncodedVideoFrame encodedFrame);
    /** {zh}
     * @type api
     * @brief 推送外部视频帧。
     * @param videoFrame 视频帧。参看 VideoFrame{@link #VideoFrame}。在推送过程中，帧的分辨率应保持一致，帧的宽高应为 16 的倍数。
     * @notes 根据实际帧率，周期推送外部编码帧。
     * @return <br>
     *      + 0：调用成功；<br>
     *      + <0：调用失败
     */
    /** {en}
     * @brief Push external video frame.
     * @param videoFrame Encoded frame. See VideoFrame{@link #VideoFrame}. During the process, the resolution of the frames must remain the same. The frame width and the frame height must be multiples of 16.
     * @notes Push external frames periodically according to the actual frame rate.
     * @return <br>
     *      + 0: Success <br>
     *      + <0: Failure
     */
    boolean pushVideoFrame(VideoFrame videoFrame);
    /** {zh}
     * @type api
     * @brief 推送外部音频帧
     * @param audioFrame 音频帧。参看 AudioFrame{@link #AudioFrame}。在推送过程中，帧的参数应保持一致。
     * @notes 周期推送外部编码帧。建议周期为 10 ms。
     * @return <br>
     *      + 0：调用成功；<br>
     *      + <0：调用失败
     */
    /** {en}
     * @type api
     * @brief Push external audio data.
     * @param audioFrame See AudioFrame{@link #AudioFrame}. During the process, the parameters of the frames must remain the same. 
     * @notes Push external frames periodically. The period is recommended to be 10 ms.
     * @return <br>
     *      + 0: Success <br>
     *      + <0: Failure
     */
     boolean pushAudioFrame(AudioFrame audioFrame);
}
