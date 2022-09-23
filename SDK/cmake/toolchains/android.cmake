# CopyRight (c) 2020 ByteDance Inc. All Right Reserved.
# Cmake Tookchain for android

# We are now compile an android project
set(CMAKE_SYSTEM_NAME Android)
# Inhibit all of CMake's own NDK handling code.
set(CMAKE_SYSTEM_VERSION 1)

byte_init_default_flags()

# exceptions on/off?
if (NOT RTC_CPP_EXCEPTION.cache)
    byte_add_default_compile_flag(-fno-exceptions)
endif()

# rtti on/off?
if (NOT RTC_CPP_RTTI.cache)
    byte_add_default_compile_flag(-fno-rtti)
endif()

if(ANDROID_ABI MATCHES "^arm")
    set(RTC_SUPPORT_NEON TRUE)
    set(ANDROID_ARM_NEON TRUE)
else()
    set(RTC_SUPPORT_NEON FALSE)
    set(ANDROID_ARM_NEON FALSE)
endif()
set_cache(RTC_SUPPORT_NEON)
set_cache(ANDROID_ARM_NEON)

# the default path of ANDROID_NDK in mac, or manual overwriting it as where the ndk placed.
if(NOT ANDROID_NDK)
    if(DEFINED ENV{ANDROID_NDK_HOME}
        AND IS_DIRECTORY "$ENV{ANDROID_NDK_HOME}")
        set(ANDROID_NDK "$ENV{ANDROID_NDK_HOME}")
    elseif(DEFINED ENV{ANDROID_HOME}
        AND IS_DIRECTORY "$ENV{ANDROID_HOME}/ndk-bundle")
        set(ANDROID_NDK "$ENV{ANDROID_HOME}/ndk-bundle")
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux
        AND IS_DIRECTORY "$ENV{HOME}/Android/Sdk/ndk-bundle")
        set(ANDROID_NDK "$ENV{HOME}/Android/Sdk/ndk-bundle")
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin
        AND IS_DIRECTORY "$ENV{HOME}/Library/Android/sdk/ndk-bundle")
        set(ANDROID_NDK "$ENV{HOME}/Library/Android/sdk/ndk-bundle")
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows
        AND IS_DIRECTORY "$ENV{LOCALAPPDATA}/Android/Sdk/ndk-bundle")
        set(ANDROID_NDK "$ENV{LOCALAPPDATA}/Android/Sdk/ndk-bundle")
    else()
        message(FATAL_ERROR "Android NDK unspecified.")
    endif()
else()
    get_filename_component(ANDROID_NDK "${ANDROID_NDK}" ABSOLUTE)
endif()
file(TO_CMAKE_PATH "${ANDROID_NDK}" ANDROID_NDK)

# Android NDK revision
# Possible formats:
# * r16, build 1234: 16.0.1234
# * r16b, build 1234: 16.1.1234
# * r16 beta 1, build 1234: 16.0.1234-beta1
#
# Canary builds are not specially marked.
file(READ "${ANDROID_NDK}/source.properties" ANDROID_NDK_SOURCE_PROPERTIES)

set(ANDROID_NDK_REVISION_REGEX
    "^Pkg\\.Desc = Android NDK\nPkg\\.Revision = ([0-9]+)\\.([0-9]+)\\.([0-9]+)(-beta([0-9]+))?")
if(NOT ANDROID_NDK_SOURCE_PROPERTIES MATCHES "${ANDROID_NDK_REVISION_REGEX}")
    message(SEND_ERROR "Failed to parse Android NDK revision: ${ANDROID_NDK}/source.properties.\n${ANDROID_NDK_SOURCE_PROPERTIES}")
endif()

set(ANDROID_NDK_MAJOR "${CMAKE_MATCH_1}")
set(ANDROID_NDK_MINOR "${CMAKE_MATCH_2}")
set(ANDROID_NDK_BUILD "${CMAKE_MATCH_3}")
set(ANDROID_NDK_BETA "${CMAKE_MATCH_5}")
if(ANDROID_NDK_BETA STREQUAL "")
    set(ANDROID_NDK_BETA "0")
endif()
set(ANDROID_NDK_REVISION
    "${ANDROID_NDK_MAJOR}.${ANDROID_NDK_MINOR}.${ANDROID_NDK_BUILD}${CMAKE_MATCH_4}")

# Compatibility for configurable variables.
# Compatible with configurable variables from the other toolchain file:
#         https://github.com/taka-no-me/android-cmake
# TODO: We should consider dropping compatibility to simplify things once most
# of our users have migrated to our standard set of configurable variables.
if(ANDROID_TOOLCHAIN_NAME AND NOT ANDROID_TOOLCHAIN)
    if(ANDROID_TOOLCHAIN_NAME MATCHES "-clang([0-9].[0-9])?$")
        set(ANDROID_TOOLCHAIN clang)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "-[0-9].[0-9]$")
        set(ANDROID_TOOLCHAIN gcc)
    endif()
endif()
if(ANDROID_ABI STREQUAL "armeabi-v7a with NEON")
    set(ANDROID_ABI armeabi-v7a)
    set(ANDROID_ARM_NEON TRUE)
elseif(ANDROID_TOOLCHAIN_NAME AND NOT ANDROID_ABI)
    if(ANDROID_TOOLCHAIN_NAME MATCHES "^arm-linux-androideabi-")
        set(ANDROID_ABI armeabi-v7a)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "^aarch64-linux-android-")
        set(ANDROID_ABI arm64-v8a)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "^x86-")
        set(ANDROID_ABI x86)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "^x86_64-")
        set(ANDROID_ABI x86_64)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "^mipsel-linux-android-")
        set(ANDROID_ABI mips)
    elseif(ANDROID_TOOLCHAIN_NAME MATCHES "^mips64el-linux-android-")
        set(ANDROID_ABI mips64)
    endif()
endif()
if(ANDROID_NATIVE_API_LEVEL AND NOT ANDROID_PLATFORM)
    if(ANDROID_NATIVE_API_LEVEL MATCHES "^android-[0-9]+$")
        set(ANDROID_PLATFORM ${ANDROID_NATIVE_API_LEVEL})
    elseif(ANDROID_NATIVE_API_LEVEL MATCHES "^[0-9]+$")
        set(ANDROID_PLATFORM android-${ANDROID_NATIVE_API_LEVEL})
    endif()
endif()
if(DEFINED ANDROID_APP_PIE AND NOT DEFINED ANDROID_PIE)
    set(ANDROID_PIE "${ANDROID_APP_PIE}")
endif()
if(DEFINED ANDROID_NO_UNDEFINED AND NOT DEFINED ANDROID_ALLOW_UNDEFINED_SYMBOLS)
    if(ANDROID_NO_UNDEFINED)
        set(ANDROID_ALLOW_UNDEFINED_SYMBOLS FALSE)
    else()
        set(ANDROID_ALLOW_UNDEFINED_SYMBOLS TRUE)
    endif()
endif()
if(DEFINED ANDROID_SO_UNDEFINED AND NOT DEFINED ANDROID_ALLOW_UNDEFINED_SYMBOLS)
    set(ANDROID_ALLOW_UNDEFINED_SYMBOLS "${ANDROID_SO_UNDEFINED}")
endif()
if(DEFINED ANDROID_FORCE_ARM_BUILD AND NOT ANDROID_ARM_MODE)
    if(ANDROID_FORCE_ARM_BUILD)
        set(ANDROID_ARM_MODE arm)
    else()
        set(ANDROID_ARM_MODE thumb)
    endif()
endif()
if(NDK_CCACHE AND NOT ANDROID_CCACHE)
    set(ANDROID_CCACHE "${NDK_CCACHE}")
endif()

# Default values for configurable variables.
if(NOT ANDROID_TOOLCHAIN)
    set(ANDROID_TOOLCHAIN clang)
elseif(ANDROID_TOOLCHAIN STREQUAL gcc)
    message(FATAL_ERROR "GCC is no longer supported. See "
    "https://android.googlesource.com/platform/ndk/+/master/docs/ClangMigration.md.")
endif()
if(NOT ANDROID_ABI)
    set(ANDROID_ABI armeabi-v7a)
endif()

if(ANDROID_ABI STREQUAL armeabi)
    message(FATAL_ERROR "armeabi is no longer supported. Use armeabi-v7a.")
elseif(ANDROID_ABI MATCHES "^(mips|mips64)$")
    message(FATAL_ERROR "MIPS and MIPS64 are no longer supported.")
endif()

include(${ANDROID_NDK}/build/cmake/platforms.cmake)

# If no platform version was chosen by the user, default to the minimum version
# supported by this NDK.
if(NOT ANDROID_PLATFORM)
    message(STATUS "\
ANDROID_PLATFORM not set. Defaulting to minimum supported version
${NDK_MIN_PLATFORM_LEVEL}.")

    set(ANDROID_PLATFORM "android-${NDK_MIN_PLATFORM_LEVEL}")
endif()

if(ANDROID_PLATFORM STREQUAL "latest")
    message(STATUS
        "Using latest available ANDROID_PLATFORM: ${NDK_MAX_PLATFORM_LEVEL}.")
    set(ANDROID_PLATFORM "android-${NDK_MAX_PLATFORM_LEVEL}")
    string(REPLACE "android-" "" ANDROID_PLATFORM_LEVEL ${ANDROID_PLATFORM})
endif()

string(REPLACE "android-" "" ANDROID_PLATFORM_LEVEL ${ANDROID_PLATFORM})

# Aliases defined by meta/platforms.json include codename aliases for platform
# API levels as well as cover any gaps in platforms that may not have had NDK
# APIs.
if(NOT "${NDK_PLATFORM_ALIAS_${ANDROID_PLATFORM_LEVEL}}" STREQUAL "")
    message(STATUS "\
${ANDROID_PLATFORM} is an alias for \
${NDK_PLATFORM_ALIAS_${ANDROID_PLATFORM_LEVEL}}. Adjusting ANDROID_PLATFORM to \
match.")
    set(ANDROID_PLATFORM "${NDK_PLATFORM_ALIAS_${ANDROID_PLATFORM_LEVEL}}")
    string(REPLACE "android-" "" ANDROID_PLATFORM_LEVEL ${ANDROID_PLATFORM})
endif()

# Pull up to the minimum supported version if an old API level was requested.
if(ANDROID_PLATFORM_LEVEL LESS NDK_MIN_PLATFORM_LEVEL)
    message(STATUS "\
${ANDROID_PLATFORM} is unsupported. Using minimum supported version \
${NDK_MIN_PLATFORM_LEVEL}.")
    set(ANDROID_PLATFORM "android-${NDK_MIN_PLATFORM_LEVEL}")
    string(REPLACE "android-" "" ANDROID_PLATFORM_LEVEL ${ANDROID_PLATFORM})
endif()

# And for LP64 we need to pull up to 21. No diagnostic is provided here because
# minSdkVersion < 21 is valid for the project even though it may not be for this
# ABI.
if(ANDROID_ABI MATCHES "64(-v8a)?$" AND ANDROID_PLATFORM_LEVEL LESS 21)
    set(ANDROID_PLATFORM android-21)
    set(ANDROID_PLATFORM_LEVEL 21)
endif()

# ANDROID_PLATFORM beyond the maximum is an error. The correct way to specify
# the latest version is ANDROID_PLATFORM=latest.
if(ANDROID_PLATFORM_LEVEL GREATER NDK_MAX_PLATFORM_LEVEL)
    message(SEND_ERROR "\
${ANDROID_PLATFORM} is above the maximum supported version \
${NDK_MAX_PLATFORM_LEVEL}. Choose a supported API level or set \
ANDROID_PLATFORM to \"latest\".")
endif()

if(NOT ANDROID_STL)
    set(ANDROID_STL c++_static)
endif()

if("${ANDROID_STL}" STREQUAL "gnustl_shared" OR
    "${ANDROID_STL}" STREQUAL "gnustl_static" OR
    "${ANDROID_STL}" STREQUAL "stlport_shared" OR
    "${ANDROID_STL}" STREQUAL "stlport_static")
    message(FATAL_ERROR "\
${ANDROID_STL} is no longer supported. Please switch to either c++_shared or \
c++_static. See https://developer.android.com/ndk/guides/cpp-support.html \
for more information.")
endif()

set(ANDROID_PIE TRUE)
if(NOT ANDROID_ARM_MODE)
    set(ANDROID_ARM_MODE thumb)
endif()

# Standard cross-compiling stuff.
set(ANDROID TRUE)
set(CMAKE_SYSTEM_NAME Android)

# https://github.com/android-ndk/ndk/issues/890
#
# ONLY doesn't do anything when CMAKE_FIND_ROOT_PATH is empty. Without this,
# CMake will wrongly search host sysroots for headers/libraries. The actual path
# used here is fairly meaningless since CMake doesn't handle the NDK sysroot
# layout (per-arch and per-verion subdirectories for libraries), so find_library
# is handled separately by CMAKE_SYSTEM_LIBRARY_PATH.
list(APPEND CMAKE_FIND_ROOT_PATH "${ANDROID_NDK}")

# Allow users to override these values in case they want more strict behaviors.
# For example, they may want to prevent the NDK's libz from being picked up so
# they can use their own.
# https://github.com/android-ndk/ndk/issues/517
if(NOT CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
endif()

if(NOT CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
endif()

if(NOT CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()

if(NOT CMAKE_FIND_ROOT_PATH_MODE_PACKAGE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

# ABI.
set(CMAKE_ANDROID_ARCH_ABI ${ANDROID_ABI})
if(ANDROID_ABI STREQUAL armeabi-v7a)
    set(ANDROID_SYSROOT_ABI arm)
    set(ANDROID_TOOLCHAIN_NAME arm-linux-androideabi)
    set(CMAKE_SYSTEM_PROCESSOR armv7-a)
    set(ANDROID_LLVM_TRIPLE armv7-none-linux-androideabi)
elseif(ANDROID_ABI STREQUAL arm64-v8a)
    set(ANDROID_SYSROOT_ABI arm64)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(ANDROID_TOOLCHAIN_NAME aarch64-linux-android)
    set(ANDROID_LLVM_TRIPLE aarch64-none-linux-android)
elseif(ANDROID_ABI STREQUAL x86)
    set(ANDROID_SYSROOT_ABI x86)
    set(CMAKE_SYSTEM_PROCESSOR i686)
    set(ANDROID_TOOLCHAIN_NAME i686-linux-android)
    set(ANDROID_LLVM_TRIPLE i686-none-linux-android)
elseif(ANDROID_ABI STREQUAL x86_64)
    set(ANDROID_SYSROOT_ABI x86_64)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
    set(ANDROID_TOOLCHAIN_NAME x86_64-linux-android)
    set(ANDROID_LLVM_TRIPLE x86_64-none-linux-android)
else()
    message(FATAL_ERROR "Invalid Android ABI: ${ANDROID_ABI}.")
endif()

set(ANDROID_LLVM_TRIPLE "${ANDROID_LLVM_TRIPLE}${ANDROID_PLATFORM_LEVEL}")

# Don't re-export libgcc symbols in every binary.
byte_add_default_link_flag(-Wl,--exclude-libs,libgcc.a)
byte_add_default_link_flag(-Wl,--exclude-libs,libatomic.a)

# STL.
set(ANDROID_CXX_STANDARD_LIBRARIES)
if(ANDROID_STL STREQUAL system)
    set(_RTC_ANDROID_CXX_FLAGS_ "${_RTC_ANDROID_CXX_FLAGS_} -stdlib=libstdc++")
    if(NOT "x${ANDROID_CPP_FEATURES}" STREQUAL "x")
        list(APPEND ANDROID_CXX_STANDARD_LIBRARIES "-lc++abi")
        if(ANDROID_PLATFORM_LEVEL LESS 21)
            list(APPEND ANDROID_CXX_STANDARD_LIBRARIES "-landroid_support")
        endif()
    endif()
elseif(ANDROID_STL STREQUAL c++_static)
    byte_add_default_link_flag(-static-libstdc++)
elseif(ANDROID_STL STREQUAL c++_shared)
elseif(ANDROID_STL STREQUAL none)
    byte_add_default_compile_flag(-nostdinc++ CXX)
    byte_add_default_link_flag(-nostdlib++)
else()
    message(FATAL_ERROR "Invalid Android STL: ${ANDROID_STL}.")
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux)
    set(ANDROID_HOST_TAG linux-x86_64)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
    set(ANDROID_HOST_TAG darwin-x86_64)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
    set(ANDROID_HOST_TAG windows-x86_64)
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)
    set(ANDROID_TOOLCHAIN_SUFFIX .exe)
endif()

# Toolchain.
set(ANDROID_TOOLCHAIN_ROOT
    "${ANDROID_NDK}/toolchains/llvm/prebuilt/${ANDROID_HOST_TAG}")
set(ANDROID_TOOLCHAIN_PREFIX
    "${ANDROID_TOOLCHAIN_ROOT}/bin/${ANDROID_TOOLCHAIN_NAME}-")

# find_library searches a handful of paths as described by
# https://cmake.org/cmake/help/v3.6/command/find_library.html. CMake doesn't
# understand the Android sysroot layout, so we need to give the direct path to
# the libraries rather than just the sysroot. Set up CMAKE_SYSTEM_LIBRARY_PATH
# (https://cmake.org/cmake/help/v3.6/variable/CMAKE_SYSTEM_LIBRARY_PATH.html)
# instead.

# NB: This variable causes CMake to automatically pass --sysroot to the
# toolchain. Studio currently relies on this to recognize Android builds. If
# this variable is removed, ensure that flag is still passed.
# TODO: Teach Studio to recognize Android builds based on --target.
set(CMAKE_SYSROOT "${ANDROID_TOOLCHAIN_ROOT}/sysroot")

# Allows CMake to find headers in the architecture-specific include directories.
set(CMAKE_LIBRARY_ARCHITECTURE "${ANDROID_TOOLCHAIN_NAME}")

# Instructs CMake to search the correct API level for libraries.
list(APPEND CMAKE_SYSTEM_LIBRARY_PATH
    "/usr/lib/${ANDROID_TOOLCHAIN_NAME}/${ANDROID_PLATFORM_LEVEL}")

set(ANDROID_HOST_PREBUILTS "${ANDROID_NDK}/prebuilt/${ANDROID_HOST_TAG}")

set(ANDROID_C_COMPILER
    "${ANDROID_TOOLCHAIN_ROOT}/bin/clang${ANDROID_TOOLCHAIN_SUFFIX}")
set(ANDROID_CXX_COMPILER
    "${ANDROID_TOOLCHAIN_ROOT}/bin/clang++${ANDROID_TOOLCHAIN_SUFFIX}")
set(ANDROID_ASM_COMPILER
    "${ANDROID_TOOLCHAIN_ROOT}/bin/clang${ANDROID_TOOLCHAIN_SUFFIX}")
# Clang can fail to compile if CMake doesn't correctly supply the target and
# external toolchain, but to do so, CMake needs to already know that the
# compiler is clang. Tell CMake that the compiler is really clang, but don't
# use CMakeForceCompiler, since we still want compile checks. We only want
# to skip the compiler ID detection step.
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_ID Clang)
set(CMAKE_CXX_COMPILER_ID Clang)
set(CMAKE_C_COMPILER_VERSION 8.0)
set(CMAKE_CXX_COMPILER_VERSION 8.0)
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT 11)
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT 14)
set(CMAKE_C_COMPILER_TARGET   ${ANDROID_LLVM_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${ANDROID_LLVM_TRIPLE})
set(CMAKE_ASM_COMPILER_TARGET ${ANDROID_LLVM_TRIPLE})
set(CMAKE_C_COMPILER_EXTERNAL_TOOLCHAIN   "${ANDROID_TOOLCHAIN_ROOT}")
set(CMAKE_CXX_COMPILER_EXTERNAL_TOOLCHAIN "${ANDROID_TOOLCHAIN_ROOT}")
set(CMAKE_ASM_COMPILER_EXTERNAL_TOOLCHAIN "${ANDROID_TOOLCHAIN_ROOT}")
set(ANDROID_AR "${ANDROID_TOOLCHAIN_PREFIX}ar${ANDROID_TOOLCHAIN_SUFFIX}")
set(ANDROID_RANLIB
  "${ANDROID_TOOLCHAIN_PREFIX}ranlib${ANDROID_TOOLCHAIN_SUFFIX}")

# Generic flags.
byte_add_default_compile_flag(-DANDROID)
byte_add_default_compile_flag(-fdata-sections)
byte_add_default_compile_flag(-ffunction-sections)
byte_add_default_compile_flag(-funwind-tables)
byte_add_default_compile_flag(-fstack-protector)
byte_add_default_compile_flag(-no-canonical-prefixes)
byte_add_default_compile_flag(-fvisibility-inlines-hidden)
byte_add_default_compile_flag(-fvisibility=hidden)
byte_add_default_compile_flag(-fmerge-all-constants)

byte_add_default_link_flag(-Wl,--build-id)
byte_add_default_link_flag(-Wl,--warn-shared-textrel)
byte_add_default_link_flag(-Wl,--fatal-warnings)
byte_add_default_link_flag(-Wl,--gc-sections)
byte_add_default_link_flag(-Wl,--gc-sections All EXECUTABLE)

# Debug and release flags.
if(ANDROID_TOOLCHAIN STREQUAL clang)
    byte_add_default_compile_flag(-fno-limit-debug-info All Debug)
endif()

# -fomit-frame-pointer to reduce size
byte_add_default_compile_flag(-fomit-frame-pointer All Release)

# Toolchain and ABI specific flags.
if(ANDROID_ABI STREQUAL x86 AND ANDROID_PLATFORM_LEVEL LESS 24)
    # http://b.android.com/222239
    # http://b.android.com/220159 (internal http://b/31809417)
    # x86 devices have stack alignment issues.
    byte_add_default_compile_flag(-mstackrealign)
endif()

# TODO: Remove when https://github.com/android-ndk/ndk/issues/884 is fixed.
byte_add_default_compile_flag(-fno-addrsig)

# STL specific flags.
if(ANDROID_STL MATCHES "^c\\+\\+_")
    if(ANDROID_ABI MATCHES "^armeabi")
        byte_add_default_link_flag(-Wl,--exclude-libs,libunwind.a)
    endif()
endif()

set(CMAKE_C_STANDARD_LIBRARIES_INIT "-latomic -lm")
set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_C_STANDARD_LIBRARIES_INIT}")
if(ANDROID_CXX_STANDARD_LIBRARIES)
    string(REPLACE ";" "\" \"" ANDROID_CXX_STANDARD_LIBRARIES "\"${ANDROID_CXX_STANDARD_LIBRARIES}\"")
    set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_CXX_STANDARD_LIBRARIES_INIT} ${ANDROID_CXX_STANDARD_LIBRARIES}")
endif()

# Configuration specific flags.

# PIE is supported on all currently supported Android releases, but it is not
# supported with static executables, so we still provide ANDROID_PIE as an
# escape hatch for those.
if(ANDROID_PIE)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

if(NOT ANDROID_ALLOW_UNDEFINED_SYMBOLS)
    byte_add_default_link_flag(-Wl,--no-undefined)
endif()
if(ANDROID_ABI MATCHES "armeabi")
    # Clang does not set this up properly when using -fno-integrated-as.
    # https://github.com/android-ndk/ndk/issues/906
    byte_add_default_compile_flag(-march=armv7-a)
    if(ANDROID_ARM_MODE STREQUAL thumb)
        byte_add_default_compile_flag(-mthumb)
    elseif(ANDROID_ARM_MODE STREQUAL arm)
        # Default behavior.
    else()
        message(FATAL_ERROR "Invalid Android ARM mode: ${ANDROID_ARM_MODE}.")
    endif()
    if(ANDROID_ABI STREQUAL "armeabi-v7a" AND ANDROID_ARM_NEON)
        byte_add_default_compile_flag(-mfpu=neon)
    endif()
endif()

# CMake automatically forwards all compiler flags to the linker, and clang
# doesn't like having -Wa flags being used for linking. To prevent CMake from
# doing this would require meddling with the CMAKE_<LANG>_COMPILE_OBJECT rules,
# which would get quite messy.
byte_add_default_link_flag(-Qunused-arguments)

byte_add_default_compile_flag(-Wa,--noexecstack)
byte_add_default_link_flag(-Wl,-z,noexecstack)

if(ANDROID_DISABLE_FORMAT_STRING_CHECKS)
    byte_add_default_compile_flag(-Wno-error=format-security)
else()
    byte_add_default_compile_flag(-Wformat)
    byte_add_default_compile_flag(-Werror=format-security)
endif()

if(ANDROID_CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER   "${ANDROID_CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${ANDROID_CCACHE}")
endif()
set(CMAKE_C_COMPILER        "${ANDROID_C_COMPILER}")
set(CMAKE_CXX_COMPILER      "${ANDROID_CXX_COMPILER}")
set(CMAKE_AR                "${ANDROID_AR}" CACHE FILEPATH "Archiver")
set(CMAKE_RANLIB            "${ANDROID_RANLIB}" CACHE FILEPATH "Ranlib")
set(_CMAKE_TOOLCHAIN_PREFIX "${ANDROID_TOOLCHAIN_PREFIX}")

if(ANDROID_ABI STREQUAL "x86" OR ANDROID_ABI STREQUAL "x86_64")
    set(CMAKE_ASM_NASM_COMPILER
        "${ANDROID_TOOLCHAIN_ROOT}/bin/yasm${ANDROID_TOOLCHAIN_SUFFIX}")
    set(CMAKE_ASM_NASM_COMPILER_ARG1 "-DELF")
endif()

# Compatibility for read-only variables.
# Read-only variables for compatibility with the other toolchain file.
# We'll keep these around for the existing projects that still use them.
# TODO: All of the variables here have equivalents in our standard set of
# configurable variables, so we can remove these once most of our users migrate
# to those variables.
set(ANDROID_NATIVE_API_LEVEL ${ANDROID_PLATFORM_LEVEL})
if(ANDROID_ALLOW_UNDEFINED_SYMBOLS)
    set(ANDROID_SO_UNDEFINED TRUE)
else()
    set(ANDROID_NO_UNDEFINED TRUE)
endif()
set(ANDROID_FUNCTION_LEVEL_LINKING TRUE)
set(ANDROID_GOLD_LINKER TRUE)
set(ANDROID_NOEXECSTACK TRUE)
set(ANDROID_RELRO TRUE)
if(ANDROID_ARM_MODE STREQUAL arm)
    set(ANDROID_FORCE_ARM_BUILD TRUE)
endif()
if(ANDROID_CCACHE)
    set(NDK_CCACHE "${ANDROID_CCACHE}")
endif()
if(ANDROID_TOOLCHAIN STREQUAL clang)
    set(ANDROID_TOOLCHAIN_NAME ${ANDROID_TOOLCHAIN_NAME}-clang)
else()
    set(ANDROID_TOOLCHAIN_NAME ${ANDROID_TOOLCHAIN_NAME}-4.9)
endif()
set(ANDROID_NDK_HOST_X64 TRUE)
set(ANDROID_NDK_LAYOUT RELEASE)
if(ANDROID_ABI STREQUAL armeabi-v7a)
    set(ARMEABI_V7A TRUE)
    if(ANDROID_ARM_NEON)
        set(NEON TRUE)
    endif()
elseif(ANDROID_ABI STREQUAL arm64-v8a)
    set(ARM64_V8A TRUE)
elseif(ANDROID_ABI STREQUAL x86)
    set(X86 TRUE)
elseif(ANDROID_ABI STREQUAL x86_64)
    set(X86_64 TRUE)
endif()
set(ANDROID_NDK_HOST_SYSTEM_NAME ${ANDROID_HOST_TAG})
set(ANDROID_NDK_ABI_NAME ${ANDROID_ABI})
set(ANDROID_NDK_RELEASE r${ANDROID_NDK_REVISION})
set(ANDROID_ARCH_NAME ${ANDROID_SYSROOT_ABI})
set(TOOL_OS_SUFFIX ${ANDROID_TOOLCHAIN_SUFFIX})
if(ANDROID_TOOLCHAIN STREQUAL clang)
    set(ANDROID_COMPILER_IS_CLANG TRUE)
endif()

# CMake 3.7+ compatibility.
if (CMAKE_VERSION VERSION_GREATER 3.7.0)
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK})
    set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)

    set(CMAKE_ANDROID_STL_TYPE ${ANDROID_STL})

    if(ANDROID_ABI MATCHES "^armeabi(-v7a)?$")
        set(CMAKE_ANDROID_ARM_NEON ${ANDROID_ARM_NEON})
        set(CMAKE_ANDROID_ARM_MODE ${ANDROID_ARM_MODE})
    endif()
endif()

byte_add_default_compile_flag(-pthread)
byte_add_default_compile_flag(-fPIC)
byte_add_default_compile_flag(-O0 All Debug)
byte_add_default_compile_flag(-g All Debug)
byte_add_default_compile_flag(-DDEBUG=1 All Debug)
byte_add_default_compile_flag(-D_DEBUG=1 All Debug)
byte_add_default_compile_flag(${RTC_OPTIMIZE_FLAG.cache} All Release)
byte_add_default_compile_flag(-DNDEBUG=1 All Release)
byte_add_default_compile_flag(-D_NDEBUG=1 All Release)
byte_add_default_compile_flag(-std=c++17 CXX)
byte_add_default_compile_flag(-Wno-multichar)

## Print status messages to inform of the current state
# exceptions on/off?
if (RTC_CPP_EXCEPTION)
    message(STATUS "Enabling CPP Exception flag.")
else()
    message(STATUS "Disabling CPP Exception flag.")
endif()

# rtti on/off?
if (RTC_CPP_RTTI)
    message(STATUS "Enabling CPP RTTI flag.")
else()
    message(STATUS "Disabling CPP RTTI flag.")
endif()

byte_save_dafault_flags()

