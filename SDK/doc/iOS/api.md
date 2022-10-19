## Publisher
```objectivec
@interface Publisher : NSObject
```

Publisher Class


### 静态函数

| 返回 | 名称 |
| --- | --- |
| **instancetype** | [create:](#Publisher-create) |


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [destroy](#Publisher-destroy) |
| **int** | [setVideoConfig:](#Publisher-setvideoconfig) |
| **int** | [setLocalView:](#Publisher-setlocalview) |
| **int** | [startVideoCapture](#Publisher-startvideocapture) |
| **int** | [stopVideoCapture](#Publisher-stopvideocapture) |
| **int** | [switchCamera](#Publisher-switchcamera) |
| **int** | [startAudioCapture](#Publisher-startaudiocapture) |
| **int** | [stopAudioCapture](#Publisher-stopaudiocapture) |
| **int** | [startPublish:](#Publisher-startpublish) |
| **int** | [stopPublish](#Publisher-stoppublish) |
| **bool** | [isPublishing](#Publisher-ispublishing) |


### 函数说明
<span id="Publisher-create"></span>
#### create:
```objectivec

+ (instancetype)create:(id<PublisherDelegate> _Nullable)delegate;
```
创建 Publisher 实例。<br>
如果当前线程中未创建实例，那么你必须先调用此 API，以使用 WTN 提供的各种音视频能力。  <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| delegate | **id<PublisherDelegate>** | 参见 [PublisherDelegate](callback.md#publisherdelegate) |

**返回值**
Publisher：创建成功。返回一个可用的 [Publisher](#publisher) 实例  


**注意**

+ 你可以创建多个 Publisher 实例，以发布多个音视频流。
+ 你应注意保持 PublisherDelegate 的生命周期必须大于 Publisher 的生命周期，即 PublisherDelegate 必须在调用 create 之前创建，在调用 [destroy](#Publisher-destroy) 之后销毁。


<span id="Publisher-destroy"></span>
#### destroy
```objectivec

- (void)destroy;
```
销毁由 [create:](#Publisher-create) 所创建的 Publisher 实例，并释放所有相关资源


**注意**

+ 请确保需要销毁的 [Publisher](#publisher) 实例相关的业务场景全部结束后，才调用此方法。  
+ 该方法在调用之后，会销毁所有和此 [Publisher](#publisher) 实例相关的内存，并且停止与服务端的任何交互。  
+ 调用本方法会启动 SDK 退出逻辑。线程会保留，直到退出逻辑完成。


<span id="Publisher-setvideoconfig"></span>
#### setVideoConfig:
```objectivec

- (int)setVideoConfig:(VideoConfig * _Nonnull)videoConfig;
```
设置视频采集参数

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| videoConfig | **VideoConfig*** | 视频采集参数。参看 [VideoConfig](keytype.md#videoconfig)。 |

**返回值**

+ 0: 成功  
+ < 0: 失败  


**注意**

+ 必须在调用 [startVideoCapture](#Publisher-startvideocapture) 前调用此接口。
+ 建议同一设备上的不同 Publisher 实例使用相同的视频采集参数。


<span id="Publisher-setlocalview"></span>
#### setLocalView:
```objectivec

- (int)setLocalView:(UIView * _Nullable)view;
```
设置本地视频渲染时使用的视图。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| view | **UIView*** | 视图对象。必须保证此对象处于有效的生命周期内。 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 如果需要解除绑定，你可以调用本方法传入空视图。
+ 必须在调用 [startVideoCapture](#Publisher-startvideocapture) 前调用此接口。


<span id="Publisher-startvideocapture"></span>
#### startVideoCapture
```objectivec

- (int)startVideoCapture;
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
```objectivec

- (int)stopVideoCapture;
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
```objectivec

- (int)switchCamera;
```
使用内部采集时，切换使用的摄像头：前置或后置摄像头。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 默认使用前置摄像头。
+ 如果你正在使用相机进行视频采集，切换操作当即生效；如果未处于采集状态，后续开启摄像头采集时，会打开设定的摄像头。


<span id="Publisher-startaudiocapture"></span>
#### startAudioCapture
```objectivec

- (int)startAudioCapture;
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
```objectivec

- (int)stopAudioCapture;
```
关闭麦克风采集。

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startAudioCapture](#Publisher-startaudiocapture) 可以开启麦克风采集。  
+ 如果不调用此方法停止麦克风采集，则只有当销毁 Publisher 实例时，麦克风采集才会停止。


<span id="Publisher-startpublish"></span>
#### startPublish:
```objectivec

- (int)startPublish:(NSString*)url;
```
发布音视频流

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **NSString*** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，PUSH 方法，appID，你设定的 streamID, token 和 TokenType 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/push/<appID>/<streamID>?Token=<token>&TokenType=Bearer` |

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 对于一个 Publisher 实例，仅能同时发布一路音视频流。重复调用此接口会失败。
+ 无论是否开启音视频采集，你都可以调用此接口开启发布。
+ 调用此接口后，会收到 [publisher:onPublishStateChanged:reason:](callback.md#PublisherDelegate-publisher-onpublishstatechanged-reason)。
+ 调用 [stopPublish](#Publisher-stoppublish) 取消发布。


<span id="Publisher-stoppublish"></span>
#### stopPublish
```objectivec

- (int)stopPublish;
```
停止发布音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败


**注意**

+ 调用 [startPublish:](#Publisher-startpublish) 发布音视频流。
+ 调用此接口后，会收到 [publisher:onPublishStateChanged:reason:](callback.md#PublisherDelegate-publisher-onpublishstatechanged-reason)。


<span id="Publisher-ispublishing"></span>
#### isPublishing
```objectivec

- (bool)isPublishing;
```
查询音视频流发布状态

**返回值**

+ true：发布中 
+ false：未发布 


## Subscriber
```objectivec
@interface Subscriber : NSObject
```

Subscriber Class


### 静态函数

| 返回 | 名称 |
| --- | --- |
| **instancetype** | [create:](#Subscriber-create) |


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [destroy](#Subscriber-destroy) |
| **int** | [setRemoteView:](#Subscriber-setremoteview) |
| **int** | [setEnableSpeaker:](#Subscriber-setenablespeaker) |
| **int** | [startSubscribe:](#Subscriber-startsubscribe) |
| **int** | [stopSubscribe](#Subscriber-stopsubscribe) |
| **bool** | [isSubscribed](#Subscriber-issubscribed) |


### 函数说明
<span id="Subscriber-create"></span>
#### create:
```objectivec

+ (instancetype)create:(id<SubscriberDelegate> _Nullable)delegate;
```
创建 Subscriber 实例。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| delegate | **id<SubscriberDelegate>** | 参见 [SubscriberDelegate](callback.md#subscriberdelegate)。 |

**返回值**
Subscriber：创建成功。返回一个可用的 [Subscriber](#subscriber) 实例  


**注意**

+ 你应注意保持 SubscriberDelegate 的生命周期必须大于 Subscriber 的生命周期，即 handler 必须在调用 create 之前创建，在调用 [destroy](#Subscriber-destroy) 之后销毁。  
+ 你可以创建多个 Subscriber 实例，以订阅多个音视频流。


<span id="Subscriber-destroy"></span>
#### destroy
```objectivec

- (void)destroy;
```
销毁由 [create:](#Subscriber-create) 所创建的 Subscriber 实例，并释放所有相关资源


**注意**

+ 请确保需要销毁的 [Subscriber](#subscriber) 实例相关的业务场景全部结束后，才调用此方法  
+ 该方法在调用之后，会销毁所有和此 [Subscriber](#subscriber) 实例相关的内存，并且停止与服务端的任何交互  
+ 调用本方法会启动 SDK 退出逻辑。线程会保留，直到退出逻辑完成。


<span id="Subscriber-setremoteview"></span>
#### setRemoteView:
```objectivec

- (int)setRemoteView:(UIView*)view;
```
设置渲染远端视频流时使用的视图

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| view | **UIView*** | 视图对象。必须保证此对象处于有效的生命周期内。 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 你必须在订阅前，设置渲染视图。 
+ 如果需要解除绑定，你可以调用本方法传入空视图。


<span id="Subscriber-setenablespeaker"></span>
#### setEnableSpeaker:
```objectivec

- (int)setEnableSpeaker:(BOOL)enable;
```
切换播放音频流时使用的听筒/扬声器 <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| enable | **BOOL** | • true：扬声器 <br/>• false：听筒 |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 默认使用扬声器。 
+ 此设置在播放音频流前后都可以生效。


<span id="Subscriber-startsubscribe"></span>
#### startSubscribe:
```objectivec

- (int)startSubscribe:(NSString*)url;
```
开启订阅音视频流。  <br>
调用该方法后，本地用户会收到 [subscriber:onSubscribeStateChanged:reason:](callback.md#SubscriberDelegate-subscriber-onsubscribestatechanged-reason) 的回调。  <br>

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| url | **NSString*** | WHIP 请求的 url。<br/>url 由 WTN 固定域名 `https://wtn.volcvideo.com`，PULL 方法，appID，和你设定的 streamID 拼接得到。 <br/>形如：`https://wtn.volcvideo.com/pull/<appID>/<streamID>` |

**返回值**

+ 0: 成功  
+ -1: 失败。 


**注意**

+ 一个 Subscriber 实例仅可以订阅一路音视频流。
+ 订阅音视频流前，请先确保此流已发布。如果未发布，会订阅失败。 
+ 订阅前需要先调用 [setRemoteView:](#Subscriber-setremoteview) 设置视图。 
+ 调用 [stopSubscribe](#Subscriber-stopsubscribe) 可以停止订阅。 


<span id="Subscriber-stopsubscribe"></span>
#### stopSubscribe
```objectivec

- (int)stopSubscribe;
```
停止订阅音视频流

**返回值**

+ 0：方法调用成功  
+ < 0：方法调用失败  


**注意**

+ 调用 [startSubscribe:](#Subscriber-startsubscribe) 发布音视频流。
+ 调用该方法后，会收到 [subscriber:onSubscribeStateChanged:reason:](callback.md#SubscriberDelegate-subscriber-onsubscribestatechanged-reason) 的回调。


<span id="Subscriber-issubscribed"></span>
#### isSubscribed
```objectivec

- (bool)isSubscribed;
```
查询音视频流订阅状态

**返回值**

+ true：正在订阅 
+ false：未订阅 


