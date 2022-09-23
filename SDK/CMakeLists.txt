# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.10.0)

# Debug: Display current project root source
message(STATUS "source dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "current list dir: ${CMAKE_CURRENT_LIST_DIR}")

project(veWTN)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/pre_config.cmake)

# Do not create a zero_check target
set(CMAKE_SUPPRESS_REGENERATION TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (${CMAKE_VERSION} VERSION_GREATER "3.13.0")
    cmake_policy(SET CMP0079 NEW)
endif()

set(RTC_LIBRARY_TYPE STATIC)
if (NOT RTC_LIBRARY_STATIC.cache)
    set(RTC_LIBRARY_TYPE SHARED)
endif ()

if ( CMAKE_BUILD_TYPE STREQUAL "" )
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Let find_library search custom path on Android and iOS
if (RTC_PLATFORM_IOS.cache OR RTC_PLATFORM_ANDROID.cache)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
endif()

byte_dump_value(CMAKE_BUILD_TYPE)
byte_dump_value(CMAKE_VS_PLATFORM_NAME)
byte_dump_value(CMAKE_RUNTIME_LIBRARY)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME veWTN)

set(VEWTN_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add source
include(${VEWTN_ROOT_DIR}/cmake/sources.cmake)

# create target
if (RTC_PLATFORM_ANDROID.cache)
    add_library(${TARGET_NAME} SHARED ${SOURCES})
elseif (RTC_PLATFORM_IOS.cache)
    add_library(${TARGET_NAME} SHARED ${SOURCES} 
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/AVCaptureSession+DevicePosition.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/UIDevice+RTCDevice.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/UIDevice+H264Profile.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCAudioSession+Configuration.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCVideoCodecInfo+Private.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCMTLI420Renderer.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCMTLNV12Renderer.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCVideoEncoderSettings+Private.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/RTCEncodedImage+Private.o"
        "${VEWTN_ROOT_DIR}/third_party/webrtc/ios/NSString+StdString.o"
        )
endif ()
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# set binary output dir
byte_apply_default_target_options(${TARGET_NAME})
byte_config_output_dir(${TARGET_NAME})

# third party headers and prebuild
include(${VEWTN_ROOT_DIR}/cmake/third_party.cmake)

# add header dir
target_include_directories(${TARGET_NAME} PRIVATE ${VEWTN_ROOT_DIR}/src)

# platform process
if (RTC_PLATFORM_IOS.cache)
    include(${VEWTN_ROOT_DIR}/cmake/ios.cmake)
elseif (RTC_PLATFORM_ANDROID.cache)
    include(${VEWTN_ROOT_DIR}/cmake/android.cmake)
endif ()
