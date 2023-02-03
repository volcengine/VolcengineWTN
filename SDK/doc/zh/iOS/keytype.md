# VEWTNPubAudioSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNPubAudioSource)
```

发布的音频源类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNPubAudioSourceInternalRecord | 0 | 内部麦克风采集的数据。默认值。 |
| VEWTNPubAudioSourcePushExternalPCM | 1 | 推送的外部 PCM 数据 |


# VEWTNPubVideoSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNPubVideoSource)
```

发布的视频源类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNPubVideoSourceInternalCamera | 0 | 内部摄像头采集的数据。默认值。 |
| VEWTNPubVideoSourcePushExternalYUV | 1 | 推送的外部 YUV 数据 |
| VEWTNPubVideoSourcePushExternalH264 | 2 | 推送的外部 H.264 编码的数据 |


# VEWTNSubAudioSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNSubAudioSource)
```

订阅的音频源类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNSubAudioSourceInternalPlay | 0 | 内部采集的音频数据。默认值。 |
| VEWTNSubAudioSourceCallbackPCM | 1 | PCM 音频数据。 |


# VEWTNSubVideoSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNSubVideoSource)
```

订阅的视频源类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNSubVideoSourceInternalRender | 0 | 内部采集的视频数据。默认值。 |
| VEWTNSubVideoSourceCallbackYUV | 1 | YUV 格式的视频数据。 |
| VEWTNSubVideoSourceCallbackH264 | 2 | H.264 格式封装的视频数据。 |


# VEWTNEncodedVideoFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNEncodedVideoFrameType)
```

外部编码视频帧的编码类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNCodceH264 | 0 | H.264 |


# VEWTNVideoFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNVideoFrameType)
```

视频帧类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNYuvI420 | 0 | YUV-I420 |


# VEWTNAudioFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNAudioFrameType)
```

音频帧类型


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNPcm16 | 0 | PCM 16 |


# VEWTNState
```objectivec
typedef NS_ENUM(NSInteger, VEWTNState)
```

音视频流状态


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNStateStartSuccess | 0 | 开始发布/订阅成功 |
| VEWTNStateStartFailed | 1 | 开始发布/订阅失败 |
| VEWTNStateStopSuccess | 2 | 停止发布/订阅成功 |
| VEWTNStateStopFailed | 3 | 停止发布/订阅失败 |


# VEWTNReason
```objectivec
typedef NS_ENUM(NSInteger, VEWTNReason)
```

发布成功或失败的原因


### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| VEWTNReasonSuccess | 0 | 成功 |
| VEWTNReasonParameterError | 1 | 失败。参数错误。 |
| VEWTNReasonTokenError | 2 | 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。 |
| VEWTNReasonResourcesNotFound | 3 | 失败。未找到资源。 |
| VEWTNReasonServerInternalError | 4 | 失败。服务器内部错误。请重试。 |
| VEWTNReasonSDKInternalError | 5 | 失败。SDK 内部错误。请重试。 |
| VEWTNReasonNetworkError | 6 | 失败。网络错误。 |
| VEWTNReasonUnknowError | 7 | 失败。未知错误。请重试。 |
| VEWTNReasonStreamNotExist | 8 | 失败。订阅的流不存在。 |


# VEWTNPubExtraParam
```objectivec
@interface VEWTNPubExtraParam : NSObject
```

发布参数


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **VEWTNPubAudioSource** | [pub_audio_source](#VEWTNPubExtraParam-pub_audio_source) |
| **VEWTNPubVideoSource** | [pub_video_source](#VEWTNPubExtraParam-pub_video_source) |


## 变量说明
<span id="VEWTNPubExtraParam-pub_audio_source"></span>
### pub_audio_source
```objectivec
@property (nonatomic, assign) VEWTNPubAudioSource pub_audio_source;
```
音频源发布


<span id="VEWTNPubExtraParam-pub_video_source"></span>
### pub_video_source
```objectivec
@property (nonatomic, assign) VEWTNPubVideoSource pub_video_source;
```
视频源发布



# VEWTNSubExtraParam
```objectivec
@interface VEWTNSubExtraParam: NSObject
```

订阅额外参数


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **VEWTNSubAudioSource** | [sub_audio_source](#VEWTNSubExtraParam-sub_audio_source) |
| **VEWTNSubVideoSource** | [sub_video_source](#VEWTNSubExtraParam-sub_video_source) |


## 变量说明
<span id="VEWTNSubExtraParam-sub_audio_source"></span>
### sub_audio_source
```objectivec
@property (nonatomic, assign) VEWTNSubAudioSource sub_audio_source;
```
音频源订阅。参看 [VEWTNSubAudioSource](#vewtnsubaudiosource)。


<span id="VEWTNSubExtraParam-sub_video_source"></span>
### sub_video_source
```objectivec
@property (nonatomic, assign) VEWTNSubVideoSource sub_video_source;
```
视频源订阅。参看 [VEWTNSubVideoSource](#vewtnsubvideosource)。



# VEWTNEncodedVideoFrame
```objectivec
@interface VEWTNEncodedVideoFrame: NSObject
```

编码后的视频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **VEWTNEncodedVideoFrameType** | [codec_type](#VEWTNEncodedVideoFrame-codec_type) |
| **const uint8_t*** | [data](#VEWTNEncodedVideoFrame-data) |
| **int** | [size](#VEWTNEncodedVideoFrame-size) |
| **int** | [width](#VEWTNEncodedVideoFrame-width) |
| **int** | [height](#VEWTNEncodedVideoFrame-height) |
| **unsigned** | [timestamp_ms](#VEWTNEncodedVideoFrame-timestamp_ms) |
| **int** | [rotation](#VEWTNEncodedVideoFrame-rotation) |
| **BOOL** | [is_key_frame](#VEWTNEncodedVideoFrame-is_key_frame) |


## 变量说明
<span id="VEWTNEncodedVideoFrame-codec_type"></span>
### codec_type
```objectivec
@property (nonatomic, assign) VEWTNEncodedVideoFrameType codec_type;
```
帧类型。目前仅支持 H.264，且必须是 Annex B 格式。


<span id="VEWTNEncodedVideoFrame-data"></span>
### data
```objectivec
@property (nonatomic, assign) const uint8_t * data;
```
数据缓存


<span id="VEWTNEncodedVideoFrame-size"></span>
### size
```objectivec
@property (nonatomic, assign) int size;
```
数据缓存大小


<span id="VEWTNEncodedVideoFrame-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
宽度（px）


<span id="VEWTNEncodedVideoFrame-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
高度（px）


<span id="VEWTNEncodedVideoFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
时间戳，单位毫秒


<span id="VEWTNEncodedVideoFrame-rotation"></span>
### rotation
```objectivec
@property (nonatomic, assign) int rotation;
```
逆时针旋转角度，单位为度。可选值为 0，90，180，270。


<span id="VEWTNEncodedVideoFrame-is_key_frame"></span>
### is_key_frame
```objectivec
@property (nonatomic, assign) BOOL is_key_frame;
```
是否为关键帧。如果是关键帧，那么，必须包含 SPS 和 PPS 信息。



# VEWTNVideoFrame
```objectivec
@interface VEWTNVideoFrame: NSObject
```

视频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **VEWTNVideoFrameType** | [type](#VEWTNVideoFrame-type) |
| **const uint8_t*** | [y_buffer](#VEWTNVideoFrame-y_buffer) |
| **const uint8_t*** | [u_buffer](#VEWTNVideoFrame-u_buffer) |
| **const uint8_t*** | [v_buffer](#VEWTNVideoFrame-v_buffer) |
| **int** | [y_stride](#VEWTNVideoFrame-y_stride) |
| **int** | [u_stride](#VEWTNVideoFrame-u_stride) |
| **int** | [v_stride](#VEWTNVideoFrame-v_stride) |
| **int** | [width](#VEWTNVideoFrame-width) |
| **int** | [height](#VEWTNVideoFrame-height) |
| **unsigned** | [timestamp_ms](#VEWTNVideoFrame-timestamp_ms) |
| **int** | [rotation](#VEWTNVideoFrame-rotation) |


## 变量说明
<span id="VEWTNVideoFrame-type"></span>
### type
```objectivec
@property (nonatomic, assign) VEWTNVideoFrameType type;
```
帧类型。目前仅支持YUV I420。


<span id="VEWTNVideoFrame-y_buffer"></span>
### y_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * y_buffer;
```
Y 分量数据缓存


<span id="VEWTNVideoFrame-u_buffer"></span>
### u_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * u_buffer;
```
U 分量数据缓存


<span id="VEWTNVideoFrame-v_buffer"></span>
### v_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * v_buffer;
```
V 分量数据缓存


<span id="VEWTNVideoFrame-y_stride"></span>
### y_stride
```objectivec
@property (nonatomic, assign) int y_stride;
```
Y 分量 stride


<span id="VEWTNVideoFrame-u_stride"></span>
### u_stride
```objectivec
@property (nonatomic, assign) int u_stride;
```
U 分量 stride


<span id="VEWTNVideoFrame-v_stride"></span>
### v_stride
```objectivec
@property (nonatomic, assign) int v_stride;
```
V 分量 stride


<span id="VEWTNVideoFrame-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
宽度（px）


<span id="VEWTNVideoFrame-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
高度（px）


<span id="VEWTNVideoFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
时间戳，单位毫秒。


<span id="VEWTNVideoFrame-rotation"></span>
### rotation
```objectivec
@property (nonatomic, assign) int rotation;
```
逆时针旋转角度，可选值为 0，90，180，270



# VEWTNAudioFrame
```objectivec
@interface VEWTNAudioFrame: NSObject
```

音频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **VEWTNAudioFrameType** | [type](#VEWTNAudioFrame-type) |
| **const uint8_t*** | [data](#VEWTNAudioFrame-data) |
| **int** | [sample_length](#VEWTNAudioFrame-sample_length) |
| **unsigned** | [timestamp_ms](#VEWTNAudioFrame-timestamp_ms) |
| **int** | [sample_rate](#VEWTNAudioFrame-sample_rate) |
| **int** | [channels](#VEWTNAudioFrame-channels) |


## 变量说明
<span id="VEWTNAudioFrame-type"></span>
### type
```objectivec
@property (nonatomic, assign) VEWTNAudioFrameType type;
```
帧类型。目前仅支持 PCM16。


<span id="VEWTNAudioFrame-data"></span>
### data
```objectivec
@property (nonatomic, assign) const uint8_t * data;
```
数据缓存


<span id="VEWTNAudioFrame-sample_length"></span>
### sample_length
```objectivec
@property (nonatomic, assign) int sample_length;
```
数据缓存大小


<span id="VEWTNAudioFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
时间戳，单位：毫秒。


<span id="VEWTNAudioFrame-sample_rate"></span>
### sample_rate
```objectivec
@property (nonatomic, assign) int sample_rate;
```
采样率


<span id="VEWTNAudioFrame-channels"></span>
### channels
```objectivec
@property (nonatomic, assign) int channels;
```
通道数



# VEWTNVideoConfig
```objectivec
@interface VEWTNVideoConfig: NSObject
```

视频配置


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [width](#VEWTNVideoConfig-width) |
| **int** | [height](#VEWTNVideoConfig-height) |
| **int** | [max_fps](#VEWTNVideoConfig-max_fps) |


## 变量说明
<span id="VEWTNVideoConfig-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
宽度（px）。默认值是 640。


<span id="VEWTNVideoConfig-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
高度（px）。默认值是 960。


<span id="VEWTNVideoConfig-max_fps"></span>
### max_fps
```objectivec
@property (nonatomic, assign) int max_fps;
```
最大帧率（fps）。默认值是 15。



# VEWTNStallInfo
```objectivec
@interface VEWTNStallInfo: NSObject
```

视频卡顿信息


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [stall_count_500](#VEWTNStallInfo-stall_count_500) |
| **int** | [stall_duration_500](#VEWTNStallInfo-stall_duration_500) |
| **int** | [stall_count_200](#VEWTNStallInfo-stall_count_200) |
| **int** | [stall_duration_200](#VEWTNStallInfo-stall_duration_200) |
| **int** | [frame_rate](#VEWTNStallInfo-frame_rate) |


## 变量说明
<span id="VEWTNStallInfo-stall_count_500"></span>
### stall_count_500
```objectivec
@property (nonatomic, assign) int stall_count_500;
```
500 毫秒卡顿次数


<span id="VEWTNStallInfo-stall_duration_500"></span>
### stall_duration_500
```objectivec
@property (nonatomic, assign) int stall_duration_500;
```
500 毫秒卡顿时长


<span id="VEWTNStallInfo-stall_count_200"></span>
### stall_count_200
```objectivec
@property (nonatomic, assign) int stall_count_200;
```
200 毫秒卡顿次数


<span id="VEWTNStallInfo-stall_duration_200"></span>
### stall_duration_200
```objectivec
@property (nonatomic, assign) int stall_duration_200;
```
200 毫秒卡顿时长


<span id="VEWTNStallInfo-frame_rate"></span>
### frame_rate
```objectivec
@property (nonatomic, assign) int frame_rate;
```
帧率



# VEWTNRTCAudioStats
```objectivec
@interface VEWTNRTCAudioStats: NSObject
```

音频流数据


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int64_t** | [bytes_sent](#VEWTNRTCAudioStats-bytes_sent) |
| **int64_t** | [bytes_received](#VEWTNRTCAudioStats-bytes_received) |
| **double** | [bytes_sent_per_second](#VEWTNRTCAudioStats-bytes_sent_per_second) |
| **double** | [bytes_received_per_second](#VEWTNRTCAudioStats-bytes_received_per_second) |
| **double** | [audio_level](#VEWTNRTCAudioStats-audio_level) |
| **double** | [total_audio_energy](#VEWTNRTCAudioStats-total_audio_energy) |
| **NSString*** | [codec_id](#VEWTNRTCAudioStats-codec_id) |
| **uint64_t** | [round_trip_time_measurements](#VEWTNRTCAudioStats-round_trip_time_measurements) |
| **double** | [total_round_trip_time](#VEWTNRTCAudioStats-total_round_trip_time) |
| **uint32_t** | [packets_sent](#VEWTNRTCAudioStats-packets_sent) |
| **uint32_t** | [packets_received](#VEWTNRTCAudioStats-packets_received) |
| **uint32_t** | [packets_lost](#VEWTNRTCAudioStats-packets_lost) |
| **double** | [franction_loss](#VEWTNRTCAudioStats-franction_loss) |
| **double** | [jitter](#VEWTNRTCAudioStats-jitter) |


## 变量说明
<span id="VEWTNRTCAudioStats-bytes_sent"></span>
### bytes_sent
```objectivec
@property (nonatomic, assign) int64_t bytes_sent;
```
发送字节数，单位：字节。


<span id="VEWTNRTCAudioStats-bytes_received"></span>
### bytes_received
```objectivec
@property (nonatomic, assign) int64_t bytes_received;
```
接收字节数，单位：字节。


<span id="VEWTNRTCAudioStats-bytes_sent_per_second"></span>
### bytes_sent_per_second
```objectivec
@property (nonatomic, assign) double bytes_sent_per_second;
```
每秒发送字节数，单位：字节。


<span id="VEWTNRTCAudioStats-bytes_received_per_second"></span>
### bytes_received_per_second
```objectivec
@property (nonatomic, assign) double bytes_received_per_second;
```
每秒接收字节数，单位：字节。


<span id="VEWTNRTCAudioStats-audio_level"></span>
### audio_level
```objectivec
@property (nonatomic, assign) double audio_level;
```
音量强度。<br>
取值范围：`[0, 1.0]`。值为 `1.0` 时，表示 0dBov；值为 `0.5` 时，表示 6dB SPL。


<span id="VEWTNRTCAudioStats-total_audio_energy"></span>
### total_audio_energy
```objectivec
@property (nonatomic, assign) double total_audio_energy;
```
总音量强度。值越大，强度越高。


<span id="VEWTNRTCAudioStats-codec_id"></span>
### codec_id
```objectivec
@property (nonatomic, strong) NSString* codec_id;
```
编码器 ID


<span id="VEWTNRTCAudioStats-round_trip_time_measurements"></span>
### round_trip_time_measurements
```objectivec
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
```
收到的 RTCP RR 包的总个数。


<span id="VEWTNRTCAudioStats-total_round_trip_time"></span>
### total_round_trip_time
```objectivec
@property (nonatomic, assign) double total_round_trip_time;
```
总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。


<span id="VEWTNRTCAudioStats-packets_sent"></span>
### packets_sent
```objectivec
@property (nonatomic, assign) uint32_t packets_sent;
```
发送包个数


<span id="VEWTNRTCAudioStats-packets_received"></span>
### packets_received
```objectivec
@property (nonatomic, assign) uint32_t packets_received;
```
接收包个数


<span id="VEWTNRTCAudioStats-packets_lost"></span>
### packets_lost
```objectivec
@property (nonatomic, assign) uint32_t packets_lost;
```
发布过程中的丢包数


<span id="VEWTNRTCAudioStats-franction_loss"></span>
### franction_loss
```objectivec
@property (nonatomic, assign) double franction_loss;
```
丢包率。取值范围：`[0, 1.0]`。


<span id="VEWTNRTCAudioStats-jitter"></span>
### jitter
```objectivec
@property (nonatomic, assign) double jitter;
```
jitter 抖动。单位：秒。



# VEWTNRTCVideoStats
```objectivec
@interface VEWTNRTCVideoStats: NSObject
```

视频流数据


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int64_t** | [bytes_sent](#VEWTNRTCVideoStats-bytes_sent) |
| **int64_t** | [bytes_received](#VEWTNRTCVideoStats-bytes_received) |
| **int64_t** | [bytes_sent_per_second](#VEWTNRTCVideoStats-bytes_sent_per_second) |
| **int64_t** | [bytes_received_per_second](#VEWTNRTCVideoStats-bytes_received_per_second) |
| **int64_t** | [frame_width](#VEWTNRTCVideoStats-frame_width) |
| **int64_t** | [frame_height](#VEWTNRTCVideoStats-frame_height) |
| **double** | [frames_per_second](#VEWTNRTCVideoStats-frames_per_second) |
| **NSString*** | [codec_id](#VEWTNRTCVideoStats-codec_id) |
| **uint64_t** | [round_trip_time_measurements](#VEWTNRTCVideoStats-round_trip_time_measurements) |
| **double** | [total_round_trip_time](#VEWTNRTCVideoStats-total_round_trip_time) |
| **uint32_t** | [packets_sent](#VEWTNRTCVideoStats-packets_sent) |
| **uint32_t** | [packets_received](#VEWTNRTCVideoStats-packets_received) |
| **uint32_t** | [packets_lost](#VEWTNRTCVideoStats-packets_lost) |
| **double** | [franction_loss](#VEWTNRTCVideoStats-franction_loss) |
| **double** | [jitter](#VEWTNRTCVideoStats-jitter) |


## 变量说明
<span id="VEWTNRTCVideoStats-bytes_sent"></span>
### bytes_sent
```objectivec
@property (nonatomic, assign) int64_t bytes_sent;
```
发送字节数，单位字节


<span id="VEWTNRTCVideoStats-bytes_received"></span>
### bytes_received
```objectivec
@property (nonatomic, assign) int64_t bytes_received;
```
接收字节数，单位字节


<span id="VEWTNRTCVideoStats-bytes_sent_per_second"></span>
### bytes_sent_per_second
```objectivec
@property (nonatomic, assign) int64_t bytes_sent_per_second;
```
每秒发送字节数，单位字节


<span id="VEWTNRTCVideoStats-bytes_received_per_second"></span>
### bytes_received_per_second
```objectivec
@property (nonatomic, assign) int64_t bytes_received_per_second;
```
每秒接收字节数，单位字节


<span id="VEWTNRTCVideoStats-frame_width"></span>
### frame_width
```objectivec
@property (nonatomic, assign) int64_t frame_width;
```
视频帧宽度。单位：px。


<span id="VEWTNRTCVideoStats-frame_height"></span>
### frame_height
```objectivec
@property (nonatomic, assign) int64_t frame_height;
```
视频帧高度。单位：px。


<span id="VEWTNRTCVideoStats-frames_per_second"></span>
### frames_per_second
```objectivec
@property (nonatomic, assign) double frames_per_second;
```
帧率


<span id="VEWTNRTCVideoStats-codec_id"></span>
### codec_id
```objectivec
@property (nonatomic, strong) NSString* codec_id;
```
编码器 ID


<span id="VEWTNRTCVideoStats-round_trip_time_measurements"></span>
### round_trip_time_measurements
```objectivec
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
```
收到的 RTCP RR 包的总个数。


<span id="VEWTNRTCVideoStats-total_round_trip_time"></span>
### total_round_trip_time
```objectivec
@property (nonatomic, assign) double total_round_trip_time;
```
总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。


<span id="VEWTNRTCVideoStats-packets_sent"></span>
### packets_sent
```objectivec
@property (nonatomic, assign) uint32_t packets_sent;
```
发送包个数


<span id="VEWTNRTCVideoStats-packets_received"></span>
### packets_received
```objectivec
@property (nonatomic, assign) uint32_t packets_received;
```
接收包个数


<span id="VEWTNRTCVideoStats-packets_lost"></span>
### packets_lost
```objectivec
@property (nonatomic, assign) uint32_t packets_lost;
```
发布过程中的丢包数


<span id="VEWTNRTCVideoStats-franction_loss"></span>
### franction_loss
```objectivec
@property (nonatomic, assign) double franction_loss;
```
丢包率。取值范围：`[0, 1.0]`。


<span id="VEWTNRTCVideoStats-jitter"></span>
### jitter
```objectivec
@property (nonatomic, assign) double jitter;
```
jitter 抖动。单位：秒。



