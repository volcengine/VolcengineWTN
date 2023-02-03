cmake_minimum_required(VERSION 3.10.0)

# add compiler macros
target_compile_definitions(${TARGET_NAME} PRIVATE WEBRTC_POSIX WEBRTC_LINUX)

# link libs
target_link_libraries(${TARGET_NAME} PUBLIC dl)
