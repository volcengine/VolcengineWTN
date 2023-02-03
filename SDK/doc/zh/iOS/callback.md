# VEWTNEngineDelegate
```objectivec
@protocol VEWTNEngineDelegate <NSObject>
```

事件回调接口


## 成员函数

| 返回 | 名称 |
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


## 函数说明
<span id="VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason"></span>
### onPublishStateChanged:publishState:reason:
```objectivec

- (void)onPublishStateChanged:(NSString*)stream_id publishState:(VEWTNState)state reason:(VEWTNReason)reason;
```
音视频流的发布状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| state | **VEWTNState** | 当前发布状态，参看 [VEWTNState](keytype.md#vewtnstate)。 |
| reason | **VEWTNReason** | 发布状态发生改变的原因，参看 [VEWTNReason](keytype.md#vewtnreason)。 |


<span id="VEWTNEngineDelegate-onlocalstats-audiostats-videostats"></span>
### onLocalStats:audioStats:videoStats:
```objectivec

- (void)onLocalStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
```
发布流状态的周期回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| audio_stats | **VEWTNRTCAudioStats*** | 音频流数据，参看 [VEWTNRTCAudioStats](keytype.md#vewtnrtcaudiostats)。 |
| video_stats | **VEWTNRTCVideoStats*** | 视频流数据，参看 [VEWTNRTCVideoStats](keytype.md#vewtnrtcvideostats)。 |


<span id="VEWTNEngineDelegate-onmuteaudiosuccess"></span>
### onMuteAudioSuccess:
```objectivec

- (void)onMuteAudioSuccess:(NSString*)stream_id;
```
Mute 音频成功回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |


<span id="VEWTNEngineDelegate-onmuteaudiofailed-reason"></span>
### onMuteAudioFailed:reason:
```objectivec

- (void)onMuteAudioFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
```
Mute 音频失败回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| reason | **VEWTNReason** | 参看 [VEWTNReason](keytype.md#vewtnreason)。 |


<span id="VEWTNEngineDelegate-onmutevideosuccess"></span>
### onMuteVideoSuccess:
```objectivec

- (void)onMuteVideoSuccess:(NSString*)stream_id;
```
Mute 视频成功回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |


<span id="VEWTNEngineDelegate-onmutevideofailed-reason"></span>
### onMuteVideoFailed:reason:
```objectivec

- (void)onMuteVideoFailed:(NSString*)stream_id reason:(VEWTNReason)reason;
```
Mute 视频失败回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| reason | **VEWTNReason** | 参看 [VEWTNReason](keytype.md#vewtnreason)。 |


<span id="VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason"></span>
### onSubscribeStateChanged:subscribeState:reason:
```objectivec

- (void)onSubscribeStateChanged:(NSString*)stream_id subscribeState:(VEWTNState)state reason:(VEWTNReason)reason;
```
音视频流的订阅状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| state | **VEWTNState** | 当前订阅状态，参看 [VEWTNState](keytype.md#vewtnstate)。 |
| reason | **VEWTNReason** | 订阅状态发生改变的原因。参看 [VEWTNReason](keytype.md#vewtnreason)。 |


<span id="VEWTNEngineDelegate-onremotestats-audiostats-videostats"></span>
### onRemoteStats:audioStats:videoStats:
```objectivec

- (void)onRemoteStats:(NSString*)stream_id audioStats:(VEWTNRTCAudioStats*)audio_stats videoStats:(VEWTNRTCVideoStats*)video_stats;
```
订阅流状态的周期回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| audio_stats | **VEWTNRTCAudioStats*** | 音频流数据，参看 [VEWTNRTCAudioStats](keytype.md#vewtnrtcaudiostats)。 |
| video_stats | **VEWTNRTCVideoStats*** | 视频流数据，参看 [VEWTNRTCVideoStats](keytype.md#vewtnrtcvideostats)。 |


<span id="VEWTNEngineDelegate-onfirstremotevideoframerendered"></span>
### onFirstRemoteVideoFrameRendered:
```objectivec

- (void)onFirstRemoteVideoFrameRendered:(NSString*)stream_id;
```
订阅的远端视频流的首帧完成渲染时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |


<span id="VEWTNEngineDelegate-onstallinfo-stallinfo"></span>
### onStallInfo:stallInfo:
```objectivec

- (void)onStallInfo:(NSString*)stream_id stallInfo:(VEWTNStallInfo*)stall_info;
```
卡顿信息回调，周期1秒。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| stall_info | **VEWTNStallInfo*** | 卡顿信息，参看 [VEWTNStallInfo](keytype.md#vewtnstallinfo)。 |


<span id="VEWTNEngineDelegate-onencodedvideoframe-frame"></span>
### onEncodedVideoFrame:frame:
```objectivec

- (void)onEncodedVideoFrame:(NSString*)stream_id frame:(VEWTNEncodedVideoFrame*)encoded_frame;
```
编码视频帧周期回调。周期是帧率的倒数。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| encoded_frame | **VEWTNEncodedVideoFrame*** | 视频帧信息，参看 [VEWTNEncodedVideoFrame](keytype.md#vewtnencodedvideoframe)。 |


<span id="VEWTNEngineDelegate-onvideoframe-frame"></span>
### onVideoFrame:frame:
```objectivec

- (void)onVideoFrame:(NSString*)stream_id frame:(VEWTNVideoFrame*)video_frame;
```
编码视频帧周期回调。周期是帧率的倒数。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| video_frame | **VEWTNVideoFrame*** | 视频帧信息，参看 [VEWTNVideoFrame](keytype.md#vewtnvideoframe)。 |


<span id="VEWTNEngineDelegate-onaudioframe-frame"></span>
### onAudioFrame:frame:
```objectivec

- (void)onAudioFrame:(NSString*)stream_id frame:(VEWTNAudioFrame*)audio_frame;
```
音频帧回调。周期是 10ms。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| stream_id | **NSString*** | 流 ID |
| audio_frame | **VEWTNAudioFrame*** | 音频帧信息，参看 [VEWTNAudioFrame](keytype.md#vewtnaudioframe)。 |


