# Publisher
```java
public interface com.vewtn.Publisher
```

Publisher Class


## 成员函数

| 返回 | 名称 |
| --- | --- |
| **int** | [setVideoConfig](#Publisher-setvideoconfig) |
| **int** | [setLocalView](#Publisher-setlocalview) |
| **int** | [startVideoCapture](#Publisher-startvideocapture) |
| **int** | [stopVideoCapture](#Publisher-stopvideocapture) |
| **int** | [switchCamera](#Publisher-switchcamera) |
| **int** | [startAudioCapture](#Publisher-startaudiocapture) |
| **int** | [stopAudioCapture](#Publisher-stopaudiocapture) |
| **int** | [startPublish](#Publisher-startpublish) |
| **int** | [stopPublish](#Publisher-stoppublish) |
| **boolean** | [isPublishing](#Publisher-ispublishing) |
| **int** | [setLocalAudioMute](#Publisher-setlocalaudiomute) |
| **int** | [setLocalVideoMute](#Publisher-setlocalvideomute) |
| **boolean** | [pushEncodedVideoFrame](#Publisher-pushencodedvideoframe) |
| **boolean** | [pushVideoFrame](#Publisher-pushvideoframe) |
| **boolean** | [pushAudioFrame](#Publisher-pushaudioframe) |


## 函数说明
<span id="Publisher-setvideoconfig"></span>
### setVideoConfig
```java
int com.vewtn.Publisher.setVideoConfig(
    VideoConfig videoConfig)
```
设置视频采集参数

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| videoConfig | **VideoConfig** | 视频采集参数。参看 [VideoConfig](keytype.md#videoconfig)。 |

**返回值**

+ 0: 成功  
+ < 0: 失败  


**注意**

+ 必须在调用 [startVideoCapture](#Publisher-startvideocapture) 前调用此接口。


<span id="Publisher-setlocalview"></span>
### setLocalView
```java
int com.vewtn.Publisher.setLocalView(
    View view)
```
设置本地视频渲染时使用的视图。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| view | **View** | 视图对象。必须保证此对象处于有效的生命周期内。 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 如果需要解除绑定，你可以调用本方法传入空视图。
+ 必须在调用 [startVideoCapture](#Publisher-startvideocapture) 前调用此接口。 
+ 通过此接口，仅可以渲染通过 [startVideoCapture](#Publisher-startvideocapture) 采集到的视频数据。


<span id="Publisher-startvideocapture"></span>
### startVideoCapture
```java
int com.vewtn.Publisher.startVideoCapture()
```
开启摄像头采集。默认为关闭状态。  <br>

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 发布前后都可调用。 
+ 调用 [stopVideoCapture](#Publisher-stopvideocapture) 可以停止摄像头采集。 
+ 采集使用的摄像头由 [switchCamera](#Publisher-switchcamera) 指定。


<span id="Publisher-stopvideocapture"></span>
### stopVideoCapture
```java
int com.vewtn.Publisher.stopVideoCapture()
```
关闭摄像头采集。

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 调用 [startVideoCapture](#Publisher-startvideocapture) 可以开启摄像头采集。  
+ 如果不调用本方法停止采集，则只有当销毁 Publisher 实例时，摄像头采集才会停止。   


<span id="Publisher-switchcamera"></span>
### switchCamera
```java
int com.vewtn.Publisher.switchCamera()
```
切换摄像头采集时使用的前置/后置摄像头。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 默认使用前置摄像头。
+ 如果你正在使用相机进行视频采集，切换操作当即生效；如果未处于采集状态，后续开启摄像头采集时，会打开设定的摄像头。


<span id="Publisher-startaudiocapture"></span>
### startAudioCapture
```java
int com.vewtn.Publisher.startAudioCapture()
```
开启麦克风采集。默认为关闭状态。  <br>

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [stopAudioCapture](#Publisher-stopaudiocapture) 可以关闭音频采集设备。 
+ 无论是否发布音频数据，你都可以调用此方法开启麦克风采集。


<span id="Publisher-stopaudiocapture"></span>
### stopAudioCapture
```java
int com.vewtn.Publisher.stopAudioCapture()
```
关闭麦克风采集。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startAudioCapture](#Publisher-startaudiocapture) 可以开启麦克风采集。  
+ 如果不调用此方法停止麦克风采集，则只有当销毁 Publisher 实例时，麦克风采集才会停止。


<span id="Publisher-startpublish"></span>
### startPublish
```java
int com.vewtn.Publisher.startPublish(
    String url)
```
发布音视频流

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **String** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，PUB 方法，appID，你设定的 streamID, token 和 TokenType 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/pub/<appID>/<streamID>?Token=<token>&TokenType=Bearer` |

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 对于一个 Publisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。
+ 无论是否开启音视频采集，你都可以调用此接口开启发布。
+ 调用此接口后，会收到 [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged)。
+ 调用 [stopPublish](#Publisher-stoppublish) 取消发布。


<span id="Publisher-stoppublish"></span>
### stopPublish
```java
int com.vewtn.Publisher.stopPublish()
```
停止发布音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败


**注意**

+ 调用 [startPublish](#Publisher-startpublish) 发布音视频流。
+ 调用此接口后，会收到 [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged)。


<span id="Publisher-ispublishing"></span>
### isPublishing
```java
boolean com.vewtn.Publisher.isPublishing()
```
查询音视频流发布状态

**返回值**

+ true：发布中 
+ false：未发布 


<span id="Publisher-setlocalaudiomute"></span>
### setLocalAudioMute
```java
int com.vewtn.Publisher.setLocalAudioMute(
    boolean mute)
```
设置本地音频发送状态。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| mute | **boolean** | 发送音频数据的状态。默认值为发送。<br/>• ture：不发送音频数据 <br/>• false：正常发送音频数据 |

**返回值**

+ 0：调用成功；
+ <0：调用失败


**注意**
发布前后，采集前后都可调用。

<span id="Publisher-setlocalvideomute"></span>
### setLocalVideoMute
```java
int com.vewtn.Publisher.setLocalVideoMute(
    boolean mute)
```
设置本地视频发送状态。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| mute | **boolean** | 发送视频数据的状态。默认值为发送。<br/>• ture：不发送视频数据 <br/>• false：正常发送视频数据 |

**返回值**

+ 0：调用成功；
+ <0：调用失败


**注意**
发布前后，采集前后都可调用。

<span id="Publisher-pushencodedvideoframe"></span>
### pushEncodedVideoFrame
```java
boolean com.vewtn.Publisher.pushEncodedVideoFrame(
    EncodedVideoFrame encodedFrame)
```
推送外部编码帧。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| encodedFrame | **EncodedVideoFrame** | 外部编码帧。参看 [EncodedVideoFrame](keytype.md#encodedvideoframe)。在推送过程中，帧的分辨率应保持一致。 |

**返回值**

+ 0：调用成功；
+ <0：调用失败


**注意**

+ 不支持编码反馈和请求关键帧反馈。
+ 根据实际帧率，周期推送外部编码帧。


<span id="Publisher-pushvideoframe"></span>
### pushVideoFrame
```java
boolean com.vewtn.Publisher.pushVideoFrame(
    VideoFrame videoFrame)
```
推送外部视频帧。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| videoFrame | **VideoFrame** | 视频帧。参看 [VideoFrame](keytype.md#videoframe)。在推送过程中，帧的分辨率应保持一致，帧的宽高应为 16 的倍数。 |

**返回值**

+ 0：调用成功；
+ <0：调用失败


**注意**
根据实际帧率，周期推送外部编码帧。

<span id="Publisher-pushaudioframe"></span>
### pushAudioFrame
```java
boolean com.vewtn.Publisher.pushAudioFrame(
    AudioFrame audioFrame)
```
推送外部音频帧

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| audioFrame | **AudioFrame** | 音频帧。参看 [AudioFrame](keytype.md#audioframe)。在推送过程中，帧的参数应保持一致。 |

**返回值**

+ 0：调用成功；
+ <0：调用失败


**注意**
周期推送外部编码帧。建议周期为 10 ms。

# RtcEngine
```java
public abstract class com.vewtn.RtcEngine
```



## 静态函数

| 返回 | 名称 |
| --- | --- |
| **synchronized static RtcEngine** | [createRtcEngine](#RtcEngine-creatertcengine) |


## 成员函数

| 返回 | 名称 |
| --- | --- |
| **synchronized int** | [destroyRtcEngine](#RtcEngine-destroyrtcengine) |
| **abstract Publisher** | [createPublisher](#RtcEngine-createpublisher) |
| **abstract void** | [destroyPublisher](#RtcEngine-destroypublisher) |
| **abstract Subscriber** | [createSubscriber](#RtcEngine-createsubscriber) |
| **abstract void** | [destroySubscriber](#RtcEngine-destroysubscriber) |


## 函数说明
<span id="RtcEngine-creatertcengine"></span>
### createRtcEngine
```java
public synchronized static RtcEngine com.vewtn.RtcEngine.createRtcEngine(
    Context context,
    RtcEventHandler handler)
```
创建引擎实例。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| context | **Context** | Android Application Context |
| handler | **RtcEventHandler** | SDK 回调给应用层的 Handler，详见 [RtcEventHandler](callback.md#rtceventhandler)。 |

**返回值**

+ RtcEngine：创建成功。返回一个可用的 [RtcEngine](#rtcengine) 实例  
+ Null：.so 文件加载失败，创建失败。


**注意**

+ 你应注意保持 RtcEventHandler 的生命周期必须大于 RtcEngine 的生命周期，即 handler 必须在调用 createRtcEngine 之前创建，在调用 [destroyRtcEngine](#RtcEngine-destroyrtcengine) 之后销毁。  
+ 你只能创建1个 RtcEngine 实例。


<span id="RtcEngine-destroyrtcengine"></span>
### destroyRtcEngine
```java
public synchronized int com.vewtn.RtcEngine.destroyRtcEngine()
```
销毁由 [createRtcEngine](#RtcEngine-creatertcengine) 所创建的 RtcEngine 实例，并释放所有相关资源

**返回值**

+ 0：设置成功；
+ <0：设置失败


**注意**

+ 请确保和需要销毁的 [RtcEngine](#rtcengine) 实例相关的业务场景全部结束后，才调用此方法  
+ 该方法在调用之后，会销毁所有和此 [RtcEngine](#rtcengine) 实例相关的内存，并且停止与媒体服务器的任何交互  
+ 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，也不要在回调中等待主线程的执行，并同时在主线程调用本方法。不然会造成死锁。


<span id="RtcEngine-createpublisher"></span>
### createPublisher
```java
public abstract Publisher com.vewtn.RtcEngine.createPublisher(
    PubExtraParam pubExtraParam)
```
创建发布 Publisher。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| pubExtraParam | **PubExtraParam** | 设置发布的音视频源 |

**返回值**

+ Publisher：创建成功。返回一个可用的 [Publisher](#publisher) 实例  
+ Null：创建失败。可能原因有：已经创建一个内部源Publisher后再次创建。


**注意**

+ 发布可以创建任意多个外部源，但至多只能创建一个音视频内部源。 


<span id="RtcEngine-destroypublisher"></span>
### destroyPublisher
```java
public abstract void com.vewtn.RtcEngine.destroyPublisher(
    Publisher publisher)
```
销毁由 [createPublisher](#RtcEngine-createpublisher) 所创建的 Publisher 实例，并释放所有相关资源

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| publisher | **Publisher** | 待销毁的 publisher 对象 |


**注意**

+ 请确保和需要销毁的 [Publisher](#publisher) 实例相关的业务场景全部结束后，才调用此方法  
+ 该方法在调用之后，会销毁所有和此 [Publisher](#publisher) 实例相关的内存，并且停止与媒体服务器的任何交互  


<span id="RtcEngine-createsubscriber"></span>
### createSubscriber
```java
public abstract Subscriber com.vewtn.RtcEngine.createSubscriber(
    SubExtraParam subExtraParam)
```
创建发布 Subscriber。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| subExtraParam | **SubExtraParam** | 设置订阅的音视频源 |

**返回值**

+ Subscriber：创建成功。返回一个可用的 [Subscriber](#subscriber) 实例  
+ Null：创建失败。可能原因有：已经创建一个内部源 Subscriber 后再次创建。


**注意**

+ 订阅可以创建任意多个外部源，但至多只能创建一个音视频内部源。 


<span id="RtcEngine-destroysubscriber"></span>
### destroySubscriber
```java
public abstract void com.vewtn.RtcEngine.destroySubscriber(
    Subscriber subscriber)
```
销毁由 [createSubscriber](#RtcEngine-createsubscriber) 所创建的 Subscriber 实例，并释放所有相关资源

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| subscriber | **Subscriber** | 待销毁的 subscriber 对象 |


**注意**

+ 请确保和需要销毁的 [Subscriber](#subscriber) 实例相关的业务场景全部结束后，才调用此方法  
+ 该方法在调用之后，会销毁所有和此 [Subscriber](#subscriber) 实例相关的内存，并且停止与媒体服务器的任何交互  


# Subscriber
```java
public interface com.vewtn.Subscriber
```

Subscriber Class


## 成员函数

| 返回 | 名称 |
| --- | --- |
| **int** | [setRemoteView](#Subscriber-setremoteview) |
| **int** | [setEnableSpeaker](#Subscriber-setenablespeaker) |
| **int** | [startSubscribe](#Subscriber-startsubscribe) |
| **int** | [stopSubscribe](#Subscriber-stopsubscribe) |
| **boolean** | [isSubscribed](#Subscriber-issubscribed) |
| **int** | [setRemoteAudioMute](#Subscriber-setremoteaudiomute) |
| **int** | [setRemoteVideoMute](#Subscriber-setremotevideomute) |


## 函数说明
<span id="Subscriber-setremoteview"></span>
### setRemoteView
```java
int com.vewtn.Subscriber.setRemoteView(
    View view)
```
设置渲染远端视频流时使用的视图

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| view | **View** | 视图对象。必须保证此对象处于有效的生命周期内。 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 你必须在订阅前，设置渲染视图。 
+ 如果需要解除绑定，你可以调用本方法传入空视图。


<span id="Subscriber-setenablespeaker"></span>
### setEnableSpeaker
```java
int com.vewtn.Subscriber.setEnableSpeaker(
    Boolean enable)
```
切换播放音频流时使用的听筒/扬声器 <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| enable | **Boolean** | • true：扬声器 <br/>• false：听筒 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 默认使用扬声器。 
+ 此设置在播放音频流前后都可以生效。


<span id="Subscriber-startsubscribe"></span>
### startSubscribe
```java
int com.vewtn.Subscriber.startSubscribe(
    String url)
```
开启订阅音视频流。  <br>
调用该方法后，本地用户会收到 [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) 的回调。  <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **String** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，SUB 方法，appID，和你设定的 streamID 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/sub/<appID>/<streamID>` |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 一个 Subscriber 实例仅可以订阅一路音视频流。
+ 订阅音视频流前，请先确保此流已发布。如果未发布，会订阅失败。 
+ 订阅前需要先调用 [setRemoteView](#Subscriber-setremoteview) 设置视图。 
+ 调用 [stopSubscribe](#Subscriber-stopsubscribe) 可以停止订阅。 


<span id="Subscriber-stopsubscribe"></span>
### stopSubscribe
```java
int com.vewtn.Subscriber.stopSubscribe()
```
停止订阅音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startSubscribe](#Subscriber-startsubscribe) 发布音视频流。
+ 调用该方法后，会收到 [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) 的回调。


<span id="Subscriber-issubscribed"></span>
### isSubscribed
```java
boolean com.vewtn.Subscriber.isSubscribed()
```
查询音视频流订阅状态

**返回值**

+ true：正在订阅 
+ false：未订阅 


<span id="Subscriber-setremoteaudiomute"></span>
### setRemoteAudioMute
```java
int com.vewtn.Subscriber.setRemoteAudioMute(
    boolean mute)
```
mute 远端音频，不订阅。默认值为订阅。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| mute | **boolean** | 是否订阅音频数据。 |

**返回值**

+ 0: Success 
+ < 0: Failure 


**注意**
发布前后都可调用。

<span id="Subscriber-setremotevideomute"></span>
### setRemoteVideoMute
```java
int com.vewtn.Subscriber.setRemoteVideoMute(
    boolean mute)
```
mute 远端视频，不订阅。默认值为订阅。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| mute | **boolean** | 是否订阅视频数据。 |

**返回值**

+ 0: Success 
+ < 0: Failure 


**注意**
发布前后都可调用。

