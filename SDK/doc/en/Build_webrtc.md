# Compile the WebRTC static library

The WTN client SDK project is implemented based on the WebRTC static library. Refer to this article to compile the WebRTC static libraries for Android and iOS.
Note that to compile the WebRTC static library for Android and iOS, you need to do it on Linux and macOS systems respectively.

## Install depot_tools

[depot_tools](https://chromium.googlesource.com/chromium/tools/depot_tools.git) is a common toolkit developed by Chromium. Before compiling the WebRTC static library for Android and iOS, you must get the depot_tools project and configure environment variables on the development machine:


```
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git 

# configure environment variables
echo "export PATH=$PWD/depot_tools:\$PATH" >> $HOME/.bash_profile
source $HOME/.bash_profile

fetch --help
```

## Compile WebRTC static library for Android

You need to compile the WebRTC static library for WTN SDK for Android on a Linux system.

```
// Get the source code of the WebRTC project
mkdir webrtc && cd webrtc

fetch --nohooks webrtc_android

cd src
git checkout branch-heads/5050    // Current branch

gclient sync
```

```
// Construct armeabi-v7a
gn gen out/Release_arm --args='is_debug=false target_os="android" target_cpu="arm" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false'

// Compile
autoninja -C out/Release_arm
```

```
// Construct arm64-v8a
gn gen out/Release_arm64 --args='is_debug=false target_os="android" target_cpu="arm64" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false'

// Compile
autoninja -C out/Release_arm64
```

### Compiled product

Static library:
- arm：`out/Release_arm/obj/libwebrtc.a`
- arm64：`out/Release_arm64/obj/libwebrtc.a` 

jar files：
- arm：`out/Release_arm/lib.java`
- arm64：`out/Release_arm64/lib.java`

Header files: get by the following script:

```
cd src
mkdir headers
find . -name "*.h" | xargs -I {} cp --parents {} ./headers
```

## Compile WebRTC static library for iOS

You need to compile WebRTC static library for WTN SDK for iOS on macOS system.

```
// Get the source code of the WebRTC project
mkdir webrtc && cd webrtc

fetch --nohooks webrtc_ios

cd src
git checkout branch-heads/5050    // Current branch

gclient sync
```

```
// Construct arm64
gn gen out/Release_arm64 --args='is_debug=false target_os="ios" target_cpu="arm64" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false enable_dsyms=true rtc_enable_protobuf=false ios_deployment_target="9.0" use_lld=false' --ide=xcode

// Compile
autoninja -C out/Release_arm64
```

```
// Construct armv7
gn gen out/Release_arm --args='is_debug=false target_os="ios" target_cpu="arm" rtc_build_examples=false rtc_include_tests=false use_custom_libcxx=false enable_dsyms=true rtc_enable_protobuf=false ios_deployment_target="9.0" use_lld=false' --ide=xcode

// Compile
autoninja -C out/Release_arm
```

### Compiled product

```
// Use the following method to generate a static library
cd out/Release_arm or out/Release_arm64
find . -name '*.o' | xargs xcrun -r libtool -no_warning_for_no_symbols -static -o libwebrtc.a

// Merge static libraries of different architecture platforms
// into a complete general static library
cd src
lipo -create out/Release_arm64/libwebrtc.a out/Release_arm/libwebrtc.a -output libwebrtc.a

// Header files: get by the following script:
cd src
mkdir headers
find . -name "*.h" | xargs -I {} rsync -R {} ./headers
```

