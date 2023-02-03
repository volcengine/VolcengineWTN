# WTN Community SDK

> 对中文版文档，参考 [WTN 社区版 SDK](README.md)。

This project uses the WHIP / WHEP protocol, encapsulates WebRTC native library, and implements Android SDK & iOS SDK that access WTN (WebRTC Transmission Network) on the client.
You can refer to this project to build your own client SDK for access to WTN, or you can directly use this SDK on the client to implement publishing and subscribing media streams.

## Features

The project implements the following features:

- Publishing:
   - Creating multiple publishing streams;
   - Switching from multiple cameras;
   - Switching among microphone, speaker, and earpiece;
   - Setting video resolution and frame rate;
   - Setting the mute status when publishing;
   - Publishing external source YUV/H264/PCM data;
   - Monitoring publishing status. 
- Subscribing:
  - Creating multiple subscribing streams;
  - Subscribing external source YUV/H264/PCM data;
  - Monitoring subscribing status. 

## Compile the client SDK

See [Compile Client SDK](doc/en/Build_Instruction.md).

## API Reference

See [Android API Overview](doc/en/Android/overview.md) 和 [iOS API Overview](doc/en/iOS/overview.md).

## Contact us

For any questions or suggestions, contact webrtc@bytedance.com.
