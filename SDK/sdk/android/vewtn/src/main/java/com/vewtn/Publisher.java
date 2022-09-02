/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

import android.content.Context;
import android.view.View;
import com.vewtn.internal.PublisherImpl;
/** {zh}
 * @type api
 * @brief Publisher Class
 */
/** {en}
 * @type api
 * @brief Publisher Class
 */
public abstract class Publisher {
    private static PublisherImpl publiser;
    /** {zh}
     * @type api
     * @brief 创建 Publisher 实例。<br>
     *        如果当前线程中未创建实例，那么你必须先调用此 API，以使用 WTN 提供的各种音视频能力。  <br>
     * @param context Android Application Context
     * @param handler SDK 回调给应用层的 Handler，详见 PublisherEventHandler{@link #PublisherEventHandler}
     * @return  <br>
     *        + Publisher：创建成功。返回一个可用的 Publisher{@link #Publisher} 实例  <br>
     *        + Null：.so 文件加载失败，创建失败。
     * @notes  <br>
     *        + 你可以创建多个 Publisher 实例，以发布多个音视频流。<br>
     *        + 你应注意保持 PublisherEventHandler 的生命周期必须大于 Publisher 的生命周期，即 handler 必须在调用 create 之前创建，在调用 destroy{@link #Publisher#destroy} 之后销毁。
     */
    /** {en}
     * @type api
     * @brief Create the Publisher instance.   <br>
     *        If there is no instance in current thread, you must call this API to publish a media stream on WTN.
     * @param context Android application context
     * @param handler See IRTCVideoEventHandler{@link #IRTCVideoEventHandler}.
     * @return  <br>
     *         + Publisher: A successfully created Publisher instance.  <br>
     *         + Null: Failed to load the .so file. No instance is returned. <br>
     * @notes  <br>
     *         + You can create multiple Publisher instances to publish multiple media streams. <br>
     *         + The lifecycle of the handler must be longer than that of Publisher. The handler must be created before calling this API and destroyed after calling destroy{@link #Publisher#destroy}.  <br>
     */
    public synchronized static Publisher create(Context context, PublisherEventHandler handler) {
        if (publiser == null && context != null) {
            publiser = new PublisherImpl(context.getApplicationContext(), handler);
        }
        return publiser;
    }

    /** {zh}
     * @type api
     * @brief 销毁由 create{@link #Publisher#create} 所创建的 Publisher 实例，并释放所有相关资源
     * @return <br>
     *        + 0：设置成功；<br>
     *        + <0：设置失败
     * @notes  <br>
     *      + 请确保和需要销毁的 Publisher{@link #Publisher} 实例相关的业务场景全部结束后，才调用此方法。  <br>
     *      + 该方法在调用之后，会销毁所有和此 Publisher{@link #Publisher} 实例相关的内存，并且停止与媒体服务器的任何交互。  <br>
     *      + 调用本方法会启动 SDK 退出逻辑。线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，也不要在回调中等待主线程的执行，并同时在主线程调用本方法。不然会造成死锁。
     */
    /** {en}
     * @type api
     * @brief Destroy the Publisher instance created by create{@link #Publisher#create}, and release all related resources. <br>
     * @return <br>
     *        + 0: Success. <br>
     *        + <0: Failure.
     * @notes  <br>
     *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
     *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
     *       + Calling this API will start the SDK exit logic. The thread is held until the exit logic is complete. The engine thread is retained until the exit logic is complete.  <br>
     *         Therefore, do not call this API directly in the callback thread, or wait for the execution of the main thread in the callback and call this API in the main thread at the same time. Otherwise, it will cause a deadlock.
     */
    public synchronized int destroy() {
        if (publiser != null) {
            publiser.destroyPublisher();
            publiser = null;
            return 0;
        }
        return -1;
    }
   /** {zh}
    * @type api
    * @brief 设置视频采集参数
    * @param video_config 视频采集参数。参看 VideoConfig{@link #VideoConfig}。
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
    * @param video_config See VideoConfig{@link #VideoConfig}.
    * @return   <br>
    *         + 0: Success; <br>
    *         + < 0: Failure; <br>
    * @notes   <br>
    *         + You must call this API before calling startVideoCapture{@link #Publisher#startVideoCapture}. <br>
    *         + It is recommended that the Publisher instances of the same device use the same parameters.
    */
    public abstract int setVideoConfig(VideoConfig video_config);
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
    public abstract int setLocalView(View view);
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
    public abstract int startVideoCapture();
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
    public abstract int stopVideoCapture();
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
    public abstract int switchCamera();
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
    public abstract int startAudioCapture();
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
    public abstract int stopAudioCapture();
    /** {zh}
     * @type api
     * @brief 发布音视频流
     * @param url WHIP 请求的 url。<br>
     *        url 由 WTN 固定域名 `https://wtn.volcvideo.com`，POST/PULL 方法，appID，你设定的 streamID, token 和 TokenType 拼接得到。 <br>
     *        形如：`https://wtn.volcvideo.com/push/<appID>/<streamID>?Token=<token>&TokenType=Bearer`
     * @return  <br>
     *        + 0：方法调用成功  <br>
     *        + < 0：方法调用失败  <br>
     * @notes <br>
     *        + 对于一个 Publisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。<br>
     *        + 无论是否开启音视频采集，你都可以调用此接口开启发布。<br>
     *        + 调用此接口后，会收到 onPublishStateChanged{@link #PublisherEventHandler#onPublishStateChanged}。<br>
     *        + 调用 stopPublish{@link #Publisher#stopPublish} 取消发布。
     */
    /** {en}
     * @type api
     * @brief Publish the captured media stream.
     * @param url The url for WHIP request. <br>
     *        Concatenate the fixed url `https://wtn.volcvideo.com`, POST/PULL method, appID, your specified streamID, token and TokenType into the value of url. <br>
     *        The url is like `https://wtn.volcvideo.com/push/<appID>/<streamID>?Token=<token>&TokenType=Bearer`.
     * @return   <br>
     *         + 0: Success <br>
     *         + < 0: Failure <br>
     * @notes  <br>
     *        + You can only publish one media stream with one Publisher instance. Calling this API during publishing will fail. <br>
     *        + You can call this API to start publishing media streams with or without media capture. <br>
     *        + After calling this API, you will receive onPublishStateChanged{@link #PublisherEventHandler#onPublishStateChanged}. <br>
     *        + Call stopPublish{@link #Publisher#stopPublish} to stop publishing the media stream.
     */
    public abstract int startPublish(String url);
    /** {zh}
     * @type api
     * @brief 停止发布音视频流
     * @return  <br>
     *        + 0：方法调用成功  <br>
     *        + < 0：方法调用失败
     * @notes <br>
     *        + 调用 startPublish{@link #Publisher#startPublish} 发布音视频流。<br>
     *        + 调用此接口后，会收到 onPublishStateChanged{@link #PublisherEventHandler#onPublishStateChanged}。
     */
    /** {en}
     * @type api
     * @brief Stops publishing the media stream.
     * @return   <br>
     *         + 0: Success <br>
     *         + < 0: Failure <br>
     * @notes  <br>
     *         + Call startPublish{@link #Publisher#startPublish} to publish the media stream. <br>
     *         + After calling this API, you will receive onPublishStateChanged{@link #PublisherEventHandler#onPublishStateChanged}.
     */
    public abstract int stopPublish();
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
    public abstract boolean isPublishing();
}
