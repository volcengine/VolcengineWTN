# CopyRight (c) 2020 ByteDance Inc. All Right Reserved.
# Cmake Tookchain for iOS

byte_init_default_flags()

# Make all compile flags have $(inherited) in the beginning
byte_add_default_compile_flag($(inherited) All All)
byte_add_default_compile_flag(-fcolor-diagnostics)
byte_add_default_link_flag($(inherited))

# Fix for PThread library not in path
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(CMAKE_SYSTEM_NAME iOS)

# need to set some flags directly as Xcode attributes
set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
# Cache what generator is used
set(USED_CMAKE_GENERATOR "${CMAKE_GENERATOR}" CACHE STRING "Expose CMAKE_GENERATOR" FORCE)

# Set both support iphone and ipad
set(CMAKE_XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")

# Get the Xcode version being used.
execute_process(COMMAND xcodebuild -version
    OUTPUT_VARIABLE XCODE_VERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX MATCH "Xcode [0-9\\.]+" XCODE_VERSION "${XCODE_VERSION}")
string(REGEX REPLACE "Xcode ([0-9\\.]+)" "\\1" XCODE_VERSION "${XCODE_VERSION}")
set_cache(XCODE_VERSION)

# Set default ios platform, OS for iphone os
# also can be SIMULATOR 
if(NOT DEFINED RTC_IOS_PLATFORM)
    set(RTC_IOS_PLATFORM "OS")
endif()
set_cache(RTC_IOS_PLATFORM)
if ((NOT RTC_IOS_PLATFORM.cache STREQUAL "OS") AND (NOT RTC_IOS_PLATFORM.cache STREQUAL "SIMULATOR" ))
    message(FATAL_ERROR "RTC_IOS_PLATFORM can only be OS or SIMULATOR")
endif()

# Determine the platform name and architectures for use in xcodebuild commands
# from the specified RTC_IOS_PLATFORM name.
if (RTC_IOS_PLATFORM.cache STREQUAL "OS")
    set(SDK_NAME iphoneos)
    if(NOT RTC_IOS_ARCHS)
        set(RTC_IOS_ARCHS arm64)
        set(APPLE_TARGET_TRIPLE_INT arm-apple-ios)
    endif()
elseif(RTC_IOS_PLATFORM.cache STREQUAL "SIMULATOR")
    set(SDK_NAME iphonesimulator)
    if(NOT RTC_IOS_ARCHS)
        set(RTC_IOS_ARCHS x86_64)
        set(APPLE_TARGET_TRIPLE_INT x86_64-apple-ios)
    endif()
endif()
set(CMAKE_XCODE_ATTRIBUTE_ARCHS "${RTC_IOS_ARCHS}")

# Save IOS Archs
set_cache(RTC_IOS_ARCHS)

# Save SDK Name 
set_cache(SDK_NAME)

# If user did not specify the SDK root to use, then query xcodebuild for it.
execute_process(COMMAND xcodebuild -version -sdk ${SDK_NAME.cache} Path
    OUTPUT_VARIABLE RTC_OSX_SYSROOT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if (NOT DEFINED RTC_OSX_SYSROOT AND NOT DEFINED CMAKE_OSX_SYSROOT)
    message(SEND_ERROR "Please make sure that Xcode is installed and that the toolchain"
        "is pointing to the correct path. Please run:"
        "sudo xcode-select -s /Applications/Xcode.app/Contents/Developer"
        "and see if that fixes the problem for you.")
    message(FATAL_ERROR "Invalid CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT} "
        "does not exist.")
elseif(DEFINED RTC_OSX_SYSROOT)
    set(CMAKE_OSX_SYSROOT "${RTC_OSX_SYSROOT}" CACHE INTERNAL "")
endif()
set_cache(RTC_OSX_SYSROOT)

# Set Xcode property for SDKROOT as well if Xcode generator is used
if(USED_CMAKE_GENERATOR MATCHES "Xcode")
    set(CMAKE_OSX_SDKROOT "${SDK_NAME.cache}" CACHE INTERNAL "")
endif()

# Deployment target setting
if (NOT DEFINED RTC_IOS_DEPLOYMENT_TARGET)
    set(RTC_IOS_DEPLOYMENT_TARGET "9.0")
    message(STATUS "Using the default min-version since RTC_IOS_DEPLOYMENT_TARGET not provided!")
endif()
set_cache(RTC_IOS_DEPLOYMENT_TARGET)

# Enable BitCode or not 
if (NOT DEFINED RTC_ENABLE_BITCODE AND NOT RTC_IOS_ARCHS.cache MATCHES "((^|;|, )(i386|x86_64))+")
    # Unless specified, enable bitcode support by default
    message(STATUS "Disabling bitcode support by default. RTC_ENABLE_BITCODE not provided!")
    set(RTC_ENABLE_BITCODE FALSE)
elseif(NOT DEFINED RTC_ENABLE_BITCODE)
    message(STATUS "Disabling bitcode support by default on simulators. "
        "RTC_ENABLE_BITCODE not provided for override!")
    set(RTC_ENABLE_BITCODE FALSE)
endif()
set_cache(RTC_ENABLE_BITCODE)

# Enable ARC or not 
# Use ARC or not
if(NOT DEFINED RTC_ENABLE_ARC)
    # Unless specified, enable ARC support by default
    set(RTC_ENABLE_ARC TRUE)
    message(STATUS "Enabling ARC support by default. RTC_ENABLE_ARC not provided!")
endif()
set_cache(RTC_ENABLE_ARC)

# Use hidden visibility or not
if(NOT DEFINED RTC_ENABLE_VISIBILITY)
    # Unless specified, disable symbols visibility by default
    set(RTC_ENABLE_VISIBILITY FALSE)
    message(STATUS "Hiding symbols visibility by default. RTC_ENABLE_VISIBILITY not provided!")
endif()
set_cache(RTC_ENABLE_VISIBILITY)

# Use lto or not
if(NOT DEFINED RTC_ENABLE_LTO)
    # Unless specified, disable lto by default
    set(RTC_ENABLE_LTO TRUE)
    message(STATUS "Turn on lto by default. RTC_ENABLE_LTO not provided!")
endif()
set_cache(RTC_ENABLE_LTO)

# Code Sign
if(NOT DEFINED RTC_ENABLE_CODESIGN)
    set(RTC_ENABLE_CODESIGN FALSE)
    message(STATUS "Disable CodeSign by default. RTC_ENABLE_CODESIGN not provided!")
endif()
set_cache(RTC_ENABLE_CODESIGN)

# Get the SDK version information.
execute_process(COMMAND xcodebuild -sdk ${CMAKE_OSX_SYSROOT} -version SDKVersion
    OUTPUT_VARIABLE SDK_VERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
set_cache(SDK_VERSION)

# Find the Developer root for the specific iOS platform being compiled for
# from CMAKE_OSX_SYSROOT.  Should be ../../ from SDK specified in
# CMAKE_OSX_SYSROOT. There does not appear to be a direct way to obtain
# this information from xcrun or xcodebuild.
if (NOT DEFINED CMAKE_DEVELOPER_ROOT AND NOT USED_CMAKE_GENERATOR MATCHES "Xcode")
    get_filename_component(PLATFORM_SDK_DIR ${CMAKE_OSX_SYSROOT} PATH)
    get_filename_component(CMAKE_DEVELOPER_ROOT ${PLATFORM_SDK_DIR} PATH)
    if (NOT DEFINED CMAKE_DEVELOPER_ROOT)
        message(FATAL_ERROR "Invalid CMAKE_DEVELOPER_ROOT: "
            "${CMAKE_DEVELOPER_ROOT} does not exist.")
    endif()
endif()

# Find the C & C++ compilers for the specified SDK.
if(NOT CMAKE_C_COMPILER)
    execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find clang
        OUTPUT_VARIABLE CMAKE_C_COMPILER
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
endif()
if(NOT CMAKE_CXX_COMPILER)
    execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find clang++
        OUTPUT_VARIABLE CMAKE_CXX_COMPILER
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using CXX compiler: ${CMAKE_CXX_COMPILER}")
endif()
# Find (Apple's) libtool.
execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find libtool
    OUTPUT_VARIABLE BUILD_LIBTOOL
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Using libtool: ${BUILD_LIBTOOL}")

# Configure libtool to be used instead of ar + ranlib to build static libraries.
# This is required on Xcode 7+, but should also work on previous versions of
# Xcode.
set(CMAKE_C_CREATE_STATIC_LIBRARY "${BUILD_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY "${BUILD_LIBTOOL} -static -o <TARGET> <LINK_FLAGS> <OBJECTS> ")
# Find the toolchain's provided install_name_tool if none is found on the host
if(NOT CMAKE_INSTALL_NAME_TOOL)
    execute_process(COMMAND xcrun -sdk ${CMAKE_OSX_SYSROOT} -find install_name_tool
        OUTPUT_VARIABLE CMAKE_INSTALL_NAME_TOOL_INT
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_INSTALL_NAME_TOOL ${CMAKE_INSTALL_NAME_TOOL_INT} CACHE STRING "" ${FORCE_CACHE})
endif()
# Get the version of Darwin (OS X) of the host.
execute_process(COMMAND uname -r
    OUTPUT_VARIABLE CMAKE_HOST_SYSTEM_VERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
if(SDK_NAME MATCHES "iphone")
    set(CMAKE_SYSTEM_NAME iOS CACHE INTERNAL "" ${FORCE_CACHE})
endif()

# Standard settings.
set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_SYSTEM_VERSION ${SDK_VERSION.cache})
set(UNIX TRUE)
set(APPLE TRUE)
set(IOS TRUE)
set(CMAKE_AR ar)
set(CMAKE_RANLIB ranlib)
set(CMAKE_STRIP strip)
# Set the architectures for which to build.
set(CMAKE_OSX_ARCHITECTURES ${RTC_IOS_ARCHS.cache})
# Change the type of target generated for try_compile() so it'll work when cross-compiling, weak compiler checks
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# All iOS/Darwin specific settings - some may be redundant.
set(CMAKE_MACOSX_BUNDLE YES)

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
set(CMAKE_SHARED_MODULE_PREFIX "lib")
set(CMAKE_SHARED_MODULE_SUFFIX ".so")
set(CMAKE_C_COMPILER_ABI ELF)
set(CMAKE_CXX_COMPILER_ABI ELF)
set(CMAKE_C_HAS_ISYSROOT 1)
set(CMAKE_CXX_HAS_ISYSROOT 1)
set(CMAKE_MODULE_EXISTS 1)
set(CMAKE_DL_LIBS "")
set(CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG "-compatibility_version ")
set(CMAKE_C_OSX_CURRENT_VERSION_FLAG "-current_version ")
set(CMAKE_CXX_OSX_COMPATIBILITY_VERSION_FLAG "${CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG}")
set(CMAKE_CXX_OSX_CURRENT_VERSION_FLAG "${CMAKE_C_OSX_CURRENT_VERSION_FLAG}")

if(RTC_IOS_ARCHS.cache MATCHES "((^|;|, )(arm64|arm64e|x86_64))+")
    set(CMAKE_C_SIZEOF_DATA_PTR 8)
    set(CMAKE_CXX_SIZEOF_DATA_PTR 8)
    if(RTC_IOS_ARCHS.cache MATCHES "((^|;|, )(arm64|arm64e))+")
        set(CMAKE_SYSTEM_PROCESSOR "arm64")
    else()
        set(CMAKE_SYSTEM_PROCESSOR "x86_64")
    endif()
else()
    set(CMAKE_C_SIZEOF_DATA_PTR 4)
    set(CMAKE_CXX_SIZEOF_DATA_PTR 4)
    set(CMAKE_SYSTEM_PROCESSOR "arm")
endif()
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")

# Note that only Xcode 7+ supports the newer more specific:
# -m${SDK_NAME}-version-min flags, older versions of Xcode use:
# -m(ios/ios-simulator)-version-min instead.
if(${CMAKE_VERSION} VERSION_LESS "3.11")
    if(RTC_IOS_PLATFORM.cache STREQUAL "OS")
        if(XCODE_VERSION.cache VERSION_LESS 7.0)
            byte_add_default_compile_flag(-mios-version-min=${RTC_IOS_DEPLOYMENT_TARGET.cache})
        else()
            # Xcode 7.0+ uses flags we can build directly from SDK_NAME.
            byte_add_default_compile_flag(-m${SDK_NAME.cache}-version-min=${RTC_IOS_DEPLOYMENT_TARGET.cache})
        endif()
    else()
        byte_add_default_compile_flag(-mios-simulator-version-min=${RTC_IOS_DEPLOYMENT_TARGET.cache})
    endif()
else()
    # Newer versions of CMake sets the version min flags correctly
    set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${RTC_IOS_DEPLOYMENT_TARGET.cache})
endif()

if(DEFINED APPLE_TARGET_TRIPLE_INT)
    set(APPLE_TARGET_TRIPLE ${APPLE_TARGET_TRIPLE_INT} CACHE STRING
        "Autoconf target triple compatible variable" ${FORCE_CACHE})
endif()

# Apply Settings
# Bitcode 
if ( RTC_ENABLE_BITCODE.cache )
    byte_add_default_compile_flag(-fembed-bitcode)
endif()

# ARC
if ( RTC_ENABLE_ARC.cache )
    byte_add_default_compile_flag(-fobjc-arc)
else()
    byte_add_default_compile_flag(-fno-objc-arc)
endif()

# Visibility
if ( RTC_ENABLE_VISIBILITY.cache )
    byte_add_default_compile_flag(-fvisibility=hidden)
    byte_add_default_compile_flag(-fvisibility-inlines-hidden)
endif()

# LTO
# if ( RTC_ENABLE_LTO.cache )
#     byte_add_default_compile_flag(-flto All Release)
# endif()

# exceptions on/off
if (NOT RTC_CPP_EXCEPTION.cache)
    byte_add_default_compile_flag(-fno-exceptions CXX)
endif()

# rtti
if (NOT RTC_CPP_RTTI.cache)
    byte_add_default_compile_flag(-fno-rtti CXX)
endif()

byte_add_default_compile_flag(-fmerge-all-constants)
byte_add_default_compile_flag(-fstrict-aliasing C)
byte_add_default_compile_flag(-O0 All Debug)
byte_add_default_compile_flag(-g All Debug)
byte_add_default_compile_flag(-DDEBUG=1 All Debug)
byte_add_default_compile_flag(-D_DEBUG=1 All Debug)
byte_add_default_compile_flag(-DNDEBUG=1 All Release)
byte_add_default_compile_flag(-D_NDEBUG=1 All Release)
byte_add_default_compile_flag(-std=gnu++17 CXX)
byte_add_default_compile_flag(-Wno-multichar)
byte_add_default_compile_flag(-g All RelWithDebInfo)
byte_add_default_compile_flag(-O3 All RelWithDebInfo)

byte_add_default_link_flag(-Wl,-search_paths_first)
set(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp" CACHE STRING "" FORCE)

# Set the find root to the iOS developer roots and to user defined paths.
set(CMAKE_FIND_ROOT_PATH ${RTC_OSX_SYSROOT.cache} ${CMAKE_PREFIX_PATH} CACHE STRING "Root path that will be prepended
 to all search paths")
# Default to searching for frameworks first.
set(CMAKE_FIND_FRAMEWORK FIRST)
# Set up the default search directories for frameworks.
set(CMAKE_FRAMEWORK_PATH
    ${CMAKE_DEVELOPER_ROOT}/Library/PrivateFrameworks
    ${RTC_OSX_SYSROOT.cache}/System/Library/Frameworks
    ${CMAKE_FRAMEWORK_PATH} CACHE STRING "Frameworks search paths" ${FORCE_CACHE})

# By default, search both the specified iOS SDK and the remainder of the host filesystem.
if(NOT CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH CACHE STRING "" ${FORCE_CACHE})
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH CACHE STRING "" ${FORCE_CACHE})
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH CACHE STRING "" ${FORCE_CACHE})
endif()
if(NOT CMAKE_FIND_ROOT_PATH_MODE_PACKAGE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH CACHE STRING "" ${FORCE_CACHE})
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET "" CACHE STRING "Force unset of the deployment target for MACOS" FORCE)

byte_save_dafault_flags()
