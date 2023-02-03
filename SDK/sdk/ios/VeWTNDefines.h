/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#import <Foundation/Foundation.h>
#define VEWTN_IOS_EXPORT __attribute__((visibility("default")))

/** {zh}
 * @type keytype
 * @brief 发布的音频源类型
 */
/** {en}
 * @type keytype
 * @brief The type of the audio source to publish.
 */
typedef NS_ENUM(NSInteger, VEWTNPubAudioSource) {
   /** {zh}
    * @brief 内部麦克风采集的数据。默认值。
    */
   /** {en}
    * @brief The data recorded by built-in microphone. Default value.
    */
    VEWTNPubAudioSourceInternalRecord = 0,
   /** {zh}
    * @brief 推送的外部 PCM 数据
    */
   /** {en}
    * @brief PCM data pushed externally.
    */
    VEWTNPubAudioSourcePushExternalPCM = 1,
};
/** {zh}
 * @type keytype
 * @brief 发布的视频源类型
 */
/** {en}
 * @type keytype
 * @brief The type of the video source to publish.
 */
typedef NS_ENUM(NSInteger, VEWTNPubVideoSource) {
   /** {zh}
    * @brief 内部摄像头采集的数据。默认值。
    */
   /** {en}
    * @brief The data recorded by built-in camera. Default value.
    */
    VEWTNPubVideoSourceInternalCamera = 0,
   /** {zh}
    * @brief 推送的外部 YUV 数据
    */
   /** {en}
    * @brief YUV data pushed externally.
    */
    VEWTNPubVideoSourcePushExternalYUV = 1,
   /** {zh}
    * @brief 推送的外部 H.264 编码的数据
    */
   /** {en}
    * @brief H.264 encoded data pushed externally.
    */
    VEWTNPubVideoSourcePushExternalH264 = 2,
};
/** {zh}
 * @type keytype
 * @brief 订阅的音频源类型
 */
/** {en}
 * @type keytype
 * @brief The audio source type to subscribe.
 */
typedef NS_ENUM(NSInteger, VEWTNSubAudioSource) {
   /** {zh}
    * @brief 内部采集的音频数据。默认值。
    * @notes 当你选择此参数时，SDK 会进行音频渲染。
    */
   /** {en}
    * @brief Internally captured audio data. Default value.
    * @notes If you use this value, the SDK plays the audio stream.
    */
    VEWTNSubAudioSourceInternalPlay = 0,
   /** {zh}
    * @brief PCM 音频数据。
    * @notes 当你选择此参数时，会收到 onAudioFrame{@link #RtcEventHandler#onAudioFrame}。
    */
   /** {en}
    * @brief Audio data in PCM format.
    * @notes If you use this value, you'll receive onAudioFrame{@link #RtcEventHandler#onAudioFrame}.
    */
    VEWTNSubAudioSourceCallbackPCM = 1,
};

/** {zh}
 * @type keytype
 * @brief 订阅的视频源类型
 */
/** {en}
 * @type keytype
 * @brief The video source type to subscribe.
 */
typedef NS_ENUM(NSInteger, VEWTNSubVideoSource) {
   /** {zh}
    * @brief 内部采集的视频数据。默认值。
    * @notes 当你选择此参数时，SDK 会在绑定的 view 中，进行视频渲染。
    */
   /** {en}
    * @brief Internally captured video data. Default value.
    * @notes If you use this value, the SDK renders the video stream in the specified view.
    */
    VEWTNSubVideoSourceInternalRender = 0,
   /** {zh}
    * @brief YUV 格式的视频数据。
    * @notes 当你选择此参数时，会收到 onVideoFrame{@link #RtcEventHandler#onVideoFrame}。
    */
   /** {en}
    * @brief Video stream in YUV format.
    * @notes If you use this value, you'll receive onVideoFrame{@link #RtcEventHandler#onVideoFrame}.
    */
    VEWTNSubVideoSourceCallbackYUV = 1,
   /** {zh}
    * @brief H.264 格式封装的视频数据。
    * @notes 当你选择此参数时，会收到 onEncodedVideoFrame{@link #RtcEventHandler#onEncodedVideoFrame}。
    */
   /** {en}
    * @brief Video stream encoded in H.264 protocal.
    * @notes If you use this value, you'll receive onEncodedVideoFrame{@link #RtcEventHandler#onEncodedVideoFrame}.
    */
    VEWTNSubVideoSourceCallbackH264 = 2,
};

/** {zh}
 * @type keytype
 * @brief 外部编码视频帧的编码类型
 */
/** {en}
 * @type keytype
 * @brief The encoding type of the encoded video frame
 */
typedef NS_ENUM(NSInteger, VEWTNEncodedVideoFrameType) {
   /**
    * @brief H.264
    */
    VEWTNCodceH264 = 0,
};

/** {zh}
 * @type keytype
 * @brief 视频帧类型
 */
/** {en}
 * @type keytype
 * @brief Video frame type
 */
typedef NS_ENUM(NSInteger, VEWTNVideoFrameType) {
   /**
    * @brief YUV-I420
    */
    VEWTNYuvI420 = 0,
};

/** {zh}
 * @type keytype
 * @brief 音频帧类型
 */
/** {en}
 * @type keytype
 * @brief Audio frame type
 */
typedef NS_ENUM(NSInteger, VEWTNAudioFrameType) {
   /**
    * @brief PCM 16
    */
    VEWTNPcm16 = 0,
};

/** {zh}
 * @type keytype
 * @brief 音视频流状态
 */
/** {en}
 * @type keytype
 * @brief state
 */
typedef NS_ENUM(NSInteger, VEWTNState) {
   /** {zh}
    * @brief 开始发布/订阅成功
    */
   /** {en}
    * @brief Success to start publishing/subscribing the stream.
    */
    VEWTNStateStartSuccess = 0,
   /** {zh}
    * @brief 开始发布/订阅失败
    */
   /** {en}
    * @brief Failure to start publishing/subscribing the stream.
    */
    VEWTNStateStartFailed = 1,
   /** {zh}
    * @brief 停止发布/订阅成功
    */
   /** {en}
    * @brief Success to stop publishing/subscribing the stream.
    */
    VEWTNStateStopSuccess = 2,
   /** {zh}
    * @brief 停止发布/订阅失败
    */
   /** {en}
    * @brief Failure to stop publishing/subscribing the stream.
    */
    VEWTNStateStopFailed = 3,
};

/** {zh}
 * @type keytype
 * @brief 发布成功或失败的原因
 */
/** {en}
 * @type keytype
 * @brief reason
 */
typedef NS_ENUM(NSInteger, VEWTNReason) {
   /** {zh}
    * @brief 成功
    */
   /** {en}
    * @brief Success
    */
    VEWTNReasonSuccess = 0,
   /** {zh}
    * @brief 失败。参数错误。
    */
   /** {en}
    * @brief Failure. Invalid parameter.
    */
    VEWTNReasonParameterError = 1,
   /** {zh}
    * @brief 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。
    */
   /** {en}
    * @brief Failure. Token error, including invalid token, expired token and token authentication failed.
    */
    VEWTNReasonTokenError = 2,
   /** {zh}
    * @brief 失败。未找到资源。
    */
   /** {en}
    * @brief Failure. Resources not found.
    */
    VEWTNReasonResourcesNotFound = 3,
   /** {zh}
    * @brief 失败。服务器内部错误。请重试。
    */
   /** {en}
    * @brief Failure. Internal server error. Please try again.
    */
    VEWTNReasonServerInternalError = 4,
   /** {zh}
    * @brief 失败。SDK 内部错误。请重试。
    */
   /** {en}
    * @brief Failure. Internal SDK error. Please try again.
    */
    VEWTNReasonSDKInternalError = 5,
   /** {zh}
    * @brief 失败。网络错误。
    */
   /** {en}
    * @brief Failure. Network error.
    */
    VEWTNReasonNetworkError = 6,
   /** {zh}
    * @brief 失败。未知错误。请重试。
    */
   /** {en}
    * @brief Failure. Unknown error. Please try again.
    */
    VEWTNReasonUnknowError = 7,
   /** {zh}
    * @brief 失败。订阅的流不存在。
    */
   /** {en}
    * @brief Failure. Stream not exist.
    */
    VEWTNReasonStreamNotExist = 8,
};

/** {zh}
 * @type keytype
 * @brief 发布参数
 */
/** {en}
 * @type keytype
 * @brief publish param
 */
VEWTN_IOS_EXPORT @interface VEWTNPubExtraParam : NSObject
/** {zh}
 * @brief 音频源发布
 */
/** {en}
 * @brief Publish audio source
 */
@property (nonatomic, assign) VEWTNPubAudioSource pub_audio_source;
/** {zh}
 * @brief 视频源发布
 */
/** {en}
 * @brief Publish video source
 */
@property (nonatomic, assign) VEWTNPubVideoSource pub_video_source;
@end

/** {zh}
 * @type keytype
 * @brief 订阅额外参数
 */
/** {en}
 * @type keytype
 * @brief Extra param for subscribing
 */
VEWTN_IOS_EXPORT @interface VEWTNSubExtraParam: NSObject
/** {zh}
 * @brief 音频源订阅。参看 VEWTNSubAudioSource{@link #VEWTNSubAudioSource}。
 */
/** {en}
 * @brief Subscribe audio source. See VEWTNSubAudioSource{@link #VEWTNSubAudioSource}.
 */
@property (nonatomic, assign) VEWTNSubAudioSource sub_audio_source;
/** {zh}
 * @brief 视频源订阅。参看 VEWTNSubVideoSource{@link #VEWTNSubVideoSource}。
 */
/** {en}
 * @brief Subscribe video source. See VEWTNSubVideoSource{@link #VEWTNSubVideoSource}.
 */
@property (nonatomic, assign) VEWTNSubVideoSource sub_video_source;
@end

/** {zh}
 * @type keytype
 * @brief 编码后的视频帧
 */
/** {en}
 * @type keytype
 * @brief Encoded video frame struct
 */
VEWTN_IOS_EXPORT @interface VEWTNEncodedVideoFrame: NSObject
/** {zh}
 * @brief 帧类型。目前仅支持 H.264，且必须是 Annex B 格式。
 */
/** {en}
 * @brief Frame type. Only H264 - Annex B supported.
 */
@property (nonatomic, assign) VEWTNEncodedVideoFrameType codec_type;
/** {zh}
 * @brief 数据缓存
 */
/** {en}
 * @brief data buffer
 */
@property (nonatomic, assign) const uint8_t * data;
/** {zh}
 * @brief 数据缓存大小
 */
/** {en}
 * @brief data buffer size
 */
@property (nonatomic, assign) int size;
/** {zh}
 * @brief 宽度（px）
 */
/** {en}
 * @brief Width in px
 */
@property (nonatomic, assign) int width;
/** {zh}
 * @brief 高度（px）
 */
/** {en}
 * @brief Height in px
 */
@property (nonatomic, assign) int height;
/** {zh}
 * @brief 时间戳，单位毫秒
 */
/** {en}
 * @brief Timestamp in ms
 */
@property (nonatomic, assign) unsigned long timestamp_ms;
/** {zh}
 * @brief 逆时针旋转角度，单位为度。可选值为 0，90，180，270。
 */
/** {en}
 * @brief Counterclockwise rotation in degrees. Options are 0, 90, 180, 270。
 */
@property (nonatomic, assign) int rotation;
/** {zh}
 * @brief 是否为关键帧。如果是关键帧，那么，必须包含 SPS 和 PPS 信息。
 */
/** {en}
 * @brief key frame or not. If it is a key frame, it must contain SPS and PPS information.
 */
@property (nonatomic, assign) BOOL is_key_frame;
@end

/** {zh}
 * @type keytype
 * @brief 视频帧
 */
/** {en}
 * @type keytype
 * @brief Video frame struct
 */
VEWTN_IOS_EXPORT @interface VEWTNVideoFrame: NSObject
/** {zh}
 * @brief 帧类型。目前仅支持YUV I420。
 */
/** {en}
 * @brief Frame type. Only YUV I420 supported.
 */
@property (nonatomic, assign) VEWTNVideoFrameType type;
/** {zh}
 * @brief Y 分量数据缓存
 */
/** {en}
 * @brief Y buffer
 */
@property (nonatomic, assign) const uint8_t * y_buffer;
/** {zh}
 * @brief U 分量数据缓存
 */
/** {en}
 * @brief U buffer
 */
@property (nonatomic, assign) const uint8_t * u_buffer;
/** {zh}
 * @brief V 分量数据缓存
 */
/** {en}
 * @brief V buffer
 */
@property (nonatomic, assign) const uint8_t * v_buffer;
/** {zh}
 * @brief Y 分量 stride
 */
/** {en}
 * @brief Y stride
 */
@property (nonatomic, assign) int y_stride;
/** {zh}
 * @brief U 分量 stride
 */
/** {en}
 * @brief U stride
 */
@property (nonatomic, assign) int u_stride;
/** {zh}
 * @brief V 分量 stride
 */
/** {en}
 * @brief V stride
 */
@property (nonatomic, assign) int v_stride;
/** {zh}
 * @brief 宽度（px）
 */
/** {en}
 * @brief Width in px
 */
@property (nonatomic, assign) int width;
/** {zh}
 * @brief 高度（px）
 */
/** {en}
 * @brief Height in px
 */
@property (nonatomic, assign) int height;
/** {zh}
 * @brief 时间戳，单位毫秒。
 */
/** {en}
 * @brief Timestamp in ms
 */
@property (nonatomic, assign) unsigned long timestamp_ms;
/** {zh}
 * @brief 逆时针旋转角度，可选值为 0，90，180，270
 */
/** {en}
 * @brief Counterclockwise rotation. The options are 0, 90, 180, and 270.
 */
@property (nonatomic, assign) int rotation;
@end

/** {zh}
 * @type keytype
 * @brief 音频帧
 */
/** {en}
 * @type keytype
 * @brief Audio frame
 */
VEWTN_IOS_EXPORT @interface VEWTNAudioFrame: NSObject
/** {zh}
 * @brief 帧类型。目前仅支持 PCM16。
 */
/** {en}
 * @brief Frame type. Only PCM16 support.
 */
@property (nonatomic, assign) VEWTNAudioFrameType type;
/** {zh}
 * @brief 数据缓存
 */
/** {en}
 * @brief data buffer
 */
@property (nonatomic, assign) const uint8_t * data;
/** {zh}
 * @brief 数据缓存大小
 */
/** {en}
 * @brief data buffer size
 */
@property (nonatomic, assign) int sample_length;
/** {zh}
 * @brief 时间戳，单位：毫秒。
 */
/** {en}
 * @brief Timestamp in ms
 */
@property (nonatomic, assign) unsigned long timestamp_ms;
/** {zh}
 * @brief 采样率
 */
/** {en}
 * @brief SampleRate
 */
@property (nonatomic, assign) int sample_rate;
/** {zh}
 * @brief 通道数
 */
/** {en}
 * @brief Number of channels
 */
@property (nonatomic, assign) int channels;
@end

/** {zh}
 * @type keytype
 * @brief 视频配置
 */
/** {en}
 * @type keytype
 * @brief Video config
 */
VEWTN_IOS_EXPORT @interface VEWTNVideoConfig: NSObject
/** {zh}
 * @brief 宽度（px）。默认值是 640。
 */
/** {en}
 * @brief Width in px. 640 by default.
 */
@property (nonatomic, assign) int width;
/** {zh}
 * @brief 高度（px）。默认值是 960。
 */
/** {en}
 * @brief Height in px. 960 by default.
 */
@property (nonatomic, assign) int height;
/** {zh}
 * @brief 最大帧率（fps）。默认值是 15。
 */
/** {en}
 * @brief Maximum fps. 15 by default.
 */
@property (nonatomic, assign) int max_fps;
@end

/** {zh}
 * @type keytype
 * @brief 视频卡顿信息
 */
/** {en}
 * @type keytype
 * @brief Video stall info
 */
VEWTN_IOS_EXPORT @interface VEWTNStallInfo: NSObject
/** {zh}
 * @brief 500 毫秒卡顿次数
 */
/** {en}
 * @brief Stall count in 500ms
 */
@property (nonatomic, assign) int stall_count_500;
/** {zh}
 * @brief 500 毫秒卡顿时长
 */
/** {en}
 * @brief Stall duration in 500ms
 */
@property (nonatomic, assign) int stall_duration_500;
/** {zh}
 * @brief 200 毫秒卡顿次数
 */
/** {en}
 * @brief Stall count in 200ms
 */
@property (nonatomic, assign) int stall_count_200;
/** {zh}
 * @brief 200 毫秒卡顿时长
 */
/** {en}
 * @brief Stall duration in 200ms
 */
@property (nonatomic, assign) int stall_duration_200;
/** {zh}
 * @brief 帧率
 */
/** {en}
 * @brief Frame rate
 */
@property (nonatomic, assign) int frame_rate;
@end

/** {zh}
 * @type keytype
 * @brief 音频流数据
 */
/** {en}
 * @type keytype
 * @brief Audio stats
 */
VEWTN_IOS_EXPORT @interface VEWTNRTCAudioStats: NSObject
/** {zh}
 * @brief 发送字节数，单位：字节。
 */
/** {en}
 * @brief Number of bytes sent.
 */
@property (nonatomic, assign) int64_t bytes_sent;
/** {zh}
 * @brief 接收字节数，单位：字节。
 */
/** {en}
 * @brief Number of bytes received.
 */
@property (nonatomic, assign) int64_t bytes_received;
/** {zh}
 * @brief 每秒发送字节数，单位：字节。
 */
/** {en}
 * @brief Number of bytes sent per second.
 */
@property (nonatomic, assign) double bytes_sent_per_second;
/** {zh}
 * @brief 每秒接收字节数，单位：字节。
 */
/** {en}
 * @brief Number of bytes received per second.
 */
@property (nonatomic, assign) double bytes_received_per_second;
/** {zh}
 * @brief 音量强度。<br>
 *        取值范围：`[0, 1.0]`。值为 `1.0` 时，表示 0dBov；值为 `0.5` 时，表示 6dB SPL。
 */
/** {en}
 * @brief audio level. <br>
 *        The range is `[0, 1.0]`. When the value is `1.0`, the sound pressure level is 0dBov; when the value is `0.5`, the sound pressure level is 6dB SPL.
 */
@property (nonatomic, assign) double audio_level;
/** {zh}
 * @brief 总音量强度。值越大，强度越高。
 */
/** {en}
 * @brief Total audio energy. Higher the value, bigger the total energy.
 */
@property (nonatomic, assign) double total_audio_energy;
/** {zh}
 * @brief 编码器 ID
 */
/** {en}
 * @brief codec ID
 */
@property (nonatomic, strong) NSString* codec_id;
/** {zh}
 * @brief 收到的 RTCP RR 包的总个数。
 */
/** {en}
 * @brief The total number of the received RTCP RR packets.
 */
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
/** {zh}
 * @brief 总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。
 */
/** {en}
 * @brief Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.
 */
@property (nonatomic, assign) double total_round_trip_time;
/** {zh}
 * @brief 发送包个数
 */
/** {en}
 * @brief packets sent
 */
@property (nonatomic, assign) uint32_t packets_sent;
/** {zh}
 * @brief 接收包个数
 */
/** {en}
 * @brief packets received
 */
@property (nonatomic, assign) uint32_t packets_received;
/** {zh}
 * @brief 发布过程中的丢包数
 */
/** {en}
 * @brief Packets lost during publishing.
 */
@property (nonatomic, assign) uint32_t packets_lost;
/** {zh}
 * @brief 丢包率。取值范围：`[0, 1.0]`。
 */
/** {en}
 * @brief Packet lost rate during publishing. The range is `[0, 1.0]`.
 */
@property (nonatomic, assign) double franction_loss;
/** {zh}
 * @brief jitter 抖动。单位：秒。
 */
/** {en}
 * @brief jitter in seconds.
 */
@property (nonatomic, assign) double jitter;
@end

/** {zh}
 * @type keytype
 * @brief 视频流数据
 */
/** {en}
 * @type keytype
 * @brief Video stats
 */
VEWTN_IOS_EXPORT @interface VEWTNRTCVideoStats: NSObject
/** {zh}
 * @brief 发送字节数，单位字节
 */
/** {en}
 * @brief Number of bytes sent, in bytes
 */
@property (nonatomic, assign) int64_t bytes_sent;
/** {zh}
 * @brief 接收字节数，单位字节
 */
/** {en}
 * @brief Number of bytes received, in bytes
 */
@property (nonatomic, assign) int64_t bytes_received;
/** {zh}
 * @brief 每秒发送字节数，单位字节
 */
/** {en}
 * @brief Number of bytes sent per second, in bytes
 */
@property (nonatomic, assign) int64_t bytes_sent_per_second;
/** {zh}
 * @brief 每秒接收字节数，单位字节
 */
/** {en}
 * @brief Number of bytes received per second, in bytes
 */
@property (nonatomic, assign) int64_t bytes_received_per_second;
/** {zh}
 * @brief 视频帧宽度。单位：px。
 */
/** {en}
 * @brief Video frame width in px.
 */
@property (nonatomic, assign) int64_t frame_width;
/** {zh}
 * @brief 视频帧高度。单位：px。
 */
/** {en}
 * @brief Video frame height in px.
 */
@property (nonatomic, assign) int64_t frame_height;
/** {zh}
 * @brief 帧率
 */
/** {en}
 * @brief frames per second
 */
@property (nonatomic, assign) double frames_per_second;
/** {zh}
 * @brief 编码器 ID
 */
/** {en}
 * @brief codec ID
 */
@property (nonatomic, strong) NSString* codec_id;
/** {zh}
 * @brief 收到的 RTCP RR 包的总个数。
 */
/** {en}
 * @brief The total number of the received RTCP RR packets.
 */
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
/** {zh}
 * @brief 总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。
 */
/** {en}
 * @brief Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.
 */
@property (nonatomic, assign) double total_round_trip_time;
/** {zh}
 * @brief 发送包个数
 */
/** {en}
 * @brief packets sent
 */
@property (nonatomic, assign) uint32_t packets_sent;
/** {zh}
 * @brief 接收包个数
 */
/** {en}
 * @brief packets received
 */
@property (nonatomic, assign) uint32_t packets_received;
/** {zh}
 * @brief 发布过程中的丢包数
 */
/** {en}
 * @brief Packets lost during publishing.
 */
@property (nonatomic, assign) uint32_t packets_lost;
/** {zh}
 * @brief 丢包率。取值范围：`[0, 1.0]`。
 */
/** {en}
 * @brief Packet lost rate during publishing. The range is `[0, 1.0]`.
 */
@property (nonatomic, assign) double franction_loss;
/** {zh}
 * @brief jitter 抖动。单位：秒。
 */
/** {en}
 * @brief jitter in seconds.
 */
@property (nonatomic, assign) double jitter;
@end
