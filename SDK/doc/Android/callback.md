## PublisherEventHandler
```java
public abstract class com.vewtn.PublisherEventHandler
```

发布端事件回调接口


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [onPublishStateChanged](#PublisherEventHandler-onpublishstatechanged) |


### 函数说明
<span id="PublisherEventHandler-onpublishstatechanged"></span>
#### onPublishStateChanged
```java
public void com.vewtn.PublisherEventHandler.onPublishStateChanged(
    PublishState state,
    Reason reason)
```
音视频流的发布状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| state | **PublishState** | 当前发布状态，参看 [PublishState](keytype.md#publishstate)。 |
| reason | **Reason** | 发布状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


## SubscriberEventHandler
```java
public abstract class com.vewtn.SubscriberEventHandler
```

订阅端事件回调接口


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [onSubscribeStateChanged](#SubscriberEventHandler-onsubscribestatechanged) |


### 函数说明
<span id="SubscriberEventHandler-onsubscribestatechanged"></span>
#### onSubscribeStateChanged
```java
public void com.vewtn.SubscriberEventHandler.onSubscribeStateChanged(
    SubscribeState state,
    Reason reason)
```
音视频流的订阅状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| state | **SubscribeState** | 当前订阅状态，参看 [SubscribeState](keytype.md#subscribestate)。 |
| reason | **Reason** | 订阅状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


