# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.0)

set(COMMON_SOURCES
        ${VEWTN_ROOT_DIR}/src/wtn_engine.h
        ${VEWTN_ROOT_DIR}/src/wtn_engine.cc
        ${VEWTN_ROOT_DIR}/src/wtn_interface.h
        ${VEWTN_ROOT_DIR}/src/wtn_publisher_impl.h
        ${VEWTN_ROOT_DIR}/src/wtn_publisher_impl.cc
        ${VEWTN_ROOT_DIR}/src/wtn_stats_callback.h
        ${VEWTN_ROOT_DIR}/src/wtn_stats_callback.cc 
        ${VEWTN_ROOT_DIR}/src/wtn_subscriber_impl.h
        ${VEWTN_ROOT_DIR}/src/wtn_subscriber_impl.cc
        ${VEWTN_ROOT_DIR}/src/wtn_webrtc_wrapper.h
        ${VEWTN_ROOT_DIR}/src/wtn_webrtc_wrapper.cc
        ${VEWTN_ROOT_DIR}/src/wtn_event_handler.h
        ${VEWTN_ROOT_DIR}/src/wtn_event_handler.cc

        ${VEWTN_ROOT_DIR}/src/utils/http_client.h
        ${VEWTN_ROOT_DIR}/src/utils/http_client.cc
        ${VEWTN_ROOT_DIR}/src/utils/whip_client.h
        ${VEWTN_ROOT_DIR}/src/utils/whip_client.cc
        
        ${VEWTN_ROOT_DIR}/src/external_module/external_audio_device_module.h
        ${VEWTN_ROOT_DIR}/src/external_module/external_audio_device_module.cc
        ${VEWTN_ROOT_DIR}/src/external_module/external_video_source.h
        ${VEWTN_ROOT_DIR}/src/external_module/external_video_source.cc
        ${VEWTN_ROOT_DIR}/src/external_module/fake_decoder.h
        ${VEWTN_ROOT_DIR}/src/external_module/fake_decoder.cc
        ${VEWTN_ROOT_DIR}/src/external_module/fake_encoder.h
        ${VEWTN_ROOT_DIR}/src/external_module/fake_encoder.cc
        ${VEWTN_ROOT_DIR}/src/external_module/video_sink_proxy.h
        ${VEWTN_ROOT_DIR}/src/external_module/video_sink_proxy.cc
        )
 
if (RTC_PLATFORM_IOS.cache)
    set(SDK_SOURCES_PREFIX ${VEWTN_ROOT_DIR}/sdk/ios)
    set(SDK_SOURCES
        ${SDK_SOURCES_PREFIX}/VEWTNDefines.h
        ${SDK_SOURCES_PREFIX}/VEWTNEngine.h
        ${SDK_SOURCES_PREFIX}/VEWTNEngine.mm
        ${SDK_SOURCES_PREFIX}/VEWTNEventHandler.h
        ${SDK_SOURCES_PREFIX}/VEWTNEventHandler.mm
        ${SDK_SOURCES_PREFIX}/VEWTNPublisher.h
        ${SDK_SOURCES_PREFIX}/VEWTNPublisher.mm
        ${SDK_SOURCES_PREFIX}/VEWTNSubscriber.h
        ${SDK_SOURCES_PREFIX}/VEWTNSubscriber.mm
        ${SDK_SOURCES_PREFIX}/VEWTNPubSubInternal.h

        ${VEWTN_ROOT_DIR}/src/platform/objc_platform_helper.h
        ${VEWTN_ROOT_DIR}/src/platform/objc_platform_helper.mm
        )
elseif (RTC_PLATFORM_ANDROID.cache)
    set(SDK_SOURCES_PREFIX ${VEWTN_ROOT_DIR}/sdk/android/vewtn/src/main/jni)
    set(SDK_SOURCES
        ${SDK_SOURCES_PREFIX}/jni_onload.cc
        ${SDK_SOURCES_PREFIX}/vewtn_jni_common.h
        ${SDK_SOURCES_PREFIX}/native_functions_jni.cc
        ${SDK_SOURCES_PREFIX}/rtc_event_handler_jni.h
        ${SDK_SOURCES_PREFIX}/rtc_event_handler_jni.cc
        ${VEWTN_ROOT_DIR}/src/platform/android_platform_helper.h
        ${VEWTN_ROOT_DIR}/src/platform/android_platform_helper.cc
        )
endif()

set(SOURCES ${COMMON_SOURCES} ${SDK_SOURCES} ${PLATFORM_SOURCES})

# group the source files
foreach (file ${SOURCES})
    file(RELATIVE_PATH relative_file "${VEWTN_ROOT_DIR}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if (NOT "${dir}" STREQUAL "")
        string(REPLACE "/" "\\" dir ${dir})
        source_group("${dir}" FILES ${file})
    endif ()
endforeach ()
