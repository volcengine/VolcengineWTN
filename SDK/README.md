# Readme

此项目是使用 WHIP / WHEP 协议，基于 WebRTC 原生库二次封装，在客户端上实现的通过 WTN（WebRTC Transmission Network）推拉流的 Android & iOS SDK。
你可以参考此项目，自行搭建通过 WTN 推拉流的客户端 SDK，也可以直接使用此 SDK，在客户端上接入 WTN 实现推拉流。

## 功能特性
此项目包含以下功能：

-  推流端：
   -  支持摄像头、麦克风、扬声器、和听筒的开关或切换
   -  支持视频分辨率、帧率设置
   -  开始/结束推流
   -  支持推流事件监听
- 拉流端：
  - 开始/结束拉流
  - 支持拉流事件监听

## 编译客户端 SDK
参看 [编译客户端 SDK](doc/Build_Instruction.md)。

## API 参考
参看 [Android API 参考](doc/Android/overview.md) 和 [iOS API 参考](doc/iOS/overview.md)。

## 联系我们
如果您有任何问题或建议，请联系 webrtc@bytedance.com。
