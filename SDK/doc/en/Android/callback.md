# RtcEventHandler
```java
public class com.vewtn.RtcEventHandler
```

Event handler for RtcEngine


## Member Functions

| Return | Name |
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


## Instructions
<span id="RtcEventHandler-onpublishstatechanged"></span>
### onPublishStateChanged
```java
public void com.vewtn.RtcEventHandler.onPublishStateChanged(
    String streamId,
    RtcDefines.State state,
    RtcDefines.Reason reason)
```
Receive this event when the publish state of the media stream changes.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | stream id |
| state | **RtcDefines.State** | The current publishing state. See [State](keytype.md#state). |
| reason | **RtcDefines.Reason** | Error code for the change. See [Reason](keytype.md#reason). |


<span id="RtcEventHandler-onsubscribestatechanged"></span>
### onSubscribeStateChanged
```java
public void com.vewtn.RtcEventHandler.onSubscribeStateChanged(
    String streamId,
    RtcDefines.State state,
    RtcDefines.Reason reason)
```
Receive this event when the subscribing state changes.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | stream id |
| state | **RtcDefines.State** | The current subscribing state. See [State](keytype.md#state). |
| reason | **RtcDefines.Reason** | Error code when subscribing state changes. See [Reason](keytype.md#reason) |


<span id="RtcEventHandler-onlocalstats"></span>
### onLocalStats
```java
public void com.vewtn.RtcEventHandler.onLocalStats(
    String streamId,
    RTCAudioStats audioStats,
    RTCVideoStats videoStats)
```
Release flow state cycle callback with 1 second period..

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| audioStats | **RTCAudioStats** | Audio stream information, see [RTCAudioStats](keytype.md#rtcaudiostats). |
| videoStats | **RTCVideoStats** | Video stream information, see [RTCVideoStats](keytype.md#rtcvideostats). |


<span id="RtcEventHandler-onmuteaudiosuccess"></span>
### onMuteAudioSuccess
```java
public void com.vewtn.RtcEventHandler.onMuteAudioSuccess(
    String streamId)
```
Mute audio success callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |


<span id="RtcEventHandler-onmuteaudiofailed"></span>
### onMuteAudioFailed
```java
public void com.vewtn.RtcEventHandler.onMuteAudioFailed(
    String streamId,
    RtcDefines.Reason reason)
```
Mute audio failed callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |


<span id="RtcEventHandler-onmutevideosuccess"></span>
### onMuteVideoSuccess
```java
public void com.vewtn.RtcEventHandler.onMuteVideoSuccess(
    String streamId)
```
Mute video success callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |


<span id="RtcEventHandler-onmutevideofailed"></span>
### onMuteVideoFailed
```java
public void com.vewtn.RtcEventHandler.onMuteVideoFailed(
    String streamId,
    RtcDefines.Reason reason)
```
Mute video failed callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |


<span id="RtcEventHandler-onremotestats"></span>
### onRemoteStats
```java
public void com.vewtn.RtcEventHandler.onRemoteStats(
    String streamId,
    RTCAudioStats audioStats,
    RTCVideoStats videoStats)
```
Release flow state cycle callback with 1 second period..

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| audioStats | **RTCAudioStats** | Audio stream information, see [RTCAudioStats](keytype.md#rtcaudiostats). |
| videoStats | **RTCVideoStats** | Video stream information, see [RTCVideoStats](keytype.md#rtcvideostats). |


<span id="RtcEventHandler-onfirstremotevideoframerendered"></span>
### onFirstRemoteVideoFrameRendered
```java
public void com.vewtn.RtcEventHandler.onFirstRemoteVideoFrameRendered(
    String streamId)
```
Release flow state cycle callback with 1 second period..

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |


<span id="RtcEventHandler-onstallinfo"></span>
### onStallInfo
```java
public void com.vewtn.RtcEventHandler.onStallInfo(
    String streamId,
    StallInfo stallInfo)
```
Stuck message callback, period 1 sec.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| stallInfo | **StallInfo** | The stall information, see [StallInfo](keytype.md#stallinfo). |


<span id="RtcEventHandler-onencodedvideoframe"></span>
### onEncodedVideoFrame
```java
public void com.vewtn.RtcEventHandler.onEncodedVideoFrame(
    String streamId,
    EncodedVideoFrame encodedFrame)
```
Video frame callback with period determined by frame rate (1000/fps ms).

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| encodedFrame | **EncodedVideoFrame** | Video frame information, see [EncodedVideoFrame](keytype.md#encodedvideoframe). |


<span id="RtcEventHandler-onvideoframe"></span>
### onVideoFrame
```java
public void com.vewtn.RtcEventHandler.onVideoFrame(
    String streamId,
    VideoFrame videoFrame)
```
Video frame callback with period determined by frame rate (1000/fps ms).

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| videoFrame | **VideoFrame** | Video frame information, see [VideoFrame](keytype.md#videoframe). |


<span id="RtcEventHandler-onaudioframe"></span>
### onAudioFrame
```java
public void com.vewtn.RtcEventHandler.onAudioFrame(
    String streamId,
    AudioFrame audioFrame)
```
Audio frame callback with 10ms period.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| streamId | **String** | Stream ID |
| audioFrame | **AudioFrame** | Audio frame information, see [AudioFrame](keytype.md#audioframe). |


