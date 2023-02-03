# VEWTNPubAudioSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNPubAudioSource)
```

The type of the audio source to publish.


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNPubAudioSourceInternalRecord | 0 | The data recorded by built-in microphone. Default value. |
| VEWTNPubAudioSourcePushExternalPCM | 1 | PCM data pushed externally. |


# VEWTNPubVideoSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNPubVideoSource)
```

The type of the video source to publish.


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNPubVideoSourceInternalCamera | 0 | The data recorded by built-in camera. Default value. |
| VEWTNPubVideoSourcePushExternalYUV | 1 | YUV data pushed externally. |
| VEWTNPubVideoSourcePushExternalH264 | 2 | H.264 encoded data pushed externally. |


# VEWTNSubAudioSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNSubAudioSource)
```

The audio source type to subscribe.


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNSubAudioSourceInternalPlay | 0 | Internally captured audio data. Default value. |
| VEWTNSubAudioSourceCallbackPCM | 1 | Audio data in PCM format. |


# VEWTNSubVideoSource
```objectivec
typedef NS_ENUM(NSInteger, VEWTNSubVideoSource)
```

The video source type to subscribe.


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNSubVideoSourceInternalRender | 0 | Internally captured video data. Default value. |
| VEWTNSubVideoSourceCallbackYUV | 1 | Video stream in YUV format. |
| VEWTNSubVideoSourceCallbackH264 | 2 | Video stream encoded in H.264 protocal. |


# VEWTNEncodedVideoFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNEncodedVideoFrameType)
```

The encoding type of the encoded video frame


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNCodceH264 | 0 | H.264 |


# VEWTNVideoFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNVideoFrameType)
```

Video frame type


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNYuvI420 | 0 | YUV-I420 |


# VEWTNAudioFrameType
```objectivec
typedef NS_ENUM(NSInteger, VEWTNAudioFrameType)
```

Audio frame type


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNPcm16 | 0 | PCM 16 |


# VEWTNState
```objectivec
typedef NS_ENUM(NSInteger, VEWTNState)
```

state


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNStateStartSuccess | 0 | Success to start publishing/subscribing the stream. |
| VEWTNStateStartFailed | 1 | Failure to start publishing/subscribing the stream. |
| VEWTNStateStopSuccess | 2 | Success to stop publishing/subscribing the stream. |
| VEWTNStateStopFailed | 3 | Failure to stop publishing/subscribing the stream. |


# VEWTNReason
```objectivec
typedef NS_ENUM(NSInteger, VEWTNReason)
```

reason


### Enumerated Value

| Type | Value | Instructions |
| --- | --- | --- |
| VEWTNReasonSuccess | 0 | Success |
| VEWTNReasonParameterError | 1 | Failure. Invalid parameter. |
| VEWTNReasonTokenError | 2 | Failure. Token error, including invalid token, expired token and token authentication failed. |
| VEWTNReasonResourcesNotFound | 3 | Failure. Resources not found. |
| VEWTNReasonServerInternalError | 4 | Failure. Internal server error. Please try again. |
| VEWTNReasonSDKInternalError | 5 | Failure. Internal SDK error. Please try again. |
| VEWTNReasonNetworkError | 6 | Failure. Network error. |
| VEWTNReasonUnknowError | 7 | Failure. Unknown error. Please try again. |
| VEWTNReasonStreamNotExist | 8 | Failure. Stream not exist. |


# VEWTNPubExtraParam
```objectivec
@interface VEWTNPubExtraParam : NSObject
```

publish param


## Member Property

| Type | Name |
| --- | --- |
| **VEWTNPubAudioSource** | [pub_audio_source](#VEWTNPubExtraParam-pub_audio_source) |
| **VEWTNPubVideoSource** | [pub_video_source](#VEWTNPubExtraParam-pub_video_source) |


## Instructions
<span id="VEWTNPubExtraParam-pub_audio_source"></span>
### pub_audio_source
```objectivec
@property (nonatomic, assign) VEWTNPubAudioSource pub_audio_source;
```
Publish audio source


<span id="VEWTNPubExtraParam-pub_video_source"></span>
### pub_video_source
```objectivec
@property (nonatomic, assign) VEWTNPubVideoSource pub_video_source;
```
Publish video source



# VEWTNSubExtraParam
```objectivec
@interface VEWTNSubExtraParam: NSObject
```

Extra param for subscribing


## Member Property

| Type | Name |
| --- | --- |
| **VEWTNSubAudioSource** | [sub_audio_source](#VEWTNSubExtraParam-sub_audio_source) |
| **VEWTNSubVideoSource** | [sub_video_source](#VEWTNSubExtraParam-sub_video_source) |


## Instructions
<span id="VEWTNSubExtraParam-sub_audio_source"></span>
### sub_audio_source
```objectivec
@property (nonatomic, assign) VEWTNSubAudioSource sub_audio_source;
```
Subscribe audio source. See [VEWTNSubAudioSource](#vewtnsubaudiosource).


<span id="VEWTNSubExtraParam-sub_video_source"></span>
### sub_video_source
```objectivec
@property (nonatomic, assign) VEWTNSubVideoSource sub_video_source;
```
Subscribe video source. See [VEWTNSubVideoSource](#vewtnsubvideosource).



# VEWTNEncodedVideoFrame
```objectivec
@interface VEWTNEncodedVideoFrame: NSObject
```

Encoded video frame struct


## Member Property

| Type | Name |
| --- | --- |
| **VEWTNEncodedVideoFrameType** | [codec_type](#VEWTNEncodedVideoFrame-codec_type) |
| **const uint8_t*** | [data](#VEWTNEncodedVideoFrame-data) |
| **int** | [size](#VEWTNEncodedVideoFrame-size) |
| **int** | [width](#VEWTNEncodedVideoFrame-width) |
| **int** | [height](#VEWTNEncodedVideoFrame-height) |
| **unsigned** | [timestamp_ms](#VEWTNEncodedVideoFrame-timestamp_ms) |
| **int** | [rotation](#VEWTNEncodedVideoFrame-rotation) |
| **BOOL** | [is_key_frame](#VEWTNEncodedVideoFrame-is_key_frame) |


## Instructions
<span id="VEWTNEncodedVideoFrame-codec_type"></span>
### codec_type
```objectivec
@property (nonatomic, assign) VEWTNEncodedVideoFrameType codec_type;
```
Frame type. Only H264 - Annex B supported.


<span id="VEWTNEncodedVideoFrame-data"></span>
### data
```objectivec
@property (nonatomic, assign) const uint8_t * data;
```
data buffer


<span id="VEWTNEncodedVideoFrame-size"></span>
### size
```objectivec
@property (nonatomic, assign) int size;
```
data buffer size


<span id="VEWTNEncodedVideoFrame-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
Width in px


<span id="VEWTNEncodedVideoFrame-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
Height in px


<span id="VEWTNEncodedVideoFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
Timestamp in ms


<span id="VEWTNEncodedVideoFrame-rotation"></span>
### rotation
```objectivec
@property (nonatomic, assign) int rotation;
```
Counterclockwise rotation in degrees. Options are 0, 90, 180, 270。


<span id="VEWTNEncodedVideoFrame-is_key_frame"></span>
### is_key_frame
```objectivec
@property (nonatomic, assign) BOOL is_key_frame;
```
key frame or not. If it is a key frame, it must contain SPS and PPS information.



# VEWTNVideoFrame
```objectivec
@interface VEWTNVideoFrame: NSObject
```

Video frame struct


## Member Property

| Type | Name |
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


## Instructions
<span id="VEWTNVideoFrame-type"></span>
### type
```objectivec
@property (nonatomic, assign) VEWTNVideoFrameType type;
```
Frame type. Only YUV I420 supported.


<span id="VEWTNVideoFrame-y_buffer"></span>
### y_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * y_buffer;
```
Y buffer


<span id="VEWTNVideoFrame-u_buffer"></span>
### u_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * u_buffer;
```
U buffer


<span id="VEWTNVideoFrame-v_buffer"></span>
### v_buffer
```objectivec
@property (nonatomic, assign) const uint8_t * v_buffer;
```
V buffer


<span id="VEWTNVideoFrame-y_stride"></span>
### y_stride
```objectivec
@property (nonatomic, assign) int y_stride;
```
Y stride


<span id="VEWTNVideoFrame-u_stride"></span>
### u_stride
```objectivec
@property (nonatomic, assign) int u_stride;
```
U stride


<span id="VEWTNVideoFrame-v_stride"></span>
### v_stride
```objectivec
@property (nonatomic, assign) int v_stride;
```
V stride


<span id="VEWTNVideoFrame-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
Width in px


<span id="VEWTNVideoFrame-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
Height in px


<span id="VEWTNVideoFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
Timestamp in ms


<span id="VEWTNVideoFrame-rotation"></span>
### rotation
```objectivec
@property (nonatomic, assign) int rotation;
```
Counterclockwise rotation. The options are 0, 90, 180, and 270.



# VEWTNAudioFrame
```objectivec
@interface VEWTNAudioFrame: NSObject
```

Audio frame


## Member Property

| Type | Name |
| --- | --- |
| **VEWTNAudioFrameType** | [type](#VEWTNAudioFrame-type) |
| **const uint8_t*** | [data](#VEWTNAudioFrame-data) |
| **int** | [sample_length](#VEWTNAudioFrame-sample_length) |
| **unsigned** | [timestamp_ms](#VEWTNAudioFrame-timestamp_ms) |
| **int** | [sample_rate](#VEWTNAudioFrame-sample_rate) |
| **int** | [channels](#VEWTNAudioFrame-channels) |


## Instructions
<span id="VEWTNAudioFrame-type"></span>
### type
```objectivec
@property (nonatomic, assign) VEWTNAudioFrameType type;
```
Frame type. Only PCM16 support.


<span id="VEWTNAudioFrame-data"></span>
### data
```objectivec
@property (nonatomic, assign) const uint8_t * data;
```
data buffer


<span id="VEWTNAudioFrame-sample_length"></span>
### sample_length
```objectivec
@property (nonatomic, assign) int sample_length;
```
data buffer size


<span id="VEWTNAudioFrame-timestamp_ms"></span>
### timestamp_ms
```objectivec
@property (nonatomic, assign) unsigned long timestamp_ms;
```
Timestamp in ms


<span id="VEWTNAudioFrame-sample_rate"></span>
### sample_rate
```objectivec
@property (nonatomic, assign) int sample_rate;
```
SampleRate


<span id="VEWTNAudioFrame-channels"></span>
### channels
```objectivec
@property (nonatomic, assign) int channels;
```
Number of channels



# VEWTNVideoConfig
```objectivec
@interface VEWTNVideoConfig: NSObject
```

Video config


## Member Property

| Type | Name |
| --- | --- |
| **int** | [width](#VEWTNVideoConfig-width) |
| **int** | [height](#VEWTNVideoConfig-height) |
| **int** | [max_fps](#VEWTNVideoConfig-max_fps) |


## Instructions
<span id="VEWTNVideoConfig-width"></span>
### width
```objectivec
@property (nonatomic, assign) int width;
```
Width in px. 640 by default.


<span id="VEWTNVideoConfig-height"></span>
### height
```objectivec
@property (nonatomic, assign) int height;
```
Height in px. 960 by default.


<span id="VEWTNVideoConfig-max_fps"></span>
### max_fps
```objectivec
@property (nonatomic, assign) int max_fps;
```
Maximum fps. 15 by default.



# VEWTNStallInfo
```objectivec
@interface VEWTNStallInfo: NSObject
```

Video stall info


## Member Property

| Type | Name |
| --- | --- |
| **int** | [stall_count_500](#VEWTNStallInfo-stall_count_500) |
| **int** | [stall_duration_500](#VEWTNStallInfo-stall_duration_500) |
| **int** | [stall_count_200](#VEWTNStallInfo-stall_count_200) |
| **int** | [stall_duration_200](#VEWTNStallInfo-stall_duration_200) |
| **int** | [frame_rate](#VEWTNStallInfo-frame_rate) |


## Instructions
<span id="VEWTNStallInfo-stall_count_500"></span>
### stall_count_500
```objectivec
@property (nonatomic, assign) int stall_count_500;
```
Stall count in 500ms


<span id="VEWTNStallInfo-stall_duration_500"></span>
### stall_duration_500
```objectivec
@property (nonatomic, assign) int stall_duration_500;
```
Stall duration in 500ms


<span id="VEWTNStallInfo-stall_count_200"></span>
### stall_count_200
```objectivec
@property (nonatomic, assign) int stall_count_200;
```
Stall count in 200ms


<span id="VEWTNStallInfo-stall_duration_200"></span>
### stall_duration_200
```objectivec
@property (nonatomic, assign) int stall_duration_200;
```
Stall duration in 200ms


<span id="VEWTNStallInfo-frame_rate"></span>
### frame_rate
```objectivec
@property (nonatomic, assign) int frame_rate;
```
Frame rate



# VEWTNRTCAudioStats
```objectivec
@interface VEWTNRTCAudioStats: NSObject
```

Audio stats


## Member Property

| Type | Name |
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


## Instructions
<span id="VEWTNRTCAudioStats-bytes_sent"></span>
### bytes_sent
```objectivec
@property (nonatomic, assign) int64_t bytes_sent;
```
Number of bytes sent.


<span id="VEWTNRTCAudioStats-bytes_received"></span>
### bytes_received
```objectivec
@property (nonatomic, assign) int64_t bytes_received;
```
Number of bytes received.


<span id="VEWTNRTCAudioStats-bytes_sent_per_second"></span>
### bytes_sent_per_second
```objectivec
@property (nonatomic, assign) double bytes_sent_per_second;
```
Number of bytes sent per second.


<span id="VEWTNRTCAudioStats-bytes_received_per_second"></span>
### bytes_received_per_second
```objectivec
@property (nonatomic, assign) double bytes_received_per_second;
```
Number of bytes received per second.


<span id="VEWTNRTCAudioStats-audio_level"></span>
### audio_level
```objectivec
@property (nonatomic, assign) double audio_level;
```
audio level. <br>
The range is `[0, 1.0]`. When the value is `1.0`, the sound pressure level is 0dBov; when the value is `0.5`, the sound pressure level is 6dB SPL.


<span id="VEWTNRTCAudioStats-total_audio_energy"></span>
### total_audio_energy
```objectivec
@property (nonatomic, assign) double total_audio_energy;
```
Total audio energy. Higher the value, bigger the total energy.


<span id="VEWTNRTCAudioStats-codec_id"></span>
### codec_id
```objectivec
@property (nonatomic, strong) NSString* codec_id;
```
codec ID


<span id="VEWTNRTCAudioStats-round_trip_time_measurements"></span>
### round_trip_time_measurements
```objectivec
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
```
The total number of the received RTCP RR packets.


<span id="VEWTNRTCAudioStats-total_round_trip_time"></span>
### total_round_trip_time
```objectivec
@property (nonatomic, assign) double total_round_trip_time;
```
Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.


<span id="VEWTNRTCAudioStats-packets_sent"></span>
### packets_sent
```objectivec
@property (nonatomic, assign) uint32_t packets_sent;
```
packets sent


<span id="VEWTNRTCAudioStats-packets_received"></span>
### packets_received
```objectivec
@property (nonatomic, assign) uint32_t packets_received;
```
packets received


<span id="VEWTNRTCAudioStats-packets_lost"></span>
### packets_lost
```objectivec
@property (nonatomic, assign) uint32_t packets_lost;
```
Packets lost during publishing.


<span id="VEWTNRTCAudioStats-franction_loss"></span>
### franction_loss
```objectivec
@property (nonatomic, assign) double franction_loss;
```
Packet lost rate during publishing. The range is `[0, 1.0]`.


<span id="VEWTNRTCAudioStats-jitter"></span>
### jitter
```objectivec
@property (nonatomic, assign) double jitter;
```
jitter in seconds.



# VEWTNRTCVideoStats
```objectivec
@interface VEWTNRTCVideoStats: NSObject
```

Video stats


## Member Property

| Type | Name |
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


## Instructions
<span id="VEWTNRTCVideoStats-bytes_sent"></span>
### bytes_sent
```objectivec
@property (nonatomic, assign) int64_t bytes_sent;
```
Number of bytes sent, in bytes


<span id="VEWTNRTCVideoStats-bytes_received"></span>
### bytes_received
```objectivec
@property (nonatomic, assign) int64_t bytes_received;
```
Number of bytes received, in bytes


<span id="VEWTNRTCVideoStats-bytes_sent_per_second"></span>
### bytes_sent_per_second
```objectivec
@property (nonatomic, assign) int64_t bytes_sent_per_second;
```
Number of bytes sent per second, in bytes


<span id="VEWTNRTCVideoStats-bytes_received_per_second"></span>
### bytes_received_per_second
```objectivec
@property (nonatomic, assign) int64_t bytes_received_per_second;
```
Number of bytes received per second, in bytes


<span id="VEWTNRTCVideoStats-frame_width"></span>
### frame_width
```objectivec
@property (nonatomic, assign) int64_t frame_width;
```
Video frame width in px.


<span id="VEWTNRTCVideoStats-frame_height"></span>
### frame_height
```objectivec
@property (nonatomic, assign) int64_t frame_height;
```
Video frame height in px.


<span id="VEWTNRTCVideoStats-frames_per_second"></span>
### frames_per_second
```objectivec
@property (nonatomic, assign) double frames_per_second;
```
frames per second


<span id="VEWTNRTCVideoStats-codec_id"></span>
### codec_id
```objectivec
@property (nonatomic, strong) NSString* codec_id;
```
codec ID


<span id="VEWTNRTCVideoStats-round_trip_time_measurements"></span>
### round_trip_time_measurements
```objectivec
@property (nonatomic, assign) uint64_t round_trip_time_measurements;
```
The total number of the received RTCP RR packets.


<span id="VEWTNRTCVideoStats-total_round_trip_time"></span>
### total_round_trip_time
```objectivec
@property (nonatomic, assign) double total_round_trip_time;
```
Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.


<span id="VEWTNRTCVideoStats-packets_sent"></span>
### packets_sent
```objectivec
@property (nonatomic, assign) uint32_t packets_sent;
```
packets sent


<span id="VEWTNRTCVideoStats-packets_received"></span>
### packets_received
```objectivec
@property (nonatomic, assign) uint32_t packets_received;
```
packets received


<span id="VEWTNRTCVideoStats-packets_lost"></span>
### packets_lost
```objectivec
@property (nonatomic, assign) uint32_t packets_lost;
```
Packets lost during publishing.


<span id="VEWTNRTCVideoStats-franction_loss"></span>
### franction_loss
```objectivec
@property (nonatomic, assign) double franction_loss;
```
Packet lost rate during publishing. The range is `[0, 1.0]`.


<span id="VEWTNRTCVideoStats-jitter"></span>
### jitter
```objectivec
@property (nonatomic, assign) double jitter;
```
jitter in seconds.



