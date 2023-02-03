# 方法
## 引擎管理

| 方法 | 描述 |
| --- | --- |
| [createRtcEngine](api.md#RtcEngine-creatertcengine) | 创建引擎实例。 |
| [destroyRtcEngine](api.md#RtcEngine-destroyrtcengine) | 销毁由 [createRtcEngine](api.md#RtcEngine-creatertcengine) 所创建的 RtcEngine 实例，并释放所有相关资源 |
| [createPublisher](api.md#RtcEngine-createpublisher) | 创建发布 Publisher。 |
| [destroyPublisher](api.md#RtcEngine-destroypublisher) | 销毁由 [createPublisher](api.md#RtcEngine-createpublisher) 所创建的 Publisher 实例，并释放所有相关资源 |
| [createSubscriber](api.md#RtcEngine-createsubscriber) | 创建发布 Subscriber。 |
| [destroySubscriber](api.md#RtcEngine-destroysubscriber) | 销毁由 [createSubscriber](api.md#RtcEngine-createsubscriber) 所创建的 Subscriber 实例，并释放所有相关资源 |

## 发布

| 方法 | 描述 |
| --- | --- |
| [setVideoConfig](api.md#Publisher-setvideoconfig) | 设置视频采集参数 |
| [setLocalView](api.md#Publisher-setlocalview) | 设置本地视频渲染时使用的视图。 |
| [startVideoCapture](api.md#Publisher-startvideocapture) | 开启摄像头采集。默认为关闭状态。  <br> |
| [stopVideoCapture](api.md#Publisher-stopvideocapture) | 关闭摄像头采集。 |
| [switchCamera](api.md#Publisher-switchcamera) | 切换摄像头采集时使用的前置/后置摄像头。 |
| [startAudioCapture](api.md#Publisher-startaudiocapture) | 开启麦克风采集。默认为关闭状态。  <br> |
| [stopAudioCapture](api.md#Publisher-stopaudiocapture) | 关闭麦克风采集。 |
| [startPublish](api.md#Publisher-startpublish) | 发布音视频流 |
| [stopPublish](api.md#Publisher-stoppublish) | 停止发布音视频流 |
| [isPublishing](api.md#Publisher-ispublishing) | 查询音视频流发布状态 |
| [setLocalAudioMute](api.md#Publisher-setlocalaudiomute) | 设置本地音频发送状态。 |
| [setLocalVideoMute](api.md#Publisher-setlocalvideomute) | 设置本地视频发送状态。 |
| [pushVideoFrame](api.md#Publisher-pushvideoframe) | 推送外部视频帧。 |
| [pushAudioFrame](api.md#Publisher-pushaudioframe) | 推送外部音频帧 |
| [pushEncodedVideoFrame](api.md#Publisher-pushencodedvideoframe) | 推送外部编码帧。 |

## 订阅

| 方法 | 描述 |
| --- | --- |
| [setRemoteView](api.md#Subscriber-setremoteview) | 设置渲染远端视频流时使用的视图 |
| [setEnableSpeaker](api.md#Subscriber-setenablespeaker) | 切换播放音频流时使用的听筒/扬声器 <br> |
| [startSubscribe](api.md#Subscriber-startsubscribe) | 开启订阅音视频流。  <br><br/>调用该方法后，本地用户会收到 [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) 的回调。  <br> |
| [stopSubscribe](api.md#Subscriber-stopsubscribe) | 停止订阅音视频流 |
| [isSubscribed](api.md#Subscriber-issubscribed) | 查询音视频流订阅状态 |
| [setRemoteAudioMute](api.md#Subscriber-setremoteaudiomute) | mute 远端音频，不订阅。默认值为订阅。 |
| [setRemoteVideoMute](api.md#Subscriber-setremotevideomute) | mute 远端视频，不订阅。默认值为订阅。 |

# 回调
## 发布

| 回调 | 描述 |
| --- | --- |
| [onLocalStats](callback.md#RtcEventHandler-onlocalstats) | 发布流状态周期回调，周期1秒。 |
| [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged) | 音视频流的发布状态发生改变时，收到此回调。 |
| [onMuteAudioSuccess](callback.md#RtcEventHandler-onmuteaudiosuccess) | Mute音频成功回调。 |
| [onMuteAudioFailed](callback.md#RtcEventHandler-onmuteaudiofailed) | Mute音频失败回调。 |
| [onMuteVideoSuccess](callback.md#RtcEventHandler-onmutevideosuccess) | Mute视频成功回调。 |
| [onMuteVideoFailed](callback.md#RtcEventHandler-onmutevideofailed) | Mute视频失败回调。 |

## 订阅

| 回调 | 描述 |
| --- | --- |
| [onRemoteStats](callback.md#RtcEventHandler-onremotestats) | 订阅流状态周期回调，周期1秒。 |
| [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) | 音视频流的订阅状态发生改变时，收到此回调。 |
| [onFirstRemoteVideoFrameRendered](callback.md#RtcEventHandler-onfirstremotevideoframerendered) | 远端首帧回调（订阅拉流） |
| [onVideoFrame](callback.md#RtcEventHandler-onvideoframe) | 视频帧回调，周期由帧率决定(1000/fps ms)。 |
| [onAudioFrame](callback.md#RtcEventHandler-onaudioframe) | 音频帧回调，周期10ms。 |
| [onEncodedVideoFrame](callback.md#RtcEventHandler-onencodedvideoframe) | 编码视频帧回调，周期由帧率决定(1000/fps ms)。 |
| [onStallInfo](callback.md#RtcEventHandler-onstallinfo) | 卡顿信息回调，周期1秒。 |

