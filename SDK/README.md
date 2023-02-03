# WTN 社区版 SDK

> For English version, See [WTN Community SDK](README_en.md).

此项目是使用 [WHIP](https://datatracker.ietf.org/doc/draft-ietf-wish-whip/)/[WHEP](https://datatracker.ietf.org/doc/draft-murillo-whep/) 协议，基于 WebRTC 静态库二次封装，在客户端上实现的通过 WTN（WebRTC Transmission Network）发布订阅流的 Android & iOS SDK。
你可以参考此项目，自行搭建通过 WTN 发布订阅流的客户端 SDK，也可以直接使用此 SDK，在客户端上接入 WTN 实现流的发布订阅。

## 功能特性
此项目包含以下功能：

- 发布端：
   - 支持创建多个发布
   - 支持摄像头、麦克风、扬声器、和听筒的开关或切换
   - 支持视频分辨率、帧率设置
   - 开始/结束发布
   - 支持推送外部源 YUV/H264/PCM 数据
   - 支持发布事件监听
- 订阅端：
  - 支持创建多个订阅
  - 开始/结束订阅
  - 支持订阅外部源 YUV/H264/PCM 数据回调
  - 支持订阅事件监听

## 编译客户端 SDK
参看 [编译客户端 SDK](doc/zh/Build_Instruction.md)。

## API 参考
参看 [Android API 参考](doc/zh/Android/overview.md) 和 [iOS API 参考](doc/zh/iOS/overview.md)。

## 联系我们
如果您有任何问题或建议，请联系 webrtc@bytedance.com。
