/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;
/** {zh}
 * @type callback
 * @brief 订阅端事件回调接口
 */
/** {en}
 * @type callback
 * @brief Event handler for Subscriber
 */
public abstract class SubscriberEventHandler {
    /** {zh}
     * @type keytype
     * @brief 音视频流订阅状态
     */
    /** {en}
     * @type keytype
     * @brief Subscribe state
     */
    public enum SubscribeState {
       /** {zh}
        * @brief 开始订阅成功
        */
       /** {en}
        * @brief Succeed to start subscribing the stream
        */
        StartSubscribeSuccess,
        /** {zh}
        * @brief 开始订阅失败
        */
       /** {en}
        * @brief Fail to start subscribing the stream
        */
        StartSubscribeFailed,
        /** {zh}
        * @brief 停止订阅成功
        */
       /** {en}
        * @brief Succeed to stop subscribing the stream
        */
        StopSubscribeSuccess,
        /** {zh}
        * @brief 停止订阅失败
        */
       /** {en}
        * @brief Fail to stop subscribing the stream
        */
        StopSubscribeFailed,
    };

    /** {zh}
     * @type keytype
     * @brief 订阅成功或失败的原因
     */
    /** {en}
     * @type keytype
     * @brief reason
     */
    public enum Reason {
        /** {zh}
         * @brief 成功
         */
        /** {en}
         * @brief Success
         */
         Success,
        /** {zh}
         * @brief 失败。参数错误。
         */
        /** {en}
         * @brief Failure. Parameter error.
         */
         ParameterError,
        /** {zh}
         * @brief 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。
         */
        /** {en}
         * @brief Failure. Token error, including wrong token, expired token and token authentication failed.
         */
         TokenError,
        /** {zh}
         * @brief 失败。未找到资源。
         */
        /** {en}
         * @brief Failure. Resources not found.
         */
         ResourcesNotFound,
        /** {zh}
         * @brief 失败。服务器内部错误。请重试。
         */
        /** {en}
         * @brief Failure. Server internal error. Please try again.
         */
         ServerInternalError,
        /** {zh}
         * @brief 失败。SDK 内部错误。请重试。
         */
        /** {en}
         * @brief Failure. SDK internal error. Please try again.
         */
         SDKInternalError,
        /** {zh}
         * @brief 失败。网络错误。
         */
        /** {en}
         * @brief Failure. Network error.
         */
         NetworkError,
        /** {zh}
         * @brief 失败。未知错误。请重试。
         */
        /** {en}
         * @brief Failure. Unknown error. Please try again.
         */
         UnknowError,
        /** {zh}
         * @brief 失败。摄像头或者麦克风无权限。
         */
        /** {en}
         * @brief Failure. No permission for camera or microphone.
         */
         DeviceNoPermission,
     }
    /** {zh}
     * @type callback
     * @brief 音视频流的订阅状态发生改变时，收到此回调。
     * @param state 当前订阅状态，参看 SubscribeState{@link #SubscribeState}。
     * @param reason 订阅状态发生改变的原因，参看 Reason{@link #Reason}。
     */
    /** {en}
     * @type callback
     * @brief Receive this event when the subscribing state changes.
     * @param state The current subscribing state. See SubscribeState{@link #SubscribeState}.
     * @param reason Error code when subscribing state changes. See Reason{@link #Reason}
     */
    public void onSubscribeStateChanged(SubscribeState state,
                                        Reason reason) {}
}
