cmake_minimum_required(VERSION 3.10.0)

###### options
# Set add __CACHE_XXX__ to a variable, and unset the original value.
# So next time we run cmake, it will restart a new one.
macro(set_cache var)
    message(STATUS "Cache Var ${var}: ${${var}}")
    unset(__C_${var} CACHE)
    set(__C_${var} ${${var}} CACHE STRING "")
    unset(${var}.cache CACHE)
    set(${var}.cache ${${var}} CACHE STRING "")
    unset(${var} CACHE)
endmacro(set_cache)

option(RTC_LIBRARY_STATIC "generate static library or not, default is shared" OFF)
option(RTC_CPP_EXCEPTION "enable c++ exception" ON)
option(RTC_CPP_RTTI "enable c++ rtti" OFF)
option(RTC_DYNAMIC_BASE "build base as dynamic lib" OFF)
option(RTC_MULTIPLE_LIB "build each submodule into individual libs" OFF)
option(RTC_ENABLE_CODESIGN "enable code sign" ON)
option(RTC_TARGET_VEWTN "build white board library" OFF)

# The following is cache
set_cache(RTC_LIBRARY_STATIC)
set_cache(RTC_CPP_EXCEPTION)
set_cache(RTC_CPP_RTTI)
set_cache(RTC_DYNAMIC_BASE)
set_cache(RTC_MULTIPLE_LIB)
set_cache(RTC_ENABLE_CODESIGN)
set_cache(RTC_TARGET_VEWTN)


###### platform
# Check current platform
set(RTC_HOST_WINDOWS FALSE)
set(RTC_HOST_MACOS FALSE)
set(RTC_HOST_LINUX FALSE)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(RTC_HOST_WINDOWS TRUE)
    message(STATUS "Compile host: Windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(RTC_HOST_MACOS TRUE)
    message(STATUS "Compile host: MacOS")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(RTC_HOST_LINUX TRUE)
    message(STATUS "Compile host: Linux")
else()
    message(WARNING "Cannot recognized compile host, set default to Linux")
    set(RTC_HOST_LINUX TRUE)
endif()

set_cache(RTC_HOST_WINDOWS)
set_cache(RTC_HOST_MACOS)
set_cache(RTC_HOST_LINUX)

if (ANDROID)
    set(RTC_PLATFORM "android")
else()
    if (RTC_HOST_MACOS.cache AND (NOT RTC_PLATFORM))
        message(STATUS "not specified platform, guess to create macos")
        set (RTC_PLATFORM "macos")
    endif()

    # Current host is mac osx, and not compile android, not select 
    # ios or mac, which should be an fatal error
    if (RTC_HOST_MACOS.cache AND (NOT "${RTC_PLATFORM}" STREQUAL "ios") AND
            (NOT "${RTC_PLATFORM}" STREQUAL "macos") AND 
            (NOT "${RTC_PLATFORM}" STREQUAL "mac") AND
            (NOT "${RTC_PLATFORM}" STREQUAL "android"))
        message(FATAL_ERROR "Please set RTC_PLATFORM as ios or macos")
    endif()
endif()

# for other platform and set the default platform according to its host name
if (NOT RTC_PLATFORM)
    if (RTC_HOST_LINUX.cache)
        set(RTC_PLATFORM "linux")
    elseif(RTC_HOST_WINDOWS.cache)
        set(RTC_PLATFORM "windows")
    endif()
    # Don't need to check macos 
endif()

set(RTC_PLATFORM_IOS FALSE)
set(RTC_PLATFORM_ANDROID FALSE)
set(RTC_PLATFORM_MAC FALSE)
set(RTC_PLATFORM_LINUX FALSE)
set(RTC_PLATFORM_WINDOWS FALSE)
set(RTC_PLATFORM_APPLE FALSE)

if (RTC_PLATFORM STREQUAL "ios")
    set(RTC_PLATFORM_IOS TRUE)
    set(RTC_PLATFORM_APPLE TRUE)
    add_definitions( -DRTC_PLATFORM_IOS=1 )
elseif((RTC_PLATFORM STREQUAL "macos") OR (RTC_PLATFORM STREQUAL "mac"))
    set(RTC_PLATFORM_MAC TRUE)
    set(RTC_PLATFORM_APPLE TRUE)
    add_definitions( -DRTC_PLATFORM_MAC=1 )
elseif(RTC_PLATFORM STREQUAL "android")
    set(RTC_PLATFORM_ANDROID TRUE)
    add_definitions( -DRTC_PLATFORM_ANDROID=1 )
elseif((RTC_PLATFORM STREQUAL "windows") OR (RTC_PLATFORM STREQUAL "win"))
    set(RTC_PLATFORM_WINDOWS TRUE)
    add_definitions( -DRTC_PLATFORM_WINDOWS=1 )
else()
    set(RTC_PLATFORM_LINUX TRUE)
    add_definitions( -DRTC_PLATFORM_LINUX=1 )
endif()

message(STATUS "RTC_PLATFORM: ${RTC_PLATFORM}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

set_cache(RTC_PLATFORM)
set_cache(RTC_PLATFORM_IOS)
set_cache(RTC_PLATFORM_MAC)
set_cache(RTC_PLATFORM_APPLE)
set_cache(RTC_PLATFORM_ANDROID)
set_cache(RTC_PLATFORM_WINDOWS)
set_cache(RTC_PLATFORM_LINUX)

###### toolchain
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel)

if (NOT RTC_OPTIMIZE_FLAG)
    message(STATUS "Using the default Release Optimize Level since RTC_OPTIMIZE_FLAG not provided!")
    if (RTC_PLATFORM_IOS.cache)
        set(RTC_OPTIMIZE_FLAG "-Oz" CACHE STRING "")
    elseif(RTC_PLATFORM_WINDOWS.cache)
        set(RTC_OPTIMIZE_FLAG "/Ox" CACHE STRING "")
    else()
        set(RTC_OPTIMIZE_FLAG "-O3" CACHE STRING "")
    endif()
endif()

string(LENGTH "${RTC_OPTIMIZE_FLAG}" _OPT_LEVEL_LEN_)
math(EXPR _OPT_LEVEL_LEN_ "${_OPT_LEVEL_LEN_} - 1")
string(SUBSTRING "${RTC_OPTIMIZE_FLAG}" ${_OPT_LEVEL_LEN_} 1 RTC_OPTIMIZE_LEVEL)

message(STATUS "Release Optimize flag is ${RTC_OPTIMIZE_FLAG}")
message(STATUS "Release Optimize Level is ${RTC_OPTIMIZE_LEVEL}")

set_cache(RTC_OPTIMIZE_FLAG)
set_cache(RTC_OPTIMIZE_LEVEL)

# Unset the FORCE on cache variables if in try_compile()
set(FORCE_CACHE FORCE)
get_property(_CMAKE_IN_TRY_COMPILE GLOBAL PROPERTY IN_TRY_COMPILE)
if(_CMAKE_IN_TRY_COMPILE)
    unset(FORCE_CACHE)
endif()

set(TOOLCHAINS_DIR ${CMAKE_CURRENT_LIST_DIR})

# include the common macros
include(${TOOLCHAINS_DIR}/macro.common.cmake)

if(RTC_PLATFORM_IOS.cache OR RTC_PLATFORM_MAC.cache)
    include(${TOOLCHAINS_DIR}/macro.apple.cmake)
endif()

if(RTC_PLATFORM_ANDROID.cache)
    include(${TOOLCHAINS_DIR}/macro.android.cmake)
endif()

# Include differet platform's toolchain file
if (NOT CMAKE_TOOLCHAIN_FILE)
    if(RTC_PLATFORM_IOS.cache)
		if (EXISTS ${TOOLCHAINS_DIR}/ios.cmake)
			include(${TOOLCHAINS_DIR}/ios.cmake)
			set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAINS_DIR}/ios.cmake)
        endif()
    elseif(RTC_PLATFORM_ANDROID.cache)
        if (EXISTS ${TOOLCHAINS_DIR}/android.cmake)
            include(${TOOLCHAINS_DIR}/android.cmake)
            set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAINS_DIR}/android.cmake)
        endif()
    elseif(RTC_PLATFORM_LINUX.cache)
        if (EXISTS ${TOOLCHAINS_DIR}/linux.cmake)
            include(${TOOLCHAINS_DIR}/linux.cmake)
            set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAINS_DIR}/linux.cmake)
        endif()
    endif()
else()
    if (RTC_PLATFORM_ANDROID.cache)
        #force to include our toolchain file
        if (EXISTS ${TOOLCHAINS_DIR}/android.cmake)
            include(${TOOLCHAINS_DIR}/android.cmake)
        endif()
    elseif(RTC_PLATFORM_LINUX.cache)
        if (EXISTS ${TOOLCHAINS_DIR}/linux.cmake)
            include(${TOOLCHAINS_DIR}/linux.cmake)
            set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAINS_DIR}/linux.cmake)
        endif()
    endif()
endif()

include_directories(BEFORE SYSTEM ${CMAKE_SOURCE_DIR})

# Remove all default settings
unset(CMAKE_C_FLAGS CACHE)
unset(CMAKE_C_FLAGS_DEBUG CACHE)
unset(CMAKE_C_FLAGS_RELEASE CACHE)
unset(CMAKE_CXX_FLAGS CACHE)
unset(CMAKE_CXX_FLAGS_DEBUG CACHE)
unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
unset(CMAKE_LINK_FLAGS CACHE)
unset(CMAKE_LINK_FLAGS_DEBUG CACHE)
unset(CMAKE_LINK_FLAGS_RELEASE CACHE)
