cmake_minimum_required(VERSION 3.10.0)

# add compiler macros
# Release and Debug cofiguration is the same
target_compile_definitions(${TARGET_NAME} PRIVATE WEBRTC_POSIX WEBRTC_IOS WEBRTC_MAC)

set(PUB_HEADERS
        sdk/ios/veWTNDefines.h
        sdk/ios/Publisher.h
        sdk/ios/Subscriber.h
        )
set_target_properties(${TARGET_NAME} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A
        PUBLIC_HEADER "${PUB_HEADERS}"
        MACOSX_FRAMEWORK_IDENTIFIER com.vewtn
        MACOSX_FRAMEWORK_INFO_PLIST ${VEWTN_ROOT_DIR}/sdk/ios/Info.plist
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_DYLIB_INSTALL_NAME_BASE "@rpath"
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Frameworks @loader_path/Frameworks"
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES"
        XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN "YES"
        XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "YES"
        XCODE_ATTRIBUTE_STRIPFLAGS[variant=Release] "-x"
        XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Debug] "NO"
        XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP[variant=Debug] "NO"
        XCODE_ATTRIBUTE_COPY_PHASE_STRIP[variant=Release] "YES"
        XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT[variant=Debug] "NO"
        XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT[variant=Release] "YES"
        XCODE_ATTRIBUTE_STRIP_STYLE[variant=Debug] "debugging"
        XCODE_ATTRIBUTE_STRIP_STYLE[variant=Release] "non-global"
        XCODE_ATTRIBUTE_STRIP_SWIFT_SYMBOLS[variant=Debug] "NO"
        XCODE_ATTRIBUTE_STRIP_SWIFT_SYMBOLS[variant=Release] "YES"
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
        XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS "YES"
        XCODE_ATTRIBUTE_GCC_ENABLE_CPP_RTTI "NO"
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17"
        XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
        XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Release] "z"
        )

# set output name
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "veWTN")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
    target_compile_definitions(${TARGET_NAME} PRIVATE WEBRTC_HAS_NEON)
endif()

# framework
target_link_libraries(${TARGET_NAME} PRIVATE resolv
        "-framework UIKit"
        "-framework CoreTelephony"
        "-framework SystemConfiguration"
        "-framework Accelerate"
        "-framework IOKit"
        "-framework CFNetwork"
        "-framework CoreGraphics"
        "-framework Foundation"
        "-framework MetalKit"
        "-framework AVFoundation"
        "-framework AudioToolBox"
        "-framework CoreVideo"
        "-framework WebKit"
        "-framework CoreMedia"
        "-framework VideoToolbox"
        "-framework Metal"
        )

# apply xcode build settings
byte_set_xcode_property(${TARGET_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS "YES" All)
set(GENERATE_MASTER_OBJECT_FILE "YES")
set_cache(GENERATE_MASTER_OBJECT_FILE)
byte_set_xcode_property(${TARGET_NAME} GENERATE_MASTER_OBJECT_FILE "${GENERATE_MASTER_OBJECT_FILE.cache}" All)
byte_set_xcode_property(${TARGET_NAME} IPHONEOS_DEPLOYMENT_TARGET "9.0" All)

# include sdk directory
target_include_directories(${TARGET_NAME} PRIVATE ${VEWTN_ROOT_DIR}/src/sdk/ios)

# interface symbol export
set_target_properties(${TARGET_NAME} PROPERTIES EXPORTED_SYMBOLS_FILE "${VEWTN_ROOT_DIR}/src/sdk/ios/export.txt")
