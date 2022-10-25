## PublishState
```objectivec
typedef NS_ENUM(NSInteger, PublishState)
```

音视频流发布状态


#### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| StartPublishSuccess | 0 | 开始发布成功 |
| StartPublishFailed | 1 | 开始发布失败 |
| StopPublishSuccess | 2 | 停止发布成功 |
| StopPublishFailed | 3 | 停止发布失败 |


## SubscribeState
```objectivec
typedef NS_ENUM(NSInteger, SubscribeState)
```

音视频流订阅状态


#### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| StartSubscribeSuccess | 0 | 开始订阅成功 |
| StartSubscribeFailed | 1 | 开始订阅失败 |
| StopSubscribeSuccess | 2 | 停止订阅成功 |
| StopSubscribeFailed | 3 | 停止订阅失败 |


## Reason
```objectivec
typedef NS_ENUM(NSInteger, Reason)
```

订阅/发布成功或失败的原因


#### 枚举值

| 类型 | 值 | 说明 |
| --- | --- | --- |
| Success | 0 | 成功 |
| ParameterError | 1 | 失败。参数错误。 |
| TokenError | 2 | 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。 |
| ResourcesNotFound | 3 | 失败。未找到资源。 |
| ServerInternalError | 4 | 失败。服务器内部错误。请重试。 |
| SDKInternalError | 5 | 失败。SDK 内部错误。请重试。 |
| NetworkError | 6 | 失败。网络错误。 |
| UnknowError | 7 | 失败。未知错误。请重试。 |


## VideoConfig
```objectivec
@interface VideoConfig : NSObject
```

视频配置


### 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [width](#VideoConfig-width) |
| **int** | [height](#VideoConfig-height) |
| **int** | [fps](#VideoConfig-fps) |


### 变量说明
<span id="VideoConfig-width"></span>
#### width
```objectivec
@property (nonatomic, assign) int width;
```
宽度（px）。默认值是 640。


<span id="VideoConfig-height"></span>
#### height
```objectivec
@property (nonatomic, assign) int height;
```
高度（px）。默认值是 960。


<span id="VideoConfig-fps"></span>
#### fps
```objectivec
@property (nonatomic, assign) int fps;
```
最大帧率（fps）。默认值是 15。



