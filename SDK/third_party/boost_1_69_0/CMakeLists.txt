# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(BOOST_SOURCE
   ${BOOST_SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/boost/random/src/random_device.cpp
   PARENT_SCOPE
)

set(BOOST_INCLUDE_DIRS
        ${BOOST_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/system/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/asio/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/uuid/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/lexical_cast/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/config/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/type_traits/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/io/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/predef/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/throw_exception/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/range/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/assert/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/static_assert/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/detail/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/move/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/date_time/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/smart_ptr/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/random/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/utility/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/iterator/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/integer/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/mpl/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/preprocessor/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/tti/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/numeric_conversion/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/function_types/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/bind/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/regex/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/concept_check/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/algorithm/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/array/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/container/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/math/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/tokenizer/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/winapi/include
        ${CMAKE_CURRENT_SOURCE_DIR}/boost/crc/include
        PARENT_SCOPE
)