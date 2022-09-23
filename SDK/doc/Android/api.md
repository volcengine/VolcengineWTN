## Publisher
```java
public abstract class com.vewtn.Publisher
```

Publisher Class


### 静态函数

| 返回 | 名称 |
| --- | --- |
| **synchronized static Publisher** | [create](#Publisher-create) |


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **synchronized int** | [destroy](#Publisher-destroy) |
| **abstract int** | [setVideoConfig](#Publisher-setvideoconfig) |
| **abstract int** | [setLocalView](#Publisher-setlocalview) |
| **abstract int** | [startVideoCapture](#Publisher-startvideocapture) |
| **abstract int** | [stopVideoCapture](#Publisher-stopvideocapture) |
| **abstract int** | [switchCamera](#Publisher-switchcamera) |
| **abstract int** | [startAudioCapture](#Publisher-startaudiocapture) |
| **abstract int** | [stopAudioCapture](#Publisher-stopaudiocapture) |
| **abstract int** | [startPublish](#Publisher-startpublish) |
| **abstract int** | [stopPublish](#Publisher-stoppublish) |
| **abstract boolean** | [isPublishing](#Publisher-ispublishing) |


### 函数说明
<span id="Publisher-create"></span>
#### create
```java
public synchronized static Publisher com.vewtn.Publisher.create(
    Context context,
    PublisherEventHandler handler)
```
创建 Publisher 实例。<br>
如果当前线程中未创建实例，那么你必须先调用此 API，以使用 WTN 提供的各种音视频能力。  <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| context | **Context** | Android Application Context |
| handler | **PublisherEventHandler** | SDK 回调给应用层的 Handler，详见 [PublisherEventHandler](callback.md#publishereventhandler) |

**返回值**

+ Publisher：创建成功。返回一个可用的 [Publisher](#publisher) 实例  
+ Null：.so 文件加载失败，创建失败。


**注意**

+ 你可以创建多个 Publisher 实例，以发布多个音视频流。
+ 你应注意保持 PublisherEventHandler 的生命周期必须大于 Publisher 的生命周期，即 handler 必须在调用 create 之前创建，在调用 [destroy](#Publisher-destroy) 之后销毁。


<span id="Publisher-destroy"></span>
#### destroy
```java
public synchronized int com.vewtn.Publisher.destroy()
```
销毁由 [create](#Publisher-create) 所创建的 Publisher 实例，并释放所有相关资源

**返回值**

+ 0：设置成功；
+ <0：设置失败


**注意**

+ 请确保和需要销毁的 [Publisher](#publisher) 实例相关的业务场景全部结束后，才调用此方法。  
+ 该方法在调用之后，会销毁所有和此 [Publisher](#publisher) 实例相关的内存，并且停止与媒体服务器的任何交互。  
+ 调用本方法会启动 SDK 退出逻辑。线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，也不要在回调中等待主线程的执行，并同时在主线程调用本方法。不然会造成死锁。


<span id="Publisher-setvideoconfig"></span>
#### setVideoConfig
```java
public abstract int com.vewtn.Publisher.setVideoConfig(
    VideoConfig video_config)
```
设置视频采集参数

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| video_config | **VideoConfig** | 视频采集参数。参看 [VideoConfig](keytype.md#videoconfig)。 |

**返回值**

+ 0: 成功  
+ < 0: 失败  


**注意**

+ 必须在调用 [startVideoCapture](#Publisher-startvideocapture) 前调用此接口。
+ 建议同一设备上的不同 Publisher 实例使用相同的视频采集参数。


<span id="Publisher-setlocalview"></span>
#### setLocalView
```java
public abstract int com.vewtn.Publisher.setLocalView(
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


<span id="Publisher-startvideocapture"></span>
#### startVideoCapture
```java
public abstract int com.vewtn.Publisher.startVideoCapture()
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
#### stopVideoCapture
```java
public abstract int com.vewtn.Publisher.stopVideoCapture()
```
关闭摄像头采集。

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 调用 [startVideoCapture](#Publisher-startvideocapture) 可以开启摄像头采集。  
+ 如果不调用本方法停止采集，则只有当销毁 Publisher 实例时，摄像头采集才会停止。   


<span id="Publisher-switchcamera"></span>
#### switchCamera
```java
public abstract int com.vewtn.Publisher.switchCamera()
```
切换摄像头采集时使用的前置/后置摄像头。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 默认使用前置摄像头。
+ 如果你正在使用相机进行视频采集，切换操作当即生效；如果未处于采集状态，后续开启摄像头采集时，会打开设定的摄像头。


<span id="Publisher-startaudiocapture"></span>
#### startAudioCapture
```java
public abstract int com.vewtn.Publisher.startAudioCapture()
```
开启麦克风采集。默认为关闭状态。  <br>

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [stopAudioCapture](#Publisher-stopaudiocapture) 可以关闭音频采集设备。 
+ 无论是否发布音频数据，你都可以调用此方法开启麦克风采集。


<span id="Publisher-stopaudiocapture"></span>
#### stopAudioCapture
```java
public abstract int com.vewtn.Publisher.stopAudioCapture()
```
关闭麦克风采集。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startAudioCapture](#Publisher-startaudiocapture) 可以开启麦克风采集。  
+ 如果不调用此方法停止麦克风采集，则只有当销毁 Publisher 实例时，麦克风采集才会停止。


<span id="Publisher-startpublish"></span>
#### startPublish
```java
public abstract int com.vewtn.Publisher.startPublish(
    String url)
```
发布音视频流

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **String** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，POST/PULL 方法，appID，你设定的 streamID, token 和 TokenType 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/push/<appID>/<streamID>?Token=<token>&TokenType=Bearer` |

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 对于一个 Publisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。
+ 无论是否开启音视频采集，你都可以调用此接口开启发布。
+ 调用此接口后，会收到 [onPublishStateChanged](callback.md#PublisherEventHandler-onpublishstatechanged)。
+ 调用 [stopPublish](#Publisher-stoppublish) 取消发布。


<span id="Publisher-stoppublish"></span>
#### stopPublish
```java
public abstract int com.vewtn.Publisher.stopPublish()
```
停止发布音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败


**注意**

+ 调用 [startPublish](#Publisher-startpublish) 发布音视频流。
+ 调用此接口后，会收到 [onPublishStateChanged](callback.md#PublisherEventHandler-onpublishstatechanged)。


<span id="Publisher-ispublishing"></span>
#### isPublishing
```java
public abstract boolean com.vewtn.Publisher.isPublishing()
```
查询音视频流发布状态

**返回值**

+ true：发布中 
+ false：未发布 


## Subscriber
```java
public abstract class com.vewtn.Subscriber
```

Subscriber Class


### 静态函数

| 返回 | 名称 |
| --- | --- |
| **synchronized static Subscriber** | [create](#Subscriber-create) |


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **synchronized int** | [destroy](#Subscriber-destroy) |
| **abstract int** | [setRemoteView](#Subscriber-setremoteview) |
| **abstract int** | [setEnableSpeaker](#Subscriber-setenablespeaker) |
| **abstract int** | [startSubscribe](#Subscriber-startsubscribe) |
| **abstract int** | [stopSubscribe](#Subscriber-stopsubscribe) |
| **abstract boolean** | [isSubscribed](#Subscriber-issubscribed) |


### 函数说明
<span id="Subscriber-create"></span>
#### create
```java
public synchronized static Subscriber com.vewtn.Subscriber.create(
    Context context,
    SubscriberEventHandler handler)
```
创建 Subscriber 实例。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| context | **Context** | Android Application Context |
| handler | **SubscriberEventHandler** | SDK 回调给应用层的 Handler，详见 [SubscriberEventHandler](callback.md#subscribereventhandler)。 |

**返回值**

+ Subscriber：创建成功。返回一个可用的 [Subscriber](#subscriber) 实例  
+ Null：.so 文件加载失败，创建失败。


**注意**

+ 你应注意保持 SubscriberEventHandler 的生命周期必须大于 Subscriber 的生命周期，即 handler 必须在调用 create 之前创建，在调用 [destroy](#Subscriber-destroy) 之后销毁。  
+ 你可以创建多个 Subscriber 实例，以订阅多个音视频流。


<span id="Subscriber-destroy"></span>
#### destroy
```java
public synchronized int com.vewtn.Subscriber.destroy()
```
销毁由 [create](#Subscriber-create) 所创建的 Subscriber 实例，并释放所有相关资源

**返回值**

+ 0：设置成功；
+ <0：设置失败


**注意**

+ 请确保和需要销毁的 [Subscriber](#subscriber) 实例相关的业务场景全部结束后，才调用此方法  
+ 该方法在调用之后，会销毁所有和此 [Subscriber](#subscriber) 实例相关的内存，并且停止与媒体服务器的任何交互  
+ 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，也不要在回调中等待主线程的执行，并同时在主线程调用本方法。不然会造成死锁。


<span id="Subscriber-setremoteview"></span>
#### setRemoteView
```java
public abstract int com.vewtn.Subscriber.setRemoteView(
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
#### setEnableSpeaker
```java
public abstract int com.vewtn.Subscriber.setEnableSpeaker(
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
#### startSubscribe
```java
public abstract int com.vewtn.Subscriber.startSubscribe(
    String url)
```
开启订阅音视频流。  <br>
调用该方法后，本地用户会收到 [onSubscribeStateChanged](callback.md#SubscriberEventHandler-onsubscribestatechanged) 的回调。  <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **String** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，POST/PULL 方法，appID，和你设定的 streamID 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/pull/<appID>/<streamID>` |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 一个 Subscriber 实例仅可以订阅一路音视频流。
+ 订阅音视频流前，请先确保此流已发布。如果未发布，会订阅失败。 
+ 订阅前需要先调用 [setRemoteView](#Subscriber-setremoteview) 设置视图。 
+ 调用 [stopSubscribe](#Subscriber-stopsubscribe) 可以停止订阅。 


<span id="Subscriber-stopsubscribe"></span>
#### stopSubscribe
```java
public abstract int com.vewtn.Subscriber.stopSubscribe()
```
停止订阅音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startSubscribe](#Subscriber-startsubscribe) 发布音视频流。
+ 调用该方法后，会收到 [onSubscribeStateChanged](callback.md#SubscriberEventHandler-onsubscribestatechanged) 的回调。


<span id="Subscriber-issubscribed"></span>
#### isSubscribed
```java
public abstract boolean com.vewtn.Subscriber.isSubscribed()
```
查询音视频流订阅状态

**返回值**

+ true：正在订阅 
+ false：未订阅 


