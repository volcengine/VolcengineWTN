# AudioFrame
```java
public class com.vewtn.AudioFrame
```

音频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **RtcDefines.AudioFrameType** | [type](#AudioFrame-type) |
| **byte[]** | [data](#AudioFrame-data) |
| **int** | [sampleLength](#AudioFrame-samplelength) |
| **long** | [timestampMs](#AudioFrame-timestampms) |
| **int** | [sampleRate](#AudioFrame-samplerate) |
| **int** | [channels](#AudioFrame-channels) |


## 变量说明
<span id="AudioFrame-type"></span>
### type
```java
public RtcDefines.AudioFrameType com.vewtn.AudioFrame.type = RtcDefines.AudioFrameType.Pcm16;
```
帧类型。目前仅支持 PCM16。


<span id="AudioFrame-data"></span>
### data
```java
public byte[] com.vewtn.AudioFrame.data;
```
数据缓存


<span id="AudioFrame-samplelength"></span>
### sampleLength
```java
public int com.vewtn.AudioFrame.sampleLength;
```
数据缓存大小


<span id="AudioFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.AudioFrame.timestampMs;
```
时间戳，单位：毫秒。


<span id="AudioFrame-samplerate"></span>
### sampleRate
```java
public int com.vewtn.AudioFrame.sampleRate;
```
采样率


<span id="AudioFrame-channels"></span>
### channels
```java
public int com.vewtn.AudioFrame.channels;
```
通道数



# EncodedVideoFrame
```java
public class com.vewtn.EncodedVideoFrame
```

编码后的视频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **RtcDefines.EncodedVideoFrameType** | [codecType](#EncodedVideoFrame-codectype) |
| **byte[]** | [data](#EncodedVideoFrame-data) |
| **int** | [size](#EncodedVideoFrame-size) |
| **int** | [width](#EncodedVideoFrame-width) |
| **int** | [height](#EncodedVideoFrame-height) |
| **long** | [timestampMs](#EncodedVideoFrame-timestampms) |
| **int** | [rotation](#EncodedVideoFrame-rotation) |
| **boolean** | [isKeyFrame](#EncodedVideoFrame-iskeyframe) |


## 变量说明
<span id="EncodedVideoFrame-codectype"></span>
### codecType
```java
public RtcDefines.EncodedVideoFrameType com.vewtn.EncodedVideoFrame.codecType = RtcDefines.EncodedVideoFrameType.CodecH2/;"
```
帧类型。目前仅支持 H.264，且必须是 Annex B 格式。


<span id="EncodedVideoFrame-data"></span>
### data
```java
public byte[] com.vewtn.EncodedVideoFrame.data;
```
数据缓存


<span id="EncodedVideoFrame-size"></span>
### size
```java
public int com.vewtn.EncodedVideoFrame.size;
```
数据缓存大小


<span id="EncodedVideoFrame-width"></span>
### width
```java
public int com.vewtn.EncodedVideoFrame.width;
```
宽度（px）


<span id="EncodedVideoFrame-height"></span>
### height
```java
public int com.vewtn.EncodedVideoFrame.height;
```
高度（px）


<span id="EncodedVideoFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.EncodedVideoFrame.timestampMs;
```
时间戳，单位毫秒


<span id="EncodedVideoFrame-rotation"></span>
### rotation
```java
public int com.vewtn.EncodedVideoFrame.rotation;
```
逆时针旋转角度，单位为度。可选值为 0，90，180，270。


<span id="EncodedVideoFrame-iskeyframe"></span>
### isKeyFrame
```java
public boolean com.vewtn.EncodedVideoFrame.isKeyFrame;
```
是否为关键帧。如果是关键帧，那么，必须包含 SPS 和 PPS 信息。



# PubExtraParam
```java
public class com.vewtn.PubExtraParam
```

发布参数


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **RtcDefines.PubAudioSource** | [pubAudioSource](#PubExtraParam-pubaudiosource) |
| **RtcDefines.PubVideoSource** | [pubVideoSource](#PubExtraParam-pubvideosource) |


## 变量说明
<span id="PubExtraParam-pubaudiosource"></span>
### pubAudioSource
```java
public RtcDefines.PubAudioSource com.vewtn.PubExtraParam.pubAudioSource = RtcDefines.PubAudioSource.InternalRecord;
```
音频源发布


<span id="PubExtraParam-pubvideosource"></span>
### pubVideoSource
```java
public RtcDefines.PubVideoSource com.vewtn.PubExtraParam.pubVideoSource = RtcDefines.PubVideoSource.InternalCamera;
```
视频源发布



# RTCAudioStats
```java
public class com.vewtn.RTCAudioStats
```

音频流数据


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **long** | [bytesSent](#RTCAudioStats-bytessent) |
| **long** | [bytesReceived](#RTCAudioStats-bytesreceived) |
| **double** | [bytesSentPerSecond](#RTCAudioStats-bytessentpersecond) |
| **double** | [bytesReceivedPerSecond](#RTCAudioStats-bytesreceivedpersecond) |
| **double** | [audioLevel](#RTCAudioStats-audiolevel) |
| **double** | [totalAudioEnergy](#RTCAudioStats-totalaudioenergy) |
| **long** | [roundTripTimeMeasurements](#RTCAudioStats-roundtriptimemeasurements) |
| **double** | [totalRoundTripTime](#RTCAudioStats-totalroundtriptime) |
| **long** | [packetsSent](#RTCAudioStats-packetssent) |
| **long** | [packetsReceived](#RTCAudioStats-packetsreceived) |
| **long** | [packetsLost](#RTCAudioStats-packetslost) |
| **double** | [franctionLoss](#RTCAudioStats-franctionloss) |
| **double** | [jitter](#RTCAudioStats-jitter) |


## 变量说明
<span id="RTCAudioStats-bytessent"></span>
### bytesSent
```java
public long com.vewtn.RTCAudioStats.bytesSent;
```
发送字节数，单位：字节。


<span id="RTCAudioStats-bytesreceived"></span>
### bytesReceived
```java
public long com.vewtn.RTCAudioStats.bytesReceived;
```
接收字节数，单位：字节。


<span id="RTCAudioStats-bytessentpersecond"></span>
### bytesSentPerSecond
```java
public double com.vewtn.RTCAudioStats.bytesSentPerSecond;
```
每秒发送字节数，单位：字节。


<span id="RTCAudioStats-bytesreceivedpersecond"></span>
### bytesReceivedPerSecond
```java
public double com.vewtn.RTCAudioStats.bytesReceivedPerSecond;
```
每秒接收字节数，单位：字节。


<span id="RTCAudioStats-audiolevel"></span>
### audioLevel
```java
public double com.vewtn.RTCAudioStats.audioLevel;
```
音量强度。<br>
取值范围：`[0, 1.0]`。值为 `1.0` 时，表示 0dBov；值为 `0.5` 时，表示 6dB SPL。


<span id="RTCAudioStats-totalaudioenergy"></span>
### totalAudioEnergy
```java
public double com.vewtn.RTCAudioStats.totalAudioEnergy;
```
总音量强度。值越大，强度越高。


<span id="RTCAudioStats-roundtriptimemeasurements"></span>
### roundTripTimeMeasurements
```java
public long com.vewtn.RTCAudioStats.roundTripTimeMeasurements;
```
收到的 RTCP RR 包的总个数。


<span id="RTCAudioStats-totalroundtriptime"></span>
### totalRoundTripTime
```java
public double com.vewtn.RTCAudioStats.totalRoundTripTime;
```
总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。


<span id="RTCAudioStats-packetssent"></span>
### packetsSent
```java
public long com.vewtn.RTCAudioStats.packetsSent;
```
发送包个数


<span id="RTCAudioStats-packetsreceived"></span>
### packetsReceived
```java
public long com.vewtn.RTCAudioStats.packetsReceived;
```
接收包个数


<span id="RTCAudioStats-packetslost"></span>
### packetsLost
```java
public long com.vewtn.RTCAudioStats.packetsLost;
```
发布过程中的丢包数


<span id="RTCAudioStats-franctionloss"></span>
### franctionLoss
```java
public double com.vewtn.RTCAudioStats.franctionLoss;
```
丢包率。取值范围：`[0, 1.0]`。


<span id="RTCAudioStats-jitter"></span>
### jitter
```java
public double com.vewtn.RTCAudioStats.jitter;
```
jitter 抖动。单位：秒。



# RTCVideoStats
```java
public class com.vewtn.RTCVideoStats
```

视频流数据


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **long** | [bytesSent](#RTCVideoStats-bytessent) |
| **long** | [bytesReceived](#RTCVideoStats-bytesreceived) |
| **double** | [bytesSentPerSecond](#RTCVideoStats-bytessentpersecond) |
| **double** | [bytesReceivedPerSecond](#RTCVideoStats-bytesreceivedpersecond) |
| **long** | [frameWidth](#RTCVideoStats-framewidth) |
| **long** | [frameHeight](#RTCVideoStats-frameheight) |
| **long** | [framesPerSecond](#RTCVideoStats-framespersecond) |
| **long** | [roundTripTimeMeasurements](#RTCVideoStats-roundtriptimemeasurements) |
| **double** | [totalRoundTripTime](#RTCVideoStats-totalroundtriptime) |
| **long** | [packetsSent](#RTCVideoStats-packetssent) |
| **long** | [packetsReceived](#RTCVideoStats-packetsreceived) |
| **long** | [packetsLost](#RTCVideoStats-packetslost) |
| **double** | [franctionLoss](#RTCVideoStats-franctionloss) |
| **double** | [jitter](#RTCVideoStats-jitter) |


## 变量说明
<span id="RTCVideoStats-bytessent"></span>
### bytesSent
```java
public long com.vewtn.RTCVideoStats.bytesSent;
```
发送字节数，单位字节


<span id="RTCVideoStats-bytesreceived"></span>
### bytesReceived
```java
public long com.vewtn.RTCVideoStats.bytesReceived;
```
接收字节数，单位字节


<span id="RTCVideoStats-bytessentpersecond"></span>
### bytesSentPerSecond
```java
public double com.vewtn.RTCVideoStats.bytesSentPerSecond;
```
每秒发送字节数，单位字节


<span id="RTCVideoStats-bytesreceivedpersecond"></span>
### bytesReceivedPerSecond
```java
public double com.vewtn.RTCVideoStats.bytesReceivedPerSecond;
```
每秒接收字节数，单位字节


<span id="RTCVideoStats-framewidth"></span>
### frameWidth
```java
public long com.vewtn.RTCVideoStats.frameWidth;
```
视频帧宽度。单位：px。


<span id="RTCVideoStats-frameheight"></span>
### frameHeight
```java
public long com.vewtn.RTCVideoStats.frameHeight;
```
视频帧高度。单位：px。


<span id="RTCVideoStats-framespersecond"></span>
### framesPerSecond
```java
public long com.vewtn.RTCVideoStats.framesPerSecond;
```
帧率


<span id="RTCVideoStats-roundtriptimemeasurements"></span>
### roundTripTimeMeasurements
```java
public long com.vewtn.RTCVideoStats.roundTripTimeMeasurements;
```
收到的 RTCP RR 包的总个数。


<span id="RTCVideoStats-totalroundtriptime"></span>
### totalRoundTripTime
```java
public double com.vewtn.RTCVideoStats.totalRoundTripTime;
```
总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。


<span id="RTCVideoStats-packetssent"></span>
### packetsSent
```java
public long com.vewtn.RTCVideoStats.packetsSent;
```
发送包个数


<span id="RTCVideoStats-packetsreceived"></span>
### packetsReceived
```java
public long com.vewtn.RTCVideoStats.packetsReceived;
```
接收包个数


<span id="RTCVideoStats-packetslost"></span>
### packetsLost
```java
public long com.vewtn.RTCVideoStats.packetsLost;
```
发布过程中的丢包数


<span id="RTCVideoStats-franctionloss"></span>
### franctionLoss
```java
public double com.vewtn.RTCVideoStats.franctionLoss;
```
丢包率。取值范围：`[0, 1.0]`。


<span id="RTCVideoStats-jitter"></span>
### jitter
```java
public double com.vewtn.RTCVideoStats.jitter;
```
jitter 抖动。单位：秒。



# RtcDefines
```java
public class com.vewtn.RtcDefines
```



# PubAudioSource
```java
public enum com.vewtn.RtcDefines.PubAudioSource
```

发布的音频源类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **InternalRecord** | 内部麦克风采集的数据。默认值。 |
| **PushExternalPCM** | 推送的外部 PCM 数据 |


# PubVideoSource
```java
public enum com.vewtn.RtcDefines.PubVideoSource
```

发布的视频源类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **InternalCamera** | 内部摄像头采集的数据。默认值。 |
| **PushExternalYUV** | 推送的外部 YUV 数据 |
| **PushExternalH264** | 推送的外部 H.264 编码的数据 |


# SubAudioSource
```java
public enum com.vewtn.RtcDefines.SubAudioSource
```

订阅的音频源类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **InternalPlay** | 内部采集的音频数据。默认值。 |
| **CallbackPCM** | PCM 音频数据。 |


# SubVideoSource
```java
public enum com.vewtn.RtcDefines.SubVideoSource
```

订阅的视频源类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **InternalRender** | 内部采集的视频数据。默认值。 |
| **CallbackYUV** | YUV 格式的视频数据。 |
| **CallbackH264** | H.264 格式封装的视频数据。 |


# EncodedVideoFrameType
```java
public enum com.vewtn.RtcDefines.EncodedVideoFrameType
```

外部编码视频帧的编码类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **CodecH264** | H.264 |


# VideoFrameType
```java
public enum com.vewtn.RtcDefines.VideoFrameType
```

视频帧类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **YuvI420** | YUV-I420 |


# AudioFrameType
```java
public enum com.vewtn.RtcDefines.AudioFrameType
```

音频帧类型


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **Pcm16** | PCM 16 |


# State
```java
public enum com.vewtn.RtcDefines.State
```

音视频流状态


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **StartSuccess** | 开始发布/订阅成功 |
| **StartFailed** | 开始发布/订阅失败 |
| **StopSuccess** | 停止发布/订阅成功 |
| **StopFailed** | 停止发布/订阅失败 |
| **}** |  |


# Reason
```java
public enum com.vewtn.RtcDefines.Reason
```

发布成功或失败的原因


## 枚举值

| 类型 | 说明 |
| --- | --- |
| **Success** | 成功 |
| **ParameterError** | 失败。参数错误。 |
| **TokenError** | 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。 |
| **ResourcesNotFound** | 失败。未找到资源。 |
| **ServerInternalError** | 失败。服务器内部错误。请重试。 |
| **SDKInternalError** | 失败。SDK 内部错误。请重试。 |
| **NetworkError** | 失败。网络错误。 |
| **UnknowError** | 失败。未知错误。请重试。 |
| **StreamNotExist** | 失败。订阅的流不存在。 |
| **DeviceNoPermission** | 失败。没有设备权限（摄像头 or 麦克风）。 |


# StallInfo
```java
public class com.vewtn.StallInfo
```

视频卡顿信息


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [stallCount500](#StallInfo-stallcount500) |
| **int** | [stallDuration500](#StallInfo-stallduration500) |
| **int** | [stallCount200](#StallInfo-stallcount200) |
| **int** | [stallDuration200](#StallInfo-stallduration200) |
| **int** | [frameRate](#StallInfo-framerate) |


## 变量说明
<span id="StallInfo-stallcount500"></span>
### stallCount500
```java
public int com.vewtn.StallInfo.stallCount500;
```
500 毫秒卡顿次数


<span id="StallInfo-stallduration500"></span>
### stallDuration500
```java
public int com.vewtn.StallInfo.stallDuration500;
```
500 毫秒卡顿时长


<span id="StallInfo-stallcount200"></span>
### stallCount200
```java
public int com.vewtn.StallInfo.stallCount200;
```
200 毫秒卡顿次数


<span id="StallInfo-stallduration200"></span>
### stallDuration200
```java
public int com.vewtn.StallInfo.stallDuration200;
```
200 毫秒卡顿时长


<span id="StallInfo-framerate"></span>
### frameRate
```java
public int com.vewtn.StallInfo.frameRate;
```
帧率



# SubExtraParam
```java
public class com.vewtn.SubExtraParam
```

订阅额外参数


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **RtcDefines.SubAudioSource** | [subAudioSource](#SubExtraParam-subaudiosource) |
| **RtcDefines.SubVideoSource** | [subVideoSource](#SubExtraParam-subvideosource) |


## 变量说明
<span id="SubExtraParam-subaudiosource"></span>
### subAudioSource
```java
public RtcDefines.SubAudioSource com.vewtn.SubExtraParam.subAudioSource = RtcDefines.SubAudioSource.InternalPlay;
```
音频源订阅。参看 [SubAudioSource](#subaudiosource)。


<span id="SubExtraParam-subvideosource"></span>
### subVideoSource
```java
public RtcDefines.SubVideoSource com.vewtn.SubExtraParam.subVideoSource = RtcDefines.SubVideoSource.InternalRender;
```
视频源订阅。参看 [SubVideoSource](#subvideosource)。



# VideoConfig
```java
public class com.vewtn.VideoConfig
```

视频配置


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [width](#VideoConfig-width) |
| **int** | [height](#VideoConfig-height) |
| **int** | [maxFPS](#VideoConfig-maxfps) |


## 变量说明
<span id="VideoConfig-width"></span>
### width
```java
public int com.vewtn.VideoConfig.width = 640;
```
宽度（px）。默认值是 640。


<span id="VideoConfig-height"></span>
### height
```java
public int com.vewtn.VideoConfig.height = 960;
```
高度（px）。默认值是 960。


<span id="VideoConfig-maxfps"></span>
### maxFPS
```java
public int com.vewtn.VideoConfig.maxFPS = 15;
```
最大帧率（fps）。默认值是 15。



# VideoFrame
```java
public class com.vewtn.VideoFrame
```

视频帧


## 成员变量

| 类型 | 名称 |
| --- | --- |
| **RtcDefines.VideoFrameType** | [type](#VideoFrame-type) |
| **int** | [width](#VideoFrame-width) |
| **int** | [height](#VideoFrame-height) |
| **byte[]** | [yBuffer](#VideoFrame-ybuffer) |
| **byte[]** | [uBuffer](#VideoFrame-ubuffer) |
| **byte[]** | [vBuffer](#VideoFrame-vbuffer) |
| **int** | [yStride](#VideoFrame-ystride) |
| **int** | [uStride](#VideoFrame-ustride) |
| **int** | [vStride](#VideoFrame-vstride) |
| **long** | [timestampMs](#VideoFrame-timestampms) |
| **int** | [rotation](#VideoFrame-rotation) |


## 变量说明
<span id="VideoFrame-type"></span>
### type
```java
public RtcDefines.VideoFrameType com.vewtn.VideoFrame.type = RtcDefines.VideoFrameType.YuvI420;
```
帧类型。目前仅支持YUV I420。


<span id="VideoFrame-width"></span>
### width
```java
public int com.vewtn.VideoFrame.width;
```
宽度（px）


<span id="VideoFrame-height"></span>
### height
```java
public int com.vewtn.VideoFrame.height;
```
高度（px）


<span id="VideoFrame-ybuffer"></span>
### yBuffer
```java
public byte[] com.vewtn.VideoFrame.yBuffer;
```
Y 分量数据缓存


<span id="VideoFrame-ubuffer"></span>
### uBuffer
```java
public byte[] com.vewtn.VideoFrame.uBuffer;
```
U 分量数据缓存


<span id="VideoFrame-vbuffer"></span>
### vBuffer
```java
public byte[] com.vewtn.VideoFrame.vBuffer;
```
V 分量数据缓存


<span id="VideoFrame-ystride"></span>
### yStride
```java
public int com.vewtn.VideoFrame.yStride;
```
Y 分量 stride


<span id="VideoFrame-ustride"></span>
### uStride
```java
public int com.vewtn.VideoFrame.uStride;
```
U 分量 stride


<span id="VideoFrame-vstride"></span>
### vStride
```java
public int com.vewtn.VideoFrame.vStride;
```
V 分量 stride


<span id="VideoFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.VideoFrame.timestampMs;
```
时间戳，单位毫秒。


<span id="VideoFrame-rotation"></span>
### rotation
```java
public int com.vewtn.VideoFrame.rotation;
```
逆时针旋转角度，可选值为 0，90，180，270



