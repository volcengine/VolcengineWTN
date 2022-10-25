## PublishState
```java
public enum com.vewtn.PublisherEventHandler.PublishState
```

音视频流发布状态


### 枚举值

| 类型 | 说明 |
| --- | --- |
| **StartPublishSuccess** | 开始发布成功 |
| **StartPublishFailed** | 开始发布失败 |
| **StopPublishSuccess** | 停止发布成功 |
| **StopPublishFailed** | 停止发布失败 |

## Reason
```java
public enum com.vewtn.PublisherEventHandler.Reason
```

发布成功或失败的原因


### 枚举值

| 类型 | 说明 |
| --- | --- |
| **Success** | 成功 |
| **ParameterError** | 失败。参数错误。 |
| **TokenError** | 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。 |
| **ResourcesNotFound** | 失败。未找到资源。 |
| **ServerInternalError** | 失败。服务器内部错误。请重试。 |
| **SDKInternalError** | 失败。SDK 内部错误。请重试。 |
| **NetworkError** | 失败。网络错误。 |
| **UnknowError** | 失败。未知错误。请重试。 |
| **DeviceNoPermission** | 失败。摄像头或者麦克风无权限。 |


## SubscribeState
```java
public enum com.vewtn.SubscriberEventHandler.SubscribeState
```

音视频流订阅状态


### 枚举值

| 类型 | 说明 |
| --- | --- |
| **StartSubscribeSuccess** | 开始订阅成功 |
| **StartSubscribeFailed** | 开始订阅失败 |
| **StopSubscribeSuccess** | 停止订阅成功 |
| **StopSubscribeFailed** | 停止订阅失败 |

## Reason
```java
public enum com.vewtn.SubscriberEventHandler.Reason
```

订阅成功或失败的原因


### 枚举值

| 类型 | 说明 |
| --- | --- |
| **Success** | 成功 |
| **ParameterError** | 失败。参数错误。 |
| **TokenError** | 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。 |
| **ResourcesNotFound** | 失败。未找到资源。 |
| **ServerInternalError** | 失败。服务器内部错误。请重试。 |
| **SDKInternalError** | 失败。SDK 内部错误。请重试。 |
| **NetworkError** | 失败。网络错误。 |
| **UnknowError** | 失败。未知错误。请重试。 |
| **DeviceNoPermission** | 失败。摄像头或者麦克风无权限。 |


## VideoConfig
```java
public class com.vewtn.VideoConfig
```

视频配置


### 成员变量

| 类型 | 名称 |
| --- | --- |
| **int** | [width](#VideoConfig-width) |
| **int** | [height](#VideoConfig-height) |
| **int** | [maxFPS](#VideoConfig-maxfps) |


### 变量说明
<span id="VideoConfig-width"></span>
#### width
```java
public int com.vewtn.VideoConfig.width = 640;
```
宽度（px）。默认值是 640。


<span id="VideoConfig-height"></span>
#### height
```java
public int com.vewtn.VideoConfig.height = 960;
```
高度（px）。默认值是 640。


<span id="VideoConfig-maxfps"></span>
#### maxFPS
```java
public int com.vewtn.VideoConfig.maxFPS = 15;
```
最大帧率（fps）。默认值是 15。



