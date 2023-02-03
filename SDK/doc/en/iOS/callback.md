# VEWTNEngineDelegate
```objectivec
@protocol VEWTNEngineDelegate <NSObject>
```

Event handler for engine (include pub & sub)


## Member Functions

| Return | Name |
| --- | --- |
| **void** | [onPublishStateChanged:publishState:reason:](#VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason) |
| **void** | [onLocalStats:audioStats:videoStats:](#VEWTNEngineDelegate-onlocalstats-audiostats-videostats) |
| **void** | [onMuteAudioSuccess:](#VEWTNEngineDelegate-onmuteaudiosuccess) |
| **void** | [onMuteAudioFailed:reason:](#VEWTNEngineDelegate-onmuteaudiofailed-reason) |
| **void** | [onMuteVideoSuccess:](#VEWTNEngineDelegate-onmutevideosuccess) |
| **void** | [onMuteVideoFailed:reason:](#VEWTNEngineDelegate-onmutevideofailed-reason) |
| **void** | [onSubscribeStateChanged:subscribeState:reason:](#VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason) |
| **void** | [onRemoteStats:audioStats:videoStats:](#VEWTNEngineDelegate-onremotestats-audiostats-videostats) |
| **void** | [onFirstRemoteVideoFrameRendered:](#VEWTNEngineDelegate-onfirstremotevideoframerendered) |
| **void** | [onStallInfo:stallInfo:](#VEWTNEngineDelegate-onstallinfo-stallinfo) |
| **void** | [onEncodedVideoFrame:frame:](#VEWTNEngineDelegate-onencodedvideoframe-frame) |
| **void** | [onVideoFrame:frame:](#VEWTNEngineDelegate-onvideoframe-frame) |
| **void** | [onAudioFrame:frame:](#VEWTNEngineDelegate-onaudioframe-frame) |


## Instructions
<span id="VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason"></span>
### onPublishStateChanged:publishState:reason:
```objectivec

- (void)onPublishStateChanged:(NSString*)stream_id publishState:(VEWTNState)state reason:(VEWTNReason)reason;
```
Callback of media stream publishing state change.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | stream ID |
| state | **VEWTNState** | The current state. See [VEWTNState](keytype.md#vewtnstate). |
| reason | **VEWTNReason** | Error code for the change. See [VEWTNReason](keytype.md#vewtnreason). |


<span id="VEWTNEngineDelegate-onlocalstats-audiostats-videostats"></span>
### onLocalStats:audioStats:videoStats:
```objectivec

- (void)onLocalStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
```
Periodic callback of the stats of locally published stream in every second.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| audio_stats | **VEWTNRTCAudioStats*** | Stats of audio stream. See [VEWTNRTCAudioStats](keytype.md#vewtnrtcaudiostats). |
| video_stats | **VEWTNRTCVideoStats*** | Stats of video stream. See [VEWTNRTCVideoStats](keytype.md#vewtnrtcvideostats). |


<span id="VEWTNEngineDelegate-onmuteaudiosuccess"></span>
### onMuteAudioSuccess:
```objectivec

- (void)onMuteAudioSuccess:(NSString*)stream_id;
```
Success to mute audio callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |


<span id="VEWTNEngineDelegate-onmuteaudiofailed-reason"></span>
### onMuteAudioFailed:reason:
```objectivec

- (void)onMuteAudioFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
```
Failure to mute audio callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| reason | **VEWTNReason** | See [VEWTNReason](keytype.md#vewtnreason). |


<span id="VEWTNEngineDelegate-onmutevideosuccess"></span>
### onMuteVideoSuccess:
```objectivec

- (void)onMuteVideoSuccess:(NSString*)stream_id;
```
Success to mute video callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |


<span id="VEWTNEngineDelegate-onmutevideofailed-reason"></span>
### onMuteVideoFailed:reason:
```objectivec

- (void)onMuteVideoFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
```
Failure to mute video callback.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| reason | **VEWTNReason** | See [VEWTNReason](keytype.md#vewtnreason). |


<span id="VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason"></span>
### onSubscribeStateChanged:subscribeState:reason:
```objectivec

- (void)onSubscribeStateChanged:(NSString*)stream_id subscribeState:(VEWTNState)state reason:(VEWTNReason)reason;
```
Callback of media stream subscribing state change.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | stream ID |
| state | **VEWTNState** | The current subscribing state. See [VEWTNState](keytype.md#vewtnstate). |
| reason | **VEWTNReason** | Error code when subscribing state changes. See [VEWTNReason](keytype.md#vewtnreason). |


<span id="VEWTNEngineDelegate-onremotestats-audiostats-videostats"></span>
### onRemoteStats:audioStats:videoStats:
```objectivec

- (void)onRemoteStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
```
Periodic callback of the stats of the subscribed remote stream in every second.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| audio_stats | **VEWTNRTCAudioStats*** | Audio stream information. See [VEWTNRTCAudioStats](keytype.md#vewtnrtcaudiostats). |
| video_stats | **VEWTNRTCVideoStats*** | Video stream information. See [VEWTNRTCVideoStats](keytype.md#vewtnrtcvideostats). |


<span id="VEWTNEngineDelegate-onfirstremotevideoframerendered"></span>
### onFirstRemoteVideoFrameRendered:
```objectivec

- (void)onFirstRemoteVideoFrameRendered:(NSString*)stream_id;
```
Receive the callback when the first frame of the subscribed remote frame is rendered.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |


<span id="VEWTNEngineDelegate-onstallinfo-stallinfo"></span>
### onStallInfo:stallInfo:
```objectivec

- (void)onStallInfo:(NSString*)stream_id stallInfo:(VEWTNStallInfo*)stall_info;
```
Periodic callback of the stall information in every second.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| stall_info | **VEWTNStallInfo*** | The stall information. See [VEWTNStallInfo](keytype.md#vewtnstallinfo). |


<span id="VEWTNEngineDelegate-onencodedvideoframe-frame"></span>
### onEncodedVideoFrame:frame:
```objectivec

- (void)onEncodedVideoFrame:(NSString*)stream_id frame:(VEWTNEncodedVideoFrame*)encoded_frame;
```
Periodic encoded video frame callback. The period is the reciprocal of the framerate.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| encoded_frame | **VEWTNEncodedVideoFrame*** | Video frame information, see [VEWTNEncodedVideoFrame](keytype.md#vewtnencodedvideoframe). |


<span id="VEWTNEngineDelegate-onvideoframe-frame"></span>
### onVideoFrame:frame:
```objectivec

- (void)onVideoFrame:(NSString*)stream_id frame:(VEWTNVideoFrame*)video_frame;
```
Periodic encoded video frame callback. The period is the reciprocal of the framerate.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| video_frame | **VEWTNVideoFrame*** | Video frame information, see [VEWTNVideoFrame](keytype.md#vewtnvideoframe). |


<span id="VEWTNEngineDelegate-onaudioframe-frame"></span>
### onAudioFrame:frame:
```objectivec

- (void)onAudioFrame:(NSString*)stream_id frame:(VEWTNAudioFrame*)audio_frame;
```
Periodic audio frame callback in every 10ms.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| stream_id | **NSString*** | Stream ID |
| audio_frame | **VEWTNAudioFrame*** | Audio frame information, see [VEWTNAudioFrame](keytype.md#vewtnaudioframe). |


