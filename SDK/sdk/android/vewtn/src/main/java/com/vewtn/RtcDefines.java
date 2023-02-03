/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/**
 * @type keytype
 */
public class RtcDefines {
    /** {zh}
     * @type keytype
     * @brief 发布的音频源类型
     */
    /** {en}
     * @type keytype
     * @brief The type of the audio source to publish.
     */
    public enum PubAudioSource {
        /** {zh}
         * @brief 内部麦克风采集的数据。默认值。
         */
        /** {en}
         * @brief The data recorded by built-in microphone. Default value.
         */
        InternalRecord,
        /** {zh}
         * @brief 推送的外部 PCM 数据
         */
        /** {en}
         * @brief PCM data pushed externally.
         */
        PushExternalPCM
    }
    /** {zh}
     * @type keytype
     * @brief 发布的视频源类型
     */
    /** {en}
     * @type keytype
     * @brief The type of the video source to publish.
     */
    public enum PubVideoSource {
        /** {zh}
         * @brief 内部摄像头采集的数据。默认值。
         */
        /** {en}
         * @brief The data recorded by built-in camera. Default value.
         */
        InternalCamera,
        /** {zh}
         * @brief 推送的外部 YUV 数据
         */
        /** {en}
         * @brief YUV data pushed externally.
         */
        PushExternalYUV,
        /** {zh}
         * @brief 推送的外部 H.264 编码的数据
         */
        /** {en}
         * @brief H.264 encoded data pushed externally.
         */
        PushExternalH264
    }
    /** {zh}
     * @type keytype
     * @brief 订阅的音频源类型
     */
    /** {en}
     * @type keytype
     * @brief The audio source type to subscribe.
     */
    public enum SubAudioSource {
        /** {zh}
         * @brief 内部采集的音频数据。默认值。
         * @notes 当你选择此参数时，SDK 会进行音频渲染。
         */
        /** {en}
         * @brief Internally captured audio data. Default value.
         * @notes If you use this value, the SDK plays the audio stream.
         */
        InternalPlay,
        /** {zh}
         * @brief PCM 音频数据。
         * @notes 当你选择此参数时，会收到 onAudioFrame{@link #RtcEventHandler#onAudioFrame}。
         */
        /** {en}
         * @brief Audio data in PCM format.
         * @notes If you use this value, you'll receive onAudioFrame{@link #RtcEventHandler#onAudioFrame}.
         */
        CallbackPCM
    }
    /** {zh}
     * @type keytype
     * @brief 订阅的视频源类型
     */
    /** {en}
     * @type keytype
     * @brief The video source type to subscribe.
     */
    public enum SubVideoSource {
        /** {zh}
         * @brief 内部采集的视频数据。默认值。
         * @notes 当你选择此参数时，SDK 会在绑定的 view 中，进行视频渲染。
         */
        /** {en}
         * @brief Internally captured video data. Default value.
         * @notes If you use this value, the SDK renders the video stream in the specified view.
         */
        InternalRender,
        /** {zh}
         * @brief YUV 格式的视频数据。
         * @notes 当你选择此参数时，会收到 onVideoFrame{@link #RtcEventHandler#onVideoFrame}。
         */
        /** {en}
         * @brief Video stream in YUV format.
         * @notes If you use this value, you'll receive onVideoFrame{@link #RtcEventHandler#onVideoFrame}.
         */
        CallbackYUV,
        /** {zh}
         * @brief H.264 格式封装的视频数据。
         * @notes 当你选择此参数时，会收到 onEncodedVideoFrame{@link #RtcEventHandler#onEncodedVideoFrame}。
         */
        /** {en}
         * @brief Video stream encoded in H.264 protocal.
         * @notes If you use this value, you'll receive onEncodedVideoFrame{@link #RtcEventHandler#onEncodedVideoFrame}.
         */
        CallbackH264
    }
    /** {zh}
     * @type keytype
     * @brief 外部编码视频帧的编码类型
     */
    /** {en}
     * @type keytype
     * @brief The encoding type of the encoded video frame
     */
    public enum EncodedVideoFrameType {
        /**
         * @brief H.264
         */
        CodecH264,
    }
    /** {zh}
     * @type keytype
     * @brief 视频帧类型
     */
    /** {en}
     * @type keytype
     * @brief Video frame type
     */
    public enum VideoFrameType {
        /**
         * @brief YUV-I420
         */
        YuvI420,
    }
    /** {zh}
     * @type keytype
     * @brief 音频帧类型
     */
    /** {en}
     * @type keytype
     * @brief Audio frame type
     */
    public enum AudioFrameType {
        /**
         * @brief PCM 16
         */
        Pcm16,
    }
    /** {zh}
     * @type keytype
     * @brief 音视频流状态
     */
    /** {en}
     * @type keytype
     * @brief state
     */
    public enum State {
        /** {zh}
         * @brief 开始发布/订阅成功
         */
        /** {en}
         * @brief Success to start publishing/subscribing the stream.
         */
        StartSuccess,
        /** {zh}
         * @brief 开始发布/订阅失败
         */
        /** {en}
         * @brief Failure to start publishing/subscribing the stream.
         */
        StartFailed,
        /** {zh}
         * @brief 停止发布/订阅成功
         */
        /** {en}
         * @brief Success to stop publishing/subscribing the stream.
         */
        StopSuccess,
        /** {zh}
         * @brief 停止发布/订阅失败
         */
        /** {en}
         * @brief Failure to stop publishing/subscribing the stream.
         */
        StopFailed,
    };

    /** {zh}
     * @type keytype
     * @brief 发布成功或失败的原因
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
         * @brief Failure. Invalid parameter.
         */
        ParameterError,
        /** {zh}
         * @brief 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。
         */
        /** {en}
         * @brief Failure. Token error, including invalid token, expired token and token authentication failed.
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
         * @brief Failure. Internal server error. Please try again.
         */
        ServerInternalError,
        /** {zh}
         * @brief 失败。SDK 内部错误。请重试。
         */
        /** {en}
         * @brief Failure. Internal SDK error. Please try again.
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
         * @brief 失败。订阅的流不存在。
         */
        /** {en}
         * @brief Failure. Stream not exist.
         */
        StreamNotExist,
        /** {zh}
         * @brief 失败。没有设备权限（摄像头 or 麦克风）。
         */
        /** {en}
         * @brief Failure. No access to the device (camera or microphone).
         */
        DeviceNoPermission,
    }
}
