# VolcengineWTN

> 对中文版文档，参考 [VolcengineWTN](README.md)。

## Overview

[WTN](https://www.volcengine.com/docs/6752/122560)（VolcEngine WebRTC Transmission Network）is built on a global real-time transmission network that supports billion-level DAU, and provides high-quality, low-latency real-time audio and video communication capabilities worldwide.

By accessing WTN, you can implement all real-time audio and video applications.

## API

WTN provides HTTP interfaces based on the WebRTC standard signaling interaction protocol. You can offering / answering SDP exchange through these interfaces, and use WebRTC JavaScript/Native native library to access WTN media service. After establishing a connection, you can publish/subscribe audio and video streams.

For details, see API reference:
-  [创建发布资源](https://www.volcengine.com/docs/6752/132482)；
-  [销毁发布资源](https://www.volcengine.com/docs/6752/132483)；
-  [设置发布能力](https://www.volcengine.com/docs/6752/161924)；
-  [创建订阅资源](https://www.volcengine.com/docs/6752/132484)；
-  [销毁订阅资源](https://www.volcengine.com/docs/6752/132485)；
-  [设置订阅能力](https://www.volcengine.com/docs/6752/161925)。

## Community SDK

WTN provides community SDK based on WebRTC native SDK. You can integrate the SDK in your client App to access WTN.

The community SDK implements the ability to publish and subscribe to streams and control a small number of audio and video device functions. You can also refer to the implementation of the community SDK and build you own client SDK.

| Platform | Project | Building SDK | API Reference |
| --- | --- | --- | --- |
| Android | [Android](SDK/sdk/android/) | [Compile SDK](SDK/doc/en/Build_Instruction.md) | [API Reference](SDK/doc/en/Android/overview.md) |
| iOS | [iOS](SDK/sdk/ios/) | [Compile SDK](SDK/doc/en/Build_Instruction.md) | [API Reference](SDK/doc/en/iOS/overview.md) |

## Demo Projects

WTN also provides demo projects for Android, iOS, and Web platforms and OBS plug-in.

You can refer to the Demo project to integrate the community SDK in the application of the corresponding platform; you can also use the OBS plug-in to realize functions such as live broadcast. Refer to the corresponding document to try it out:

| Platform | Implement Demo Project |
| --- | --- |
| Android | [跑通 Android Demo 文档](https://www.volcengine.com/docs/6752/132445) |
| iOS | [跑通 iOS Demo 文档](https://www.volcengine.com/docs/6752/132446) |
| Web | [跑通 Web Demo 文档](https://www.volcengine.com/docs/6752/132447) |
| OBS | [使用 OBS 插件发布流](https://www.volcengine.com/docs/6752/132448) |

