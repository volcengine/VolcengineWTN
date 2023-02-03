# CopyRight (c) 2020 ByteDance Inc. All Right Reserved.
# Cmake Tookchain for Linux

option(BYTE_LINUX_MACH32 "Build 32bit target" OFF)
set_cache(BYTE_LINUX_MACH32)

byte_init_default_flags()

byte_add_default_compile_flag(-fdiagnostics-color=always)

# exceptions on/off?
if (NOT BYTE_CPP_EXCEPTION.cache)
    # byte_add_default_compile_flag(-fno-exceptions CXX)
endif()

# rtti on/off?
if (NOT BYTE_CPP_RTTI.cache)
    byte_add_default_compile_flag(-fno-rtti CXX)
endif()

if (BYTE_LINUX_MACH32.cache)
    byte_add_default_compile_flag(-m32)
    byte_add_default_link_flags(-m32)
endif()

byte_add_default_compile_flag(-fPIC)
byte_add_default_compile_flag(-Wno-extra)
byte_add_default_compile_flag(-Wno-multichar)
# byte_add_default_compile_flag(-pthread)
#byte_add_default_compile_flag("-Wl,--no-as-needed -ldl")

byte_add_default_compile_flag(-DDEBUG=1 All Debug)
byte_add_default_compile_flag(-D_DEBUG=1 All Debug)
byte_add_default_compile_flag(-DNDEBUG=1 All Release)
byte_add_default_compile_flag(-DN_DEBUG=1 All Release)

byte_add_default_compile_flag(-O0 All Debug)
byte_add_default_compile_flag(-g All Debug)
byte_add_default_compile_flag(${BYTE_OPTIMIZE_FLAG.cache} All Release)

byte_add_default_compile_flag(-g All RelWithDebInfo)
byte_add_default_compile_flag(-O3 All RelWithDebInfo)

byte_add_default_compile_flag(-std=c++17 CXX)
byte_add_default_compile_flag(-fvisibility=hidden CXX)
byte_add_default_compile_flag(-fvisibility-inlines-hidden CXX)
byte_add_default_compile_flag(-Wno-multichar)

## Print status messages to inform of the current state
# exceptions on/off?
if (BYTE_CPP_EXCEPTION.cache)
    message(STATUS "Enabling CPP Exception flag.")
else()
    message(STATUS "Disabling CPP Exception flag.")
endif()

# rtti on/off?
if (BYTE_CPP_RTTI.cache)
    message(STATUS "Enabling CPP RTTI flag.")
else()
    message(STATUS "Disabling CPP RTTI flag.")
endif()

byte_save_dafault_flags()