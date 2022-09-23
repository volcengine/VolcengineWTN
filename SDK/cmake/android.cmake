cmake_minimum_required(VERSION 3.10.0)

# add compiler macros
target_compile_definitions(${TARGET_NAME} PRIVATE WEBRTC_POSIX WEBRTC_ANDROID NOMINMAX)
if(ANDROID_ARM_NEON)
    target_compile_definitions(${TARGET_NAME} PRIVATE WEBRTC_HAS_NEON)
endif(ANDROID_ARM_NEON)

# include
target_include_directories(${TARGET_NAME} PRIVATE sdk/android/vewtn/gen/cpp)

# link libs, only for android
target_link_libraries(${TARGET_NAME} PRIVATE OpenSLES)
target_link_libraries(${TARGET_NAME} PRIVATE log)
