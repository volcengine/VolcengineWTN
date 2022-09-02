# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.0)

set(COMMON_SOURCES
        ${VEWTN_ROOT_DIR}/src/http_client.h
        ${VEWTN_ROOT_DIR}/src/http_client.cc
        ${VEWTN_ROOT_DIR}/src/vewtn_interface.h
        ${VEWTN_ROOT_DIR}/src/publisher_impl.h
        ${VEWTN_ROOT_DIR}/src/publisher_impl.cc
        ${VEWTN_ROOT_DIR}/src/subscriber_impl.h
        ${VEWTN_ROOT_DIR}/src/subscriber_impl.cc
        ${VEWTN_ROOT_DIR}/src/webrtc_wrapper.h
        ${VEWTN_ROOT_DIR}/src/webrtc_wrapper.cc
        ${VEWTN_ROOT_DIR}/src/whip_client.h
        ${VEWTN_ROOT_DIR}/src/whip_client.cc
        )
 
if (RTC_PLATFORM_IOS.cache)
    set(SDK_SOURCES_PREFIX ${VEWTN_ROOT_DIR}/sdk/ios)
    set(SDK_SOURCES
        ${SDK_SOURCES_PREFIX}/veWTNDefines.h
        ${SDK_SOURCES_PREFIX}/Publisher.h
        ${SDK_SOURCES_PREFIX}/Publisher.mm
        ${SDK_SOURCES_PREFIX}/PublisherEventHandler.h
        ${SDK_SOURCES_PREFIX}/PublisherEventHandler.mm
        ${SDK_SOURCES_PREFIX}/Subscriber.h
        ${SDK_SOURCES_PREFIX}/Subscriber.mm
        ${SDK_SOURCES_PREFIX}/SubscriberEventHandler.h
        ${SDK_SOURCES_PREFIX}/SubscriberEventHandler.mm
        )
elseif (RTC_PLATFORM_ANDROID.cache)
    set(SDK_SOURCES_PREFIX ${VEWTN_ROOT_DIR}/sdk/android/vewtn/src/main/jni)
    set(SDK_SOURCES
        # webrtc
        ${SDK_SOURCES_PREFIX}/webrtc/android_histogram.cc
        ${SDK_SOURCES_PREFIX}/webrtc/jni_common.cc
        ${SDK_SOURCES_PREFIX}/webrtc/logging/log_sink.h
        ${SDK_SOURCES_PREFIX}/webrtc/logging/log_sink.cc
        ${SDK_SOURCES_PREFIX}/webrtc/pc/logging.cc
        ${SDK_SOURCES_PREFIX}/webrtc/yuv_helper.cc
        ${SDK_SOURCES_PREFIX}/webrtc/video_encoder_factory_wrapper.h
        ${SDK_SOURCES_PREFIX}/webrtc/video_encoder_factory_wrapper.cc
        ${SDK_SOURCES_PREFIX}/webrtc/h264_utils.cc
        ${SDK_SOURCES_PREFIX}/webrtc/video_sink.h
        ${SDK_SOURCES_PREFIX}/webrtc/video_sink.cc
        # local project
        ${SDK_SOURCES_PREFIX}/jni_onload.cc
        ${SDK_SOURCES_PREFIX}/vewtn_jni_common.h
        ${SDK_SOURCES_PREFIX}/native_functions_jni.cc
        ${SDK_SOURCES_PREFIX}/publisher_event_handler_jni.h
        ${SDK_SOURCES_PREFIX}/publisher_event_handler_jni.cc
        ${SDK_SOURCES_PREFIX}/subscriber_event_handler_jni.h
        ${SDK_SOURCES_PREFIX}/subscriber_event_handler_jni.cc
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
