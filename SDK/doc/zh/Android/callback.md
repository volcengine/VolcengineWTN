# RtcEventHandler
```java
public class com.vewtn.RtcEventHandler
```

事件回调类


## 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [onPublishStateChanged](#RtcEventHandler-onpublishstatechanged) |
| **void** | [onSubscribeStateChanged](#RtcEventHandler-onsubscribestatechanged) |
| **void** | [onLocalStats](#RtcEventHandler-onlocalstats) |
| **void** | [onMuteAudioSuccess](#RtcEventHandler-onmuteaudiosuccess) |
| **void** | [onMuteAudioFailed](#RtcEventHandler-onmuteaudiofailed) |
| **void** | [onMuteVideoSuccess](#RtcEventHandler-onmutevideosuccess) |
| **void** | [onMuteVideoFailed](#RtcEventHandler-onmutevideofailed) |
| **void** | [onRemoteStats](#RtcEventHandler-onremotestats) |
| **void** | [onFirstRemoteVideoFrameRendered](#RtcEventHandler-onfirstremotevideoframerendered) |
| **void** | [onStallInfo](#RtcEventHandler-onstallinfo) |
| **void** | [onEncodedVideoFrame](#RtcEventHandler-onencodedvideoframe) |
| **void** | [onVideoFrame](#RtcEventHandler-onvideoframe) |
| **void** | [onAudioFrame](#RtcEventHandler-onaudioframe) |


## 函数说明
<span id="RtcEventHandler-onpublishstatechanged"></span>
### onPublishStateChanged
```java
public void com.vewtn.RtcEventHandler.onPublishStateChanged(
    String streamId,
    RtcDefines.State state,
    RtcDefines.Reason reason)
```
音视频流的发布状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| state | **RtcDefines.State** | 当前发布状态，参看 [State](keytype.md#state)。 |
| reason | **RtcDefines.Reason** | 发布状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


<span id="RtcEventHandler-onsubscribestatechanged"></span>
### onSubscribeStateChanged
```java
public void com.vewtn.RtcEventHandler.onSubscribeStateChanged(
    String streamId,
    RtcDefines.State state,
    RtcDefines.Reason reason)
```
音视频流的订阅状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| state | **RtcDefines.State** | 当前订阅状态，参看 [State](keytype.md#state)。 |
| reason | **RtcDefines.Reason** | 订阅状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


<span id="RtcEventHandler-onlocalstats"></span>
### onLocalStats
```java
public void com.vewtn.RtcEventHandler.onLocalStats(
    String streamId,
    RTCAudioStats audioStats,
    RTCVideoStats videoStats)
```
发布流状态周期回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| audioStats | **RTCAudioStats** | 音频流信息，参看 [RTCAudioStats](keytype.md#rtcaudiostats)。 |
| videoStats | **RTCVideoStats** | 视频流信息，参看 [RTCVideoStats](keytype.md#rtcvideostats)。 |


<span id="RtcEventHandler-onmuteaudiosuccess"></span>
### onMuteAudioSuccess
```java
public void com.vewtn.RtcEventHandler.onMuteAudioSuccess(
    String streamId)
```
Mute音频成功回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |


<span id="RtcEventHandler-onmuteaudiofailed"></span>
### onMuteAudioFailed
```java
public void com.vewtn.RtcEventHandler.onMuteAudioFailed(
    String streamId,
    RtcDefines.Reason reason)
```
Mute音频失败回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |


<span id="RtcEventHandler-onmutevideosuccess"></span>
### onMuteVideoSuccess
```java
public void com.vewtn.RtcEventHandler.onMuteVideoSuccess(
    String streamId)
```
Mute视频成功回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |


<span id="RtcEventHandler-onmutevideofailed"></span>
### onMuteVideoFailed
```java
public void com.vewtn.RtcEventHandler.onMuteVideoFailed(
    String streamId,
    RtcDefines.Reason reason)
```
Mute视频失败回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |


<span id="RtcEventHandler-onremotestats"></span>
### onRemoteStats
```java
public void com.vewtn.RtcEventHandler.onRemoteStats(
    String streamId,
    RTCAudioStats audioStats,
    RTCVideoStats videoStats)
```
订阅流状态周期回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| audioStats | **RTCAudioStats** | 音频流信息，参看 [RTCAudioStats](keytype.md#rtcaudiostats)。 |
| videoStats | **RTCVideoStats** | 视频流信息，参看 [RTCVideoStats](keytype.md#rtcvideostats)。 |


<span id="RtcEventHandler-onfirstremotevideoframerendered"></span>
### onFirstRemoteVideoFrameRendered
```java
public void com.vewtn.RtcEventHandler.onFirstRemoteVideoFrameRendered(
    String streamId)
```
远端首帧回调（订阅拉流）

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |


<span id="RtcEventHandler-onstallinfo"></span>
### onStallInfo
```java
public void com.vewtn.RtcEventHandler.onStallInfo(
    String streamId,
    StallInfo stallInfo)
```
卡顿信息回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| stallInfo | **StallInfo** | 卡顿信息，参看 [StallInfo](keytype.md#stallinfo)。 |


<span id="RtcEventHandler-onencodedvideoframe"></span>
### onEncodedVideoFrame
```java
public void com.vewtn.RtcEventHandler.onEncodedVideoFrame(
    String streamId,
    EncodedVideoFrame encodedFrame)
```
编码视频帧回调，周期由帧率决定(1000/fps ms)。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| encodedFrame | **EncodedVideoFrame** | 视频帧信息，参看 [EncodedVideoFrame](keytype.md#encodedvideoframe)。 |


<span id="RtcEventHandler-onvideoframe"></span>
### onVideoFrame
```java
public void com.vewtn.RtcEventHandler.onVideoFrame(
    String streamId,
    VideoFrame videoFrame)
```
视频帧回调，周期由帧率决定(1000/fps ms)。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| videoFrame | **VideoFrame** | 视频帧信息，参看 [VideoFrame](keytype.md#videoframe)。 |


<span id="RtcEventHandler-onaudioframe"></span>
### onAudioFrame
```java
public void com.vewtn.RtcEventHandler.onAudioFrame(
    String streamId,
    AudioFrame audioFrame)
```
音频帧回调，周期10ms。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| streamId | **String** | 流 ID |
| audioFrame | **AudioFrame** | 音频帧信息，参看 [AudioFrame](keytype.md#audioframe)。 |


