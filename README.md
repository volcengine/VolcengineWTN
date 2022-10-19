# VolcengineWTN

## WTN 介绍

[火山引擎 WebRTC 传输网络](https://www.volcengine.com/docs/6752/122560)（VolcEngine WebRTC Transmission Network，简写 WTN）可以提供全球互联网范围内高质量、低延时的实时音视频通信能力。

通过标准 WebRTC 协议接入 WTN，你可以实现丰富多彩的实时音视频应用。

> 如果查看页面时，显示页面不存在，可能是因为你没有权限。请联系技术支持人员添加权限。

## 服务端 API

WTN 提供了基于 WebRTC 标准信令交互协议的 HTTP 接口。你可以通过这些接口，完成 Offer/Answer SDP 交换，使用 WebRTC JavaScript/Native 原生库完成 WTN 媒体服务建连。建连后，你可以推拉音视频流。

关于这些 API 的详细信息，参看 API 文档：
-  [创建推流资源](https://www.volcengine.com/docs/6752/132482)；
-  [销毁推流资源](https://www.volcengine.com/docs/6752/132483)；
-  [创建拉流资源](https://www.volcengine.com/docs/6752/132484)；
-  [销毁拉流资源](https://www.volcengine.com/docs/6752/132485)。

## 社区版 SDK

WTN 提供了基于 [WHIP](https://www.ietf.org/id/draft-ietf-wish-whip-04.html)/[WHEP](https://www.ietf.org/id/draft-murillo-whep-00.html) 的 HTTP API 和 WebRTC 开源库封装了社区版 SDK。你可以参考此项目，在客户端接入 WTN，实现音视频推拉流功能。

| 平台 | 项目工程 | 编译 SDK | API 文档 |
| --- | --- | --- | --- |
| Android | [android](SDK/sdk/android/) | [编译 SDK](SDK/doc/Build_Instruction.md) | [API 文档](SDK/doc/Android/overview.md) |
| iOS | [ios](SDK/sdk/ios/) | [编译 SDK](SDK/doc/Build_Instruction.md) | [API 文档](SDK/doc/iOS/overview.md) |

开发者可以参照 Demo，使用此 SDK 实现音视频通信应用。

## 示例 Demo

项目中也提供了 Android， iOS，和 web 平台的 Demo 项目和 OBS 插件，参考对应文档进行试用：

| 平台 | 跑通 Demo 文档 |
| --- | --- |
| Android | [跑通 Android Demo 文档](https://www.volcengine.com/docs/6752/132445) |
| iOS | [跑通 iOS Demo 文档](https://www.volcengine.com/docs/6752/132446) |
| Web | [跑通 Web Demo 文档](https://www.volcengine.com/docs/6752/132447) |
| OBS | [使用 OBS 插件推流](https://www.volcengine.com/docs/6752/132448) |
