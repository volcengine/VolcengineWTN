## PublisherDelegate
```objectivec
@protocol PublisherDelegate <NSObject>
```

发布端事件回调接口


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [publisher:onPublishStateChanged:reason:](#PublisherDelegate-publisher-onpublishstatechanged-reason) |


### 函数说明
<span id="PublisherDelegate-publisher-onpublishstatechanged-reason"></span>
#### publisher:onPublishStateChanged:reason:
```objectivec

- (void)publisher:(Publisher *)publisher onPublishStateChanged:(PublishState)state reason:(Reason)reason;
```
音视频流的发布状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| publisher | **Publisher*** | Publisher 实例 |
| state | **PublishState** | 当前发布状态，参看 [PublishState](keytype.md#publishstate)。 |
| reason | **Reason** | 发布状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


## SubscriberDelegate
```objectivec
@protocol SubscriberDelegate <NSObject>
```

订阅端事件回调接口


### 成员函数

| 返回 | 名称 |
| --- | --- |
| **void** | [subscriber:onSubscribeStateChanged:reason:](#SubscriberDelegate-subscriber-onsubscribestatechanged-reason) |


### 函数说明
<span id="SubscriberDelegate-subscriber-onsubscribestatechanged-reason"></span>
#### subscriber:onSubscribeStateChanged:reason:
```objectivec

- (void)subscriber:(Subscriber *)subscriber onSubscribeStateChanged:(SubscribeState)state reason:(Reason)reason;
```
音视频流的订阅状态发生改变时，收到此回调。

**传入参数**

| 参数名 | 类型 | 说明 |
| --- | --- | --- |
| subscriber | **Subscriber*** | 参见 [Subscriber](api.md#subscriber)。 |
| state | **SubscribeState** | 当前订阅状态，参看 [SubscribeState](keytype.md#subscribestate)。 |
| reason | **Reason** | 订阅状态发生改变的原因，参看 [Reason](keytype.md#reason)。 |


