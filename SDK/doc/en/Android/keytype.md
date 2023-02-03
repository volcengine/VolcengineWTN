# AudioFrame
```java
public class com.vewtn.AudioFrame
```

Audio frame


## Member Property

| Type | Name |
| --- | --- |
| **RtcDefines.AudioFrameType** | [type](#AudioFrame-type) |
| **byte[]** | [data](#AudioFrame-data) |
| **int** | [sampleLength](#AudioFrame-samplelength) |
| **long** | [timestampMs](#AudioFrame-timestampms) |
| **int** | [sampleRate](#AudioFrame-samplerate) |
| **int** | [channels](#AudioFrame-channels) |


## Instructions
<span id="AudioFrame-type"></span>
### type
```java
public RtcDefines.AudioFrameType com.vewtn.AudioFrame.type = RtcDefines.AudioFrameType.Pcm16;
```
Frame type. Only PCM16 support.


<span id="AudioFrame-data"></span>
### data
```java
public byte[] com.vewtn.AudioFrame.data;
```
data buffer


<span id="AudioFrame-samplelength"></span>
### sampleLength
```java
public int com.vewtn.AudioFrame.sampleLength;
```
data buffer size


<span id="AudioFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.AudioFrame.timestampMs;
```
Timestamp in ms


<span id="AudioFrame-samplerate"></span>
### sampleRate
```java
public int com.vewtn.AudioFrame.sampleRate;
```
SampleRate


<span id="AudioFrame-channels"></span>
### channels
```java
public int com.vewtn.AudioFrame.channels;
```
Number of channels



# EncodedVideoFrame
```java
public class com.vewtn.EncodedVideoFrame
```

Encoded video frame struct


## Member Property

| Type | Name |
| --- | --- |
| **RtcDefines.EncodedVideoFrameType** | [codecType](#EncodedVideoFrame-codectype) |
| **byte[]** | [data](#EncodedVideoFrame-data) |
| **int** | [size](#EncodedVideoFrame-size) |
| **int** | [width](#EncodedVideoFrame-width) |
| **int** | [height](#EncodedVideoFrame-height) |
| **long** | [timestampMs](#EncodedVideoFrame-timestampms) |
| **int** | [rotation](#EncodedVideoFrame-rotation) |
| **boolean** | [isKeyFrame](#EncodedVideoFrame-iskeyframe) |


## Instructions
<span id="EncodedVideoFrame-codectype"></span>
### codecType
```java
public RtcDefines.EncodedVideoFrameType com.vewtn.EncodedVideoFrame.codecType = RtcDefines.EncodedVideoFrameType.CodecH2/;"
```
Frame type. Only H264 - Annex B supported.


<span id="EncodedVideoFrame-data"></span>
### data
```java
public byte[] com.vewtn.EncodedVideoFrame.data;
```
data buffer


<span id="EncodedVideoFrame-size"></span>
### size
```java
public int com.vewtn.EncodedVideoFrame.size;
```
data buffer size


<span id="EncodedVideoFrame-width"></span>
### width
```java
public int com.vewtn.EncodedVideoFrame.width;
```
Width in px


<span id="EncodedVideoFrame-height"></span>
### height
```java
public int com.vewtn.EncodedVideoFrame.height;
```
Height in px


<span id="EncodedVideoFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.EncodedVideoFrame.timestampMs;
```
Timestamp in ms


<span id="EncodedVideoFrame-rotation"></span>
### rotation
```java
public int com.vewtn.EncodedVideoFrame.rotation;
```
Counterclockwise rotation in degrees. Options are 0, 90, 180, 270。


<span id="EncodedVideoFrame-iskeyframe"></span>
### isKeyFrame
```java
public boolean com.vewtn.EncodedVideoFrame.isKeyFrame;
```
key frame or not. If it is a key frame, it must contain SPS and PPS information.



# PubExtraParam
```java
public class com.vewtn.PubExtraParam
```

publish param


## Member Property

| Type | Name |
| --- | --- |
| **RtcDefines.PubAudioSource** | [pubAudioSource](#PubExtraParam-pubaudiosource) |
| **RtcDefines.PubVideoSource** | [pubVideoSource](#PubExtraParam-pubvideosource) |


## Instructions
<span id="PubExtraParam-pubaudiosource"></span>
### pubAudioSource
```java
public RtcDefines.PubAudioSource com.vewtn.PubExtraParam.pubAudioSource = RtcDefines.PubAudioSource.InternalRecord;
```
Publish audio source


<span id="PubExtraParam-pubvideosource"></span>
### pubVideoSource
```java
public RtcDefines.PubVideoSource com.vewtn.PubExtraParam.pubVideoSource = RtcDefines.PubVideoSource.InternalCamera;
```
Publish video source



# RTCAudioStats
```java
public class com.vewtn.RTCAudioStats
```

Audio stats


## Member Property

| Type | Name |
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


## Instructions
<span id="RTCAudioStats-bytessent"></span>
### bytesSent
```java
public long com.vewtn.RTCAudioStats.bytesSent;
```
Number of bytes sent.


<span id="RTCAudioStats-bytesreceived"></span>
### bytesReceived
```java
public long com.vewtn.RTCAudioStats.bytesReceived;
```
Number of bytes received.


<span id="RTCAudioStats-bytessentpersecond"></span>
### bytesSentPerSecond
```java
public double com.vewtn.RTCAudioStats.bytesSentPerSecond;
```
Number of bytes sent per second.


<span id="RTCAudioStats-bytesreceivedpersecond"></span>
### bytesReceivedPerSecond
```java
public double com.vewtn.RTCAudioStats.bytesReceivedPerSecond;
```
Number of bytes received per second.


<span id="RTCAudioStats-audiolevel"></span>
### audioLevel
```java
public double com.vewtn.RTCAudioStats.audioLevel;
```
audio level. <br>
The range is `[0, 1.0]`. When the value is `1.0`, the sound pressure level is 0dBov; when the value is `0.5`, the sound pressure level is 6dB SPL.


<span id="RTCAudioStats-totalaudioenergy"></span>
### totalAudioEnergy
```java
public double com.vewtn.RTCAudioStats.totalAudioEnergy;
```
Total audio energy. Higher the value, bigger the total energy.


<span id="RTCAudioStats-roundtriptimemeasurements"></span>
### roundTripTimeMeasurements
```java
public long com.vewtn.RTCAudioStats.roundTripTimeMeasurements;
```
The total number of the received RTCP RR packets.


<span id="RTCAudioStats-totalroundtriptime"></span>
### totalRoundTripTime
```java
public double com.vewtn.RTCAudioStats.totalRoundTripTime;
```
Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.


<span id="RTCAudioStats-packetssent"></span>
### packetsSent
```java
public long com.vewtn.RTCAudioStats.packetsSent;
```
packets sent


<span id="RTCAudioStats-packetsreceived"></span>
### packetsReceived
```java
public long com.vewtn.RTCAudioStats.packetsReceived;
```
packets received


<span id="RTCAudioStats-packetslost"></span>
### packetsLost
```java
public long com.vewtn.RTCAudioStats.packetsLost;
```
Packets lost during publishing.


<span id="RTCAudioStats-franctionloss"></span>
### franctionLoss
```java
public double com.vewtn.RTCAudioStats.franctionLoss;
```
Packet lost rate during publishing. The range is `[0, 1.0]`.


<span id="RTCAudioStats-jitter"></span>
### jitter
```java
public double com.vewtn.RTCAudioStats.jitter;
```
jitter in seconds.



# RTCVideoStats
```java
public class com.vewtn.RTCVideoStats
```

Video stats


## Member Property

| Type | Name |
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


## Instructions
<span id="RTCVideoStats-bytessent"></span>
### bytesSent
```java
public long com.vewtn.RTCVideoStats.bytesSent;
```
Number of bytes sent, in bytes


<span id="RTCVideoStats-bytesreceived"></span>
### bytesReceived
```java
public long com.vewtn.RTCVideoStats.bytesReceived;
```
Number of bytes received, in bytes


<span id="RTCVideoStats-bytessentpersecond"></span>
### bytesSentPerSecond
```java
public double com.vewtn.RTCVideoStats.bytesSentPerSecond;
```
Number of bytes sent per second, in bytes


<span id="RTCVideoStats-bytesreceivedpersecond"></span>
### bytesReceivedPerSecond
```java
public double com.vewtn.RTCVideoStats.bytesReceivedPerSecond;
```
Number of bytes received per second, in bytes


<span id="RTCVideoStats-framewidth"></span>
### frameWidth
```java
public long com.vewtn.RTCVideoStats.frameWidth;
```
Video frame width in px.


<span id="RTCVideoStats-frameheight"></span>
### frameHeight
```java
public long com.vewtn.RTCVideoStats.frameHeight;
```
Video frame height in px.


<span id="RTCVideoStats-framespersecond"></span>
### framesPerSecond
```java
public long com.vewtn.RTCVideoStats.framesPerSecond;
```
frames per second


<span id="RTCVideoStats-roundtriptimemeasurements"></span>
### roundTripTimeMeasurements
```java
public long com.vewtn.RTCVideoStats.roundTripTimeMeasurements;
```
The total number of the received RTCP RR packets.


<span id="RTCVideoStats-totalroundtriptime"></span>
### totalRoundTripTime
```java
public double com.vewtn.RTCVideoStats.totalRoundTripTime;
```
Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.


<span id="RTCVideoStats-packetssent"></span>
### packetsSent
```java
public long com.vewtn.RTCVideoStats.packetsSent;
```
packets sent


<span id="RTCVideoStats-packetsreceived"></span>
### packetsReceived
```java
public long com.vewtn.RTCVideoStats.packetsReceived;
```
packets received


<span id="RTCVideoStats-packetslost"></span>
### packetsLost
```java
public long com.vewtn.RTCVideoStats.packetsLost;
```
Packets lost during publishing.


<span id="RTCVideoStats-franctionloss"></span>
### franctionLoss
```java
public double com.vewtn.RTCVideoStats.franctionLoss;
```
Packet lost rate during publishing. The range is `[0, 1.0]`.


<span id="RTCVideoStats-jitter"></span>
### jitter
```java
public double com.vewtn.RTCVideoStats.jitter;
```
jitter in seconds.



# RtcDefines
```java
public class com.vewtn.RtcDefines
```



# PubAudioSource
```java
public enum com.vewtn.RtcDefines.PubAudioSource
```

The type of the audio source to publish.


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **InternalRecord** | The data recorded by built-in microphone. Default value. |
| **PushExternalPCM** | PCM data pushed externally. |


# PubVideoSource
```java
public enum com.vewtn.RtcDefines.PubVideoSource
```

The type of the video source to publish.


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **InternalCamera** | The data recorded by built-in camera. Default value. |
| **PushExternalYUV** | YUV data pushed externally. |
| **PushExternalH264** | H.264 encoded data pushed externally. |


# SubAudioSource
```java
public enum com.vewtn.RtcDefines.SubAudioSource
```

The audio source type to subscribe.


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **InternalPlay** | Internally captured audio data. Default value. |
| **CallbackPCM** | Audio data in PCM format. |


# SubVideoSource
```java
public enum com.vewtn.RtcDefines.SubVideoSource
```

The video source type to subscribe.


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **InternalRender** | Internally captured video data. Default value. |
| **CallbackYUV** | Video stream in YUV format. |
| **CallbackH264** | Video stream encoded in H.264 protocal. |


# EncodedVideoFrameType
```java
public enum com.vewtn.RtcDefines.EncodedVideoFrameType
```

The encoding type of the encoded video frame


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **CodecH264** | H.264 |


# VideoFrameType
```java
public enum com.vewtn.RtcDefines.VideoFrameType
```

Video frame type


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **YuvI420** | YUV-I420 |


# AudioFrameType
```java
public enum com.vewtn.RtcDefines.AudioFrameType
```

Audio frame type


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **Pcm16** | PCM 16 |


# State
```java
public enum com.vewtn.RtcDefines.State
```

state


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **StartSuccess** | Success to start publishing/subscribing the stream. |
| **StartFailed** | Failure to start publishing/subscribing the stream. |
| **StopSuccess** | Success to stop publishing/subscribing the stream. |
| **StopFailed** | Failure to stop publishing/subscribing the stream. |
| **}** |  |


# Reason
```java
public enum com.vewtn.RtcDefines.Reason
```

reason


## Enumerated Value

| Type | Instructions |
| --- | --- |
| **Success** | Success |
| **ParameterError** | Failure. Invalid parameter. |
| **TokenError** | Failure. Token error, including invalid token, expired token and token authentication failed. |
| **ResourcesNotFound** | Failure. Resources not found. |
| **ServerInternalError** | Failure. Internal server error. Please try again. |
| **SDKInternalError** | Failure. Internal SDK error. Please try again. |
| **NetworkError** | Failure. Network error. |
| **UnknowError** | Failure. Unknown error. Please try again. |
| **StreamNotExist** | Failure. Stream not exist. |
| **DeviceNoPermission** | Failure. No access to the device (camera or microphone). |


# StallInfo
```java
public class com.vewtn.StallInfo
```

Video stall info


## Member Property

| Type | Name |
| --- | --- |
| **int** | [stallCount500](#StallInfo-stallcount500) |
| **int** | [stallDuration500](#StallInfo-stallduration500) |
| **int** | [stallCount200](#StallInfo-stallcount200) |
| **int** | [stallDuration200](#StallInfo-stallduration200) |
| **int** | [frameRate](#StallInfo-framerate) |


## Instructions
<span id="StallInfo-stallcount500"></span>
### stallCount500
```java
public int com.vewtn.StallInfo.stallCount500;
```
Stall count in 500ms


<span id="StallInfo-stallduration500"></span>
### stallDuration500
```java
public int com.vewtn.StallInfo.stallDuration500;
```
Stall duration in 500ms


<span id="StallInfo-stallcount200"></span>
### stallCount200
```java
public int com.vewtn.StallInfo.stallCount200;
```
Stall count in 200ms


<span id="StallInfo-stallduration200"></span>
### stallDuration200
```java
public int com.vewtn.StallInfo.stallDuration200;
```
Stall duration in 200ms


<span id="StallInfo-framerate"></span>
### frameRate
```java
public int com.vewtn.StallInfo.frameRate;
```
Frame rate



# SubExtraParam
```java
public class com.vewtn.SubExtraParam
```

Extra param for subscribing


## Member Property

| Type | Name |
| --- | --- |
| **RtcDefines.SubAudioSource** | [subAudioSource](#SubExtraParam-subaudiosource) |
| **RtcDefines.SubVideoSource** | [subVideoSource](#SubExtraParam-subvideosource) |


## Instructions
<span id="SubExtraParam-subaudiosource"></span>
### subAudioSource
```java
public RtcDefines.SubAudioSource com.vewtn.SubExtraParam.subAudioSource = RtcDefines.SubAudioSource.InternalPlay;
```
Subscribe audio source. See [SubAudioSource](#subaudiosource).


<span id="SubExtraParam-subvideosource"></span>
### subVideoSource
```java
public RtcDefines.SubVideoSource com.vewtn.SubExtraParam.subVideoSource = RtcDefines.SubVideoSource.InternalRender;
```
Subscribe video source. See [SubVideoSource](#subvideosource).



# VideoConfig
```java
public class com.vewtn.VideoConfig
```

Video config


## Member Property

| Type | Name |
| --- | --- |
| **int** | [width](#VideoConfig-width) |
| **int** | [height](#VideoConfig-height) |
| **int** | [maxFPS](#VideoConfig-maxfps) |


## Instructions
<span id="VideoConfig-width"></span>
### width
```java
public int com.vewtn.VideoConfig.width = 640;
```
Width in px. 640 by default.


<span id="VideoConfig-height"></span>
### height
```java
public int com.vewtn.VideoConfig.height = 960;
```
Height in px. 960 by default.


<span id="VideoConfig-maxfps"></span>
### maxFPS
```java
public int com.vewtn.VideoConfig.maxFPS = 15;
```
Maximum fps. 15 by default.



# VideoFrame
```java
public class com.vewtn.VideoFrame
```

Video frame struct


## Member Property

| Type | Name |
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


## Instructions
<span id="VideoFrame-type"></span>
### type
```java
public RtcDefines.VideoFrameType com.vewtn.VideoFrame.type = RtcDefines.VideoFrameType.YuvI420;
```
Frame type. Only YUV I420 supported.


<span id="VideoFrame-width"></span>
### width
```java
public int com.vewtn.VideoFrame.width;
```
Width in px


<span id="VideoFrame-height"></span>
### height
```java
public int com.vewtn.VideoFrame.height;
```
Height in px


<span id="VideoFrame-ybuffer"></span>
### yBuffer
```java
public byte[] com.vewtn.VideoFrame.yBuffer;
```
Y buffer


<span id="VideoFrame-ubuffer"></span>
### uBuffer
```java
public byte[] com.vewtn.VideoFrame.uBuffer;
```
U buffer


<span id="VideoFrame-vbuffer"></span>
### vBuffer
```java
public byte[] com.vewtn.VideoFrame.vBuffer;
```
V buffer


<span id="VideoFrame-ystride"></span>
### yStride
```java
public int com.vewtn.VideoFrame.yStride;
```
Y stride


<span id="VideoFrame-ustride"></span>
### uStride
```java
public int com.vewtn.VideoFrame.uStride;
```
U stride


<span id="VideoFrame-vstride"></span>
### vStride
```java
public int com.vewtn.VideoFrame.vStride;
```
V stride


<span id="VideoFrame-timestampms"></span>
### timestampMs
```java
public long com.vewtn.VideoFrame.timestampMs;
```
Timestamp in ms


<span id="VideoFrame-rotation"></span>
### rotation
```java
public int com.vewtn.VideoFrame.rotation;
```
Counterclockwise rotation. The options are 0, 90, 180, and 270.



