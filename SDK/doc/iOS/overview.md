## 方法
### 发布

| 方法 | 描述 |
| --- | --- |
| [create:](api.md#Publisher-create) | 创建 Publisher 实例。<br><br/>如果当前线程中未创建实例，那么你必须先调用此 API，以使用 WTN 提供的各种音视频能力。  <br> |
| [destroy](api.md#Publisher-destroy) | 销毁由 [create:](api.md#Publisher-create) 所创建的 Publisher 实例，并释放所有相关资源 |
| [setVideoConfig:](api.md#Publisher-setvideoconfig) | 设置视频采集参数 |
| [setLocalView:](api.md#Publisher-setlocalview) | 设置本地视频渲染时使用的视图。 |
| [startVideoCapture](api.md#Publisher-startvideocapture) | 开启摄像头采集。默认为关闭状态。  <br> |
| [stopVideoCapture](api.md#Publisher-stopvideocapture) | 关闭摄像头采集。 |
| [switchCamera](api.md#Publisher-switchcamera) | 使用内部采集时，切换使用的摄像头：前置或后置摄像头。 |
| [startAudioCapture](api.md#Publisher-startaudiocapture) | 开启麦克风采集。默认为关闭状态。  <br> |
| [stopAudioCapture](api.md#Publisher-stopaudiocapture) | 关闭麦克风采集。 |
| [startPublish:](api.md#Publisher-startpublish) | 发布音视频流 |
| [stopPublish](api.md#Publisher-stoppublish) | 停止发布音视频流 |
| [isPublishing](api.md#Publisher-ispublishing) | 查询音视频流发布状态 |

### 订阅

| 方法 | 描述 |
| --- | --- |
| [create:](api.md#Subscriber-create) | 创建 Subscriber 实例。 |
| [destroy](api.md#Subscriber-destroy) | 销毁由 [create:](api.md#Subscriber-create) 所创建的 Subscriber 实例，并释放所有相关资源 |
| [setRemoteView:](api.md#Subscriber-setremoteview) | 设置渲染远端视频流时使用的视图 |
| [setEnableSpeaker:](api.md#Subscriber-setenablespeaker) | 切换播放音频流时使用的听筒/扬声器 <br> |
| [startSubscribe:](api.md#Subscriber-startsubscribe) | 开启订阅音视频流。  <br><br/>调用该方法后，本地用户会收到 [subscriber:onSubscribeStateChanged:reason:](callback.md#SubscriberDelegate-subscriber-onsubscribestatechanged-reason) 的回调。  <br> |
| [stopSubscribe](api.md#Subscriber-stopsubscribe) | 停止订阅音视频流 |
| [isSubscribed](api.md#Subscriber-issubscribed) | 查询音视频流订阅状态 |

## 回调
### 发布

| 回调 | 描述 |
| --- | --- |
| [publisher:onPublishStateChanged:reason:](callback.md#PublisherDelegate-publisher-onpublishstatechanged-reason) | 音视频流的发布状态发生改变时，收到此回调。 |

### 订阅

| 回调 | 描述 |
| --- | --- |
| [subscriber:onSubscribeStateChanged:reason:](callback.md#SubscriberDelegate-subscriber-onsubscribestatechanged-reason) | 音视频流的订阅状态发生改变时，收到此回调。 |

