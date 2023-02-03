/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

import android.content.Context;
import android.view.View;
import com.vewtn.internal.RtcEngineImpl;

/**
 * @type api
 */
public abstract class RtcEngine {
    /**
     * @hidden
     */
    private static RtcEngineImpl rtcEngine;

    /** {zh}
     * @type api
     * @brief 创建引擎实例。
     * @param context Android Application Context
     * @param handler SDK 回调给应用层的 Handler，详见 RtcEventHandler{@link #RtcEventHandler}。
     * @return  <br>
     *        + RtcEngine：创建成功。返回一个可用的 RtcEngine{@link #RtcEngine} 实例  <br>
     *        + Null：.so 文件加载失败，创建失败。
     * @notes  <br>
     *        + 你应注意保持 RtcEventHandler 的生命周期必须大于 RtcEngine 的生命周期，即 handler 必须在调用 createRtcEngine 之前创建，在调用 destroyRtcEngine{@link #RtcEngine#destroyRtcEngine} 之后销毁。  <br>
     *        + 你只能创建1个 RtcEngine 实例。<br>
     */
    /** {en}
     * @type api
     * @brief Create the RtcEngine instance.
     * @param context Android application context
     * @param handler See RtcEventHandler{@link #RtcEventHandler}.
     * @return  <br>
     *         + RtcEngine: A successfully created RtcEngine instance.  <br>
     *         + Null: Failed to load the .so file. No instance is returned. <br>
     * @notes  <br>
     *         + You can only create 1 instance of RtcEngine. <br>
     *         + The lifecycle of the handler must be longer than that of RtcEngine. The handler must be created before calling this API and destroyed after calling destroyRtcEngine{@link #RtcEngine#destroyRtcEngine}.  <br>
     */
    public synchronized static RtcEngine createRtcEngine(Context context, RtcEventHandler handler) {
        if (rtcEngine == null && context != null) {
            rtcEngine = new RtcEngineImpl(context, handler);
        }
        return rtcEngine;
    }
    /** {zh}
     * @type api
     * @brief 销毁由 createRtcEngine{@link #RtcEngine#createRtcEngine} 所创建的 RtcEngine 实例，并释放所有相关资源
     * @return <br>
     *        + 0：设置成功；<br>
     *        + <0：设置失败
     * @notes  <br>
     *      + 请确保和需要销毁的 RtcEngine{@link #RtcEngine} 实例相关的业务场景全部结束后，才调用此方法  <br>
     *      + 该方法在调用之后，会销毁所有和此 RtcEngine{@link #RtcEngine} 实例相关的内存，并且停止与媒体服务器的任何交互  <br>
     *      + 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，也不要在回调中等待主线程的执行，并同时在主线程调用本方法。不然会造成死锁。
     */
    /** {en}
     * @type api
     * @brief Destroy the RtcEngine instance created by createRtcEngine{@link #RtcEngine#createRtcEngine}, and release all related resources. <br>
     * @return <br>
     *        + 0: Success. <br>
     *        + <0: Failure.
     * @notes  <br>
     *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
     *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
     *       + Calling this API will start the SDK exit logic. The thread is held until the exit logic is complete. The thread is retained until the exit logic is complete.  <br>
     *         Therefore, do not call this API directly in the callback thread, or wait for the execution of the main thread in the callback and call this API in the main thread at the same time. Otherwise, it will cause a deadlock.
     */
    public synchronized int destroyRtcEngine() {
        if (rtcEngine != null) {
            rtcEngine.destroy();
            rtcEngine = null;
            return 0;
        }
        return -1;
    }

    /** {zh}
     * @type api
     * @brief 创建发布 Publisher。
     * @param pubExtraParam 设置发布的音视频源
     * @return  <br>
     *        + Publisher：创建成功。返回一个可用的 Publisher{@link #Publisher} 实例  <br>
     *        + Null：创建失败。可能原因有：已经创建一个内部源Publisher后再次创建。
     * @notes  <br>
     *      + 发布可以创建任意多个外部源，但至多只能创建一个音视频内部源。 <br>
     */
    /** {en}
     * @type api
     * @brief Create the Publisher.
     * @param pubExtraParam 设置发布的音视频源
     * @return  <br>
     *         + Publisher: A successfully created Publisher instance.  <br>
     *         + Null: Failed to create. Possible reasons are: Creating an internal source Publisher again after it has already been created. <br>
     * @notes  <br>
     *         + Publisher can create as many external sources as you want, but only up to one internal source for audio and video. <br>
     */
    public abstract Publisher createPublisher(PubExtraParam pubExtraParam);
    /** {zh}
     * @type api
     * @brief 销毁由 createPublisher{@link #RtcEngine#createPublisher} 所创建的 Publisher 实例，并释放所有相关资源
     * @param publisher 待销毁的 publisher 对象
     * @notes  <br>
     *      + 请确保和需要销毁的 Publisher{@link #Publisher} 实例相关的业务场景全部结束后，才调用此方法  <br>
     *      + 该方法在调用之后，会销毁所有和此 Publisher{@link #Publisher} 实例相关的内存，并且停止与媒体服务器的任何交互  <br>
     */
    /** {en}
     * @type api
     * @brief Destroy the Publisher instance created by createPublisher{@link #RtcEngine#createPublisher}, and release all related resources. <br>
     * @notes  <br>
     *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
     *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
     */
    public abstract void destroyPublisher(Publisher publisher);
    /** {zh}
     * @type api
     * @brief 创建发布 Subscriber。
     * @param subExtraParam 设置订阅的音视频源
     * @return  <br>
     *        + Subscriber：创建成功。返回一个可用的 Subscriber{@link #Subscriber} 实例  <br>
     *        + Null：创建失败。可能原因有：已经创建一个内部源 Subscriber 后再次创建。
     * @notes  <br>
     *      + 订阅可以创建任意多个外部源，但至多只能创建一个音视频内部源。 <br>
     */
    /** {en}
     * @type api
     * @brief Create the Subscriber.
     * @param subExtraParam 设置发布的音视频源
     * @return  <br>
     *         + Subscriber: A successfully created Subscriber instance.  <br>
     *         + Null: Failed to create. Possible reasons are: Creating an internal source Subscriber again after it has already been created. <br>
     * @notes  <br>
     *         + Subscriber can create as many external sources as you want, but only up to one internal source for audio and video. <br>
     */
    public abstract Subscriber createSubscriber(SubExtraParam subExtraParam);
    /** {zh}
     * @type api
     * @brief 销毁由 createSubscriber{@link #RtcEngine#createSubscriber} 所创建的 Subscriber 实例，并释放所有相关资源
     * @param subscriber 待销毁的 subscriber 对象
     * @notes  <br>
     *      + 请确保和需要销毁的 Subscriber{@link #Subscriber} 实例相关的业务场景全部结束后，才调用此方法  <br>
     *      + 该方法在调用之后，会销毁所有和此 Subscriber{@link #Subscriber} 实例相关的内存，并且停止与媒体服务器的任何交互  <br>
     */
    /** {en}
     * @type api
     * @brief Destroy the Subscriber instance created by createSubscriber{@link #RtcEngine#createSubscriber}, and release all related resources. <br>
     * @notes  <br>
     *       + Call this API after all business scenarios related to the instance are destroyed.  <br>
     *       + When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  <br>
     */
    public abstract void destroySubscriber(Subscriber subscriber);
}
