# Compile Client SDK

You can refer to this project to build a client SDK for WTN.

## Compile WTN SDK for Android
### Prerequisites

Before you compile the WTN SDK for Android, Android Studio must be installed.

### Stpes
#### 1. Add WebRTC static library in the client SDK project

1. Get WebRTC static libraries, including static libraries for arm64-v8a architecture and armeabib-v7a architecture. See [Compile the WebRTC static library](Build_webrtc.md).
2. Add the WebRTC static library file to the client SDK project:
     1. Copy the header files from the WebRTC static library to: `./third_party/webrtc/headers/`
     2. Copy the Android static library to the following path:
          - `./third_party/webrtc/android/armeabi-v7a/libwebrtc.a`
          - `./third_party/webrtc/android/arm64-v8a/libwebrtc.a`
     3. Copy the .jar file to the following path:
          - `./third_party/webrtc/android/jar/libwebrtc.jar`

#### 2. Compile WTN SDK

Open `./sdk/android` with Android Studio.

## Compile WTN SDK for iOS
### Prerequisites

Before you compile the WTN SDK for iOS, Xcode must be installed.

### Steps
#### 1. Add WebRTC static library in the client SDK project

1. Get WebRTC static libraries, including static libraries for arm64 architecture and amrv7 architecture. See [Compile the WebRTC static library](Build_webrtc.md).
2. Add the WebRTC static library file to the client SDK project:
     1. Copy the header files from the WebRTC static library to `./third_party/webrtc/headers/`;
     2. Copy the iOS static library to `./third_party/webrtc/ios/libwebrtc.a`.

#### 2. Compile WTN SDK

cd to `VolcengineWTN/SDK/`, and excute:

```
cmake -GXcode -DRTC_PLATFORM=ios -Bbuild_ios

cd build_ios
open veWTN-All.xcodeproj    // Use Xcode
```

## Project Structure

The structure of the project:

```
├── cmake
│   └── toolchains
├── doc
├── sdk
│   ├── android
│   └── ios
├── src
└── third_party
    ├── boost_1_69_0
    └── webrtc
        ├── android
        ├── headers
        └── ios
```
