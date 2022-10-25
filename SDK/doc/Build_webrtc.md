# 编译 WebRTC 静态库

WTN 客户端 SDK 项目基于 WebRTC SDK 实现。你需要参考本文，编译在 Android 和 iOS 平台上适用的 WebRTC 静态库。
注意，要编译 Android 和 iOS 平台上使用的 WebRTC SDK，你需要分别在 Linux 和 macOS 系统上完成。

## 安装 depot_tools

[depot_tools](https://chromium.googlesource.com/chromium/tools/depot_tools.git) 是 Chromium 开发的常用工具包。在编译 Android 和 iOS 平台上使用的 WebRTC SDK 前，你都必须在开发机器上，获取 depot_tools 项目并配置环境变量：

```
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git 

# 配置环境变量
echo "export PATH=$PWD/depot_tools:\$PATH" >> $HOME/.bash_profile
source $HOME/.bash_profile

fetch --help
```

## 编译 WebRTC SDK for Android

你需要在 Linux 系统上，为 WTN SDK for Android 编译 WebRTC SDK。

```
// 获取 WebRTC 项目源码
mkdir webrtc && cd webrtc

fetch --nohooks webrtc_android

cd src
git checkout branch-heads/5050    // 目前使用的分支

gclient sync
```

```
// 构建 armeabi-v7a
gn gen out/Release_arm --args='is_debug=false target_os="android" target_cpu="arm" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false'

// 编译
autoninja -C out/Release_arm
```

```
// 构建 arm64-v8a
gn gen out/Release_arm64 --args='is_debug=false target_os="android" target_cpu="arm64" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false'

// 编译
autoninja -C out/Release_arm64
```

### 编译产物

静态库：
- arm：`out/Release_arm/obj/libwebrtc.a`
- arm64：`out/Release_arm64/obj/libwebrtc.a` 

jar包：
- arm：`out/Release_arm/lib.java`
- arm64：`out/Release_arm64/lib.java`

头文件：可以通过以下脚本获取：

```
cd src
mkdir headers
find . -name "*.h" | xargs -I {} cp --parents {} ./headers
```

## 编译 WebRTC SDK for iOS

你需要在 macOS 系统上，为 WTN SDK for iOS 编译 WebRTC SDK。

```
// 获取 WebRTC 项目源码
mkdir webrtc && cd webrtc

fetch --nohooks webrtc_ios

cd src
git checkout branch-heads/5050    // 目前使用的分支

gclient sync
```

```
// 构建 arm64
gn gen out/Release_arm64 --args='is_debug=false target_os="ios" target_cpu="arm64" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false enable_dsyms=true rtc_enable_protobuf=false ios_deployment_target="9.0" use_lld=false' --ide=xcode

// 编译
autoninja -C out/Release_arm64
```

```
// 构建 armv7
gn gen out/Release_arm --args='is_debug=false target_os="ios" target_cpu="arm" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false enable_dsyms=true rtc_enable_protobuf=false ios_deployment_target="9.0" use_lld=false' --ide=xcode

// 编译
autoninja -C out/Release_arm
```

### 编译产物

```
// 静态库，默认生成的静态库objc相关符号缺失，使用以下方式生成静态库：
cd out/Release_arm or out/Release_arm64
find . -name '*.o' | xargs xcrun -r libtool -no_warning_for_no_symbols -static -o libwebrtc.a

// 将不同架构平台的静态库合并为完整的通用静态库
cd src
lipo -create out/Release_arm64/libwebrtc.a out/Release_arm/libwebrtc.a -output libwebrtc.a

// 头文件通过脚本获取：
cd src
mkdir headers
find . -name "*.h" | xargs -I {} rsync -R {} ./headers
```
