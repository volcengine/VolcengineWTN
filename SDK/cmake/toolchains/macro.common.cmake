# CopyRight (c) 2020 ByteDance Inc. All Right Reserved.
# Common Macros
#   byte_dump_value
#   byte_group_sources
#   byte_file_cat
#   byte_config_output_dir
#   byte_recursive_include_search
#   byte_apply_default_target_options
#   byte_remove_compile_option
#   byte_add_compile_option
#   byte_remove_link_option
#   byte_add_link_option
#   byte_init_default_flags
#   byte_save_dafault_flags
#   byte_add_default_compile_flag
#   byte_add_default_link_flag

set(RTC_SUPPORTED_LANGUAGE C CXX)
set(RTC_LINK_TYPES EXECUTABLE MODULE_LIBRARY STATIC_LIBRARY SHARED_LIBRARY OBJECT_LIBRARY INTERFACE_LIBRARY)

# Get Cmake args 
set(CMAKE_ARGS "")
macro(byte_capture_cmake_args)
    execute_process(
        COMMAND python3 ${CMAKE_SOURCE_DIR}/build_system/scripts/cmakeargs.py 
        RESULT_VARIABLE RET_CMAKE_ARGS_HELPER_PYTHON
        OUTPUT_VARIABLE CMAKE_ARGS)
    string(STRIP "${CMAKE_ARGS}" CMAKE_ARGS)
    if ( NOT RET_CMAKE_ARGS_HELPER_PYTHON EQUAL 0 )
        message(FATAL_ERROR "failed to capture cmake arguments")
    endif()
    message(STATUS "Current Commands: ${CMAKE_ARGS}")
    set_cache(CMAKE_ARGS)
endmacro(byte_capture_cmake_args)

# Dump a variable
macro(byte_dump_value VALUE)
    message(STATUS "DUMP: ${VALUE}: ${${VALUE}}")
endmacro(byte_dump_value)

# Group sources into different source group according to the file path
function(byte_group_sources target begin_dir)
    set(SOURCE_GROUP_DELIMITER "/")
    set(last_dir "")
    set(files "")

    get_target_property(sources ${target} SOURCES)

    foreach(file ${sources})
        file(RELATIVE_PATH relative_file "${begin_dir}" ${file})
        get_filename_component(dir "${relative_file}" PATH)
        if(NOT "${dir}" STREQUAL "${last_dir}")
            if(files)
                source_group("${last_dir}" FILES ${files})
            endif()
            set(files "")
        endif()
        set(files ${files} ${file})
        set(last_dir "${dir}")
    endforeach()

    if(files)
        source_group("${last_dir}" FILES ${files})
    endif()
endfunction(byte_group_sources)

# connect file, connect input_file after output_file
macro(byte_file_cat input_file output_file)
    file(READ ${input_file} TEMP_CONTENTS__)
    file(APPEND ${output_file} "${TEMP_CONTENTS__}")
endmacro(byte_file_cat)

# make the given target's output items in CMAKE_BINARY_DIR/<RELVERSION>
function(byte_config_output_dir target)
    if (NOT RTC_PLATFORM_ANDROID.cache)
        set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
        set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
        set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            message(STATUS "Set output dir for ${build_type}")
            string(TOUPPER "${build_type}" upper_build_type)
            set_target_properties(
                ${target} PROPERTIES 
                RUNTIME_OUTPUT_DIRECTORY_${upper_build_type} 
                ${CMAKE_BINARY_DIR}/${build_type})
            set_target_properties(
                ${target} PROPERTIES 
                LIBRARY_OUTPUT_DIRECTORY_${upper_build_type} 
                ${CMAKE_BINARY_DIR}/${build_type})
            set_target_properties(
                ${target} PROPERTIES 
                ARCHIVE_OUTPUT_DIRECTORY_${upper_build_type} 
                ${CMAKE_BINARY_DIR}/${build_type})
        endforeach()
    endif()
endfunction(byte_config_output_dir)

# Search all folders include any header files
macro(byte_recursive_include_search begin_folder result_list)
    set(${result_list} "")
    file(GLOB_RECURSE _temp_list_ "${begin_folder}/*.h")
    set(_all_dir_list_ "")
    foreach(fpath ${_temp_list_})
        get_filename_component(_temp_dir_path_ ${fpath} PATH)
        list(APPEND _all_dir_list_ "\"${_temp_dir_path_}\"")
    endforeach()
    list(REMOVE_DUPLICATES _all_dir_list_)
    set(${result_list} ${_all_dir_list_})
endmacro(byte_recursive_include_search)

# format flags_string into list var, and save to cache with surfix "_cache"
macro(byte_flags_to_list var flags_string)
    set(_local_${var} ${flags_string})
    string(REPLACE " " ";" ${var} ${_local_${var}})
    # remove duplicate items
    list(REMOVE_DUPLICATES ${var})
    # remove empty items
    list(REMOVE_ITEM ${var} "")
endmacro(byte_flags_to_list)

macro(byte_options_to_string option out_value)
    set(temp "${option}")
    string(REPLACE "@" " " temp "${temp}")
    string(REPLACE "$" "" temp "${temp}")
    string(REPLACE "<" ";" temp "${temp}")
    string(REPLACE ">" ";" temp "${temp}")
    string(REPLACE ":/" "/" temp "${temp}")
    string(REPLACE ":-" "-" temp "${temp}")
    foreach(op ${temp})
        string(REPLACE " " "" temp_op "${op}")
        if ( NOT "${temp_op}" STREQUAL "" )
            set(${out_value} "${op}")
        endif()
    endforeach()
endmacro(byte_options_to_string)

macro(byte_lang_type_to_xcode_flag type xtype)
    if (${type} STREQUAL "C")
        set(${xtype} "C")
    elseif(${type} STREQUAL "CXX")
        set(${xtype} "CPLUSPLUS")
    endif()
endmacro(byte_lang_type_to_xcode_flag)

macro(byte_sync_compile_option target)
    if ((NOT RTC_PLATFORM_APPLE) OR (NOT CMAKE_GENERATOR STREQUAL "Xcode"))
        set_property(TARGET ${target} PROPERTY COMPILE_OPTIONS)
    else()
        # Clear Xcode config and reset
        foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
            foreach(build_type ${CMAKE_TRY_COMPILE_CONFIGURATION})
                byte_lang_type_to_xcode_flag(${lang_type} xlang_type)
                set_property(TARGET ${target} PROPERTY XCODE_ATTRIBUTE_OTHER_${xlang_type}FLAGS[variant=${build_type}])
            endforeach()
        endforeach()
    endif()
    # Save the global compile option
    foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            string(
                REPLACE ";" " " 
                ${target}_${lang_type}_flags_${build_type}_string 
                "${${target}_${lang_type}_flags_${build_type}}"
            )
            byte_dump_value(${target}_${lang_type}_flags_${build_type})
            if (RTC_PLATFORM_APPLE.cache AND (CMAKE_GENERATOR STREQUAL "Xcode"))
                byte_lang_type_to_xcode_flag(${lang_type} xlang_type)
                if (NOT "${${target}_${lang_type}_flags_${build_type}_string}" STREQUAL "")
                    set_target_properties(${target} PROPERTIES 
                        XCODE_ATTRIBUTE_OTHER_${xlang_type}FLAGS[variant=${build_type}] 
                        "${${target}_${lang_type}_flags_${build_type}_string}")
                endif()
            else()
                if (NOT "${${target}_${lang_type}_flags_${build_type}_string}" STREQUAL "")
                    target_compile_options(${target} PRIVATE
                        "$<$<AND:$<CONFIG:${build_type}>,$<COMPILE_LANGUAGE:${lang_type}>>:${${target}_${lang_type}_flags_${build_type}}>")
                endif()
            endif()
        endforeach()
    endforeach()
endmacro(byte_sync_compile_option)

# Re-sync the link flags option
macro(byte_sync_link_option target)
    # Save to cache
    foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER "${build_type}" upper_build_type)
        # clear old property
        if (RTC_PLATFORM_APPLE.cache AND (CMAKE_GENERATOR STREQUAL "Xcode"))
            set_property(TARGET ${target} PROPERTY XCODE_ATTRIBUTE_OTHER_LDFLAGS[variant=${build_type}])
        else()
            if ( ${CMAKE_VERSION} VERSION_LESS "3.13")
                set_property(TARGET ${target} PROPERTY LINK_FLAGS_${upper_build_type})
            else()
                set_property(TARGET ${target} PROPERTY LINK_OPTIONS)
            endif()
        endif()

        byte_dump_value(${target}_link_flags_${build_type})

        if (NOT ${target}_link_flags_${build_type} STREQUAL "")
            if (RTC_PLATFORM_APPLE.cache AND (CMAKE_GENERATOR STREQUAL "Xcode"))
                string(REPLACE ";" " " 
                    ${target}_link_flags_${build_type}_string 
                    "${${target}_link_flags_${build_type}}")
                set_target_properties(${target} PROPERTIES
                    XCODE_ATTRIBUTE_OTHER_LDFLAGS[variant=${build_type}]
                    "${${target}_link_flags_${build_type}_string}")
            else()
                if (${CMAKE_VERSION} VERSION_LESS "3.13")
                    string(REPLACE ";" " " 
                        ${target}_link_flags_${build_type}_string 
                        "${${target}_link_flags_${build_type}}")
                    set_target_properties(${target} PROPERTIES 
                        LINK_FLAGS_${upper_build_type} 
                        "${${target}_link_flags_${build_type}_string}")
                else()
                    set_target_properties(${target} PROPERTIES LINK_OPTIONS
                        "$<$<CONFIG:${build_type}>:${${target}_link_flags_${build_type}}>")
                endif()
            endif()
        endif()
    endforeach()
endmacro(byte_sync_link_option)

# save default compile & link flags to given target
macro(byte_apply_default_target_options target)
    get_target_property(_target_type_ ${target} TYPE)

    # clear cache
    set(${target}_global_compile_options "" CACHE STRING "")
    foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
        set(${target}_old_${lang_type}_flags "")
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            set(${target}_old_${lang_type}_flags_${build_type} "" CACHE STRING "")
        endforeach()
    endforeach()

    # get current compile option if any
    get_target_property(${target}_compile_options ${target} COMPILE_OPTIONS)
    if ( ${target}_compile_options )
        string(REPLACE ";" "@" ${target}_format_compile_options "${${target}_compile_options}")
        string(REPLACE "$<$" ";$<$" ${target}_compile_options "${${target}_format_compile_options}")

        foreach(_option ${${target}_compile_options})
            if (NOT "${_option}" MATCHES "^\\$\\<\\$.*\\>")
                string(REPLACE "@" " " ${target}_global_compile_options "${_option}")
            else()
                foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
                    if ( "${_option}" MATCHES "\\$\\<COMPILE_LANGUAGE\\:${lang_type}\\>" )
                        set(match_build_type NO)
                        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
                            if ( "${_option}" MATCHES "\\$\\<CONFIG\\:${build_type}\\>" )
                                set(match_build_type YES)
                                byte_options_to_string(${_option} ${target}_old_${lang_type}_flags_${build_type})
                            endif()
                        endforeach()
                        if (NOT match_build_type)
                            byte_options_to_string(${_option} ${target}_old_${lang_type}_flags)
                        endif()
                    endif()
                endforeach()
            endif()
        endforeach()
    endif()

    # Sync Compile Flags
    if ((NOT RTC_PLATFORM_APPLE) OR (NOT CMAKE_GENERATOR STREQUAL "Xcode"))
        byte_dump_value(${target}_global_compile_options)
    endif()

    foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER "${build_type}" upper_build_type)
            byte_flags_to_list(
                ${target}_${lang_type}_flags_${build_type} 
                "${CMAKE_${lang_type}_FLAGS} ${CMAKE_${lang_type}_FLAGS_${upper_build_type}} ${RTC_${lang_type}_FLAGS.cache} ${RTC_${lang_type}_FLAGS_${build_type}.cache} ${${target}_old_${lang_type}_flags} ${${target}_old_${lang_type}_flags_${build_type}} ${${target}_global_compile_options}")
            byte_dump_value(${target}_${lang_type}_flags_${build_type})
        endforeach()
    endforeach()

    byte_sync_compile_option(${target})

    # Sync Link Flags
    foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER "${build_type}" upper_build_type)
        set(${target}_link_flags_${build_type} "" CACHE STRING "")
        byte_flags_to_list(
            ${target}_link_flags_${build_type}
            "${RTC_LINK_FLAGS.cache} ${RTC_LINK_FLAGS_${build_type}.cache} ${RTC_LINK_FLAGS_${_target_type_}.cache} ${RTC_LINK_FLAGS_${_target_type_}_${build_type}.cache} ${CMAKE_LINK_FLAGS} ${CMAKE_LINK_FLAGS_${upper_build_type}}"
        )
        byte_dump_value(${target}_link_flags_${build_type})
    endforeach()
    byte_sync_link_option(${target})

endmacro(byte_apply_default_target_options)

# Must invoke byte_apply_default_target_options first
macro(byte_remove_compile_option)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 3 )
        message(FATAL_ERROR "byte_remove_compile_option need at least 2 arguments: target, option")
    endif()

    list(GET extra_macro_args 0 target)
    list(GET extra_macro_args 1 option)

    if ( ${num_args} EQUAL 4 )
        list(GET extra_macro_args 2 _lang_)
        list(GET extra_macro_args 3 _rel_)
    elseif( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 2 _lang_)
        set(_rel_ "All")
    elseif( ${num_args} EQUAL 2 )
        set(_lang_ "All")
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()
    if (${_lang_} STREQUAL "All")
        set(_LANG_ ${RTC_SUPPORTED_LANGUAGE})
    else()
        set(_LANG_ ${_lang_})
    endif()

    foreach(build_type ${_CONFIG_})
        foreach(lang_type ${_LANG_})
            list(REMOVE_ITEM ${target}_${lang_type}_flags_${build_type} "${option}")
        endforeach()
    endforeach()
    byte_sync_compile_option(${target})
endmacro(byte_remove_compile_option)

macro(byte_add_compile_option)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 3 )
        message(FATAL_ERROR "byte_add_compile_option need at least 2 arguments: target, option")
    endif()

    list(GET extra_macro_args 0 target)
    list(GET extra_macro_args 1 option)
    
    if ( ${num_args} EQUAL 4 )
        list(GET extra_macro_args 2 _lang_)
        list(GET extra_macro_args 3 _rel_)
    elseif( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 2 _lang_)
        set(_rel_ "All")
    elseif( ${num_args} EQUAL 2 )
        set(_lang_ "All")
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()
    if (${_lang_} STREQUAL "All")
        set(_LANG_ ${RTC_SUPPORTED_LANGUAGE})
    else()
        set(_LANG_ ${_lang_})
    endif()

    foreach(build_type ${_CONFIG_})
        foreach(lang_type ${_LANG_})
            list(APPEND ${target}_${lang_type}_flags_${build_type} "${option}")
            list(REMOVE_DUPLICATES ${target}_${lang_type}_flags_${build_type}) 
        endforeach()
    endforeach()
    byte_sync_compile_option(${target})
endmacro(byte_add_compile_option)

# Remove Link Flag
macro(byte_remove_link_option)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 3 )
        message(FATAL_ERROR "byte_remove_link_option need at least 2 arguments: target, option")
    endif()

    list(GET extra_macro_args 0 target)
    list(GET extra_macro_args 1 option)
    
    if ( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 2 _rel_)
    elseif( ${num_args} EQUAL 2 )
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()

    foreach(build_type ${_CONFIG_})
        list(REMOVE_ITEM ${target}_link_flags_${build_type} "${option}")
    endforeach()
    byte_sync_link_option(${target})
endmacro(byte_remove_link_option)

# Add Link Flags
macro(byte_add_link_option)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 3 )
        message(FATAL_ERROR "byte_add_link_option need at least 2 arguments: target, option")
    endif()

    list(GET extra_macro_args 0 target)
    list(GET extra_macro_args 1 option)
    
    if ( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 2 _rel_)
    elseif( ${num_args} EQUAL 2 )
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()

    foreach(build_type ${_CONFIG_})
        list(APPEND ${target}_link_flags_${build_type} "${option}")
        list(REMOVE_DUPLICATES ${target}_link_flags_${build_type})
    endforeach()
    byte_sync_link_option(${target})
endmacro(byte_add_link_option)

macro(byte_init_default_flags)
    foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
        set(RTC_${lang_type}_FLAGS "")
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            set(RTC_${lang_type}_FLAGS_${build_type} "")
        endforeach()
    endforeach()

    set(RTC_LINK_FLAGS "")
    foreach(link_type ${RTC_LINK_TYPES})
        set(RTC_LINK_FLAGS_${link_type} "")
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            set(RTC_LINK_FLAGS_${link_type}_${build_type} "")
        endforeach()
    endforeach()
endmacro(byte_init_default_flags)

macro(byte_save_dafault_flags)
    foreach(lang_type ${RTC_SUPPORTED_LANGUAGE})
        set_cache(RTC_${lang_type}_FLAGS "")
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            set_cache(RTC_${lang_type}_FLAGS_${build_type})
        endforeach()
    endforeach()

    set(RTC_LINK_FLAGS "")
    foreach(link_type ${RTC_LINK_TYPES})
        set_cache(RTC_LINK_FLAGS_${link_type} "")
        foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
            set_cache(RTC_LINK_FLAGS_${link_type}_${build_type})
        endforeach()
    endforeach()
endmacro(byte_save_dafault_flags)

# Flag manager 
macro(byte_add_flag_internal var flag)
    set(${var} "${${var}} ${flag}")
    set(${var}.cache "${${var}.cache} ${flag}")
endmacro(byte_add_flag_internal)

macro(byte_add_default_compile_flag)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 1 )
        message(FATAL_ERROR "byte_add_default_compile_flag need at least 1 arguments: flag")
    endif()

    list(GET extra_macro_args 0 flag)
    
    if ( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 1 _lang_)
        list(GET extra_macro_args 2 _rel_)
    elseif( ${num_args} EQUAL 2 )
        list(GET extra_macro_args 1 _lang_)
        set(_rel_ "All")
    elseif( ${num_args} EQUAL 1 )
        set(_lang_ "All")
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()
    if (${_lang_} STREQUAL "All")
        set(_LANG_ ${RTC_SUPPORTED_LANGUAGE})
    else()
        set(_LANG_ ${_lang_})
    endif()

    foreach(build_type ${_CONFIG_})
        foreach(lang_type ${_LANG_})
            byte_add_flag_internal(RTC_${lang_type}_FLAGS_${build_type} ${flag})
        endforeach()
    endforeach()
endmacro(byte_add_default_compile_flag)

macro(byte_add_default_link_flag)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 1 )
        message(FATAL_ERROR "byte_add_default_link_flag need at least 1 arguments: flag")
    endif()

    list(GET extra_macro_args 0 flag)
    
    if ( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 1 _rel_)
        list(GET extra_macro_args 2 _type_)
    elseif( ${num_args} EQUAL 2)
        list(GET extra_macro_args 1 _rel_)
        set(_type_ "All")
    elseif( ${num_args} EQUAL 1 )
        set(_type_ "All")
        set(_rel_ "All")
    endif()

    if (${_rel_} STREQUAL "All")
        set(_CONFIG_ ${CMAKE_CONFIGURATION_TYPES})
    else()
        set(_CONFIG_ ${_rel_})
    endif()
    if (${_type_} STREQUAL "All")
        set(_TYPE_ ${RTC_LINK_TYPES})
    else()
        set(_TYPE_ ${_type_})
    endif()

    foreach(build_type ${_CONFIG_})
        foreach(link_type ${_TYPE_})
            byte_add_flag_internal(RTC_LINK_FLAGS_${link_type}_${build_type} ${flag})
        endforeach()
    endforeach()
endmacro(byte_add_default_link_flag)

macro(byte_recursive_get_directories subdir dir_list)
    set(temp_dir_lists "")
    get_property(temp_dir_lists
        DIRECTORY "${subdir}"
        PROPERTY SUBDIRECTORIES)
    foreach(dir ${temp_dir_lists})
        byte_recursive_get_directories(${dir} ${dir_list})
    endforeach()
    list(APPEND ${dir_list} ${subdir})
    list(REMOVE_DUPLICATES ${dir_list})
endmacro(byte_recursive_get_directories)

macro(byte_get_all_targets subdir targets)
    set(subdirs_list "")
    byte_recursive_get_directories(${subdir} subdirs_list)

    # Clear output targets
    set(${targets} "")
    foreach(dir ${subdirs_list})
        get_property(_targets
            DIRECTORY "${dir}"
            PROPERTY BUILDSYSTEM_TARGETS)
        if ( _targets )
            list(APPEND ${targets} ${_targets})
        endif()
    endforeach()
    list(REMOVE_DUPLICATES ${targets})

    foreach(tar ${${targets}})
        get_target_property(target_type ${tar} TYPE)
        message(STATUS "${tar} type: ${target_type}")
        if ( ("${target_type}" STREQUAL "INTERFACE_LIBRARY") OR ("${target_type}" STREQUAL "UTILITY") )
            list(REMOVE_ITEM ${targets} ${tar})
        endif()
    endforeach()

endmacro(byte_get_all_targets)
