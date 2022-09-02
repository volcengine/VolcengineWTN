# Copyright (c) 2020 The ByteRtc project authors. All Right Reserved.
# ByteDance Inc.
# CMake Project Files for RTC SDK Master

set(jni_script "${CMAKE_SOURCE_DIR}/build_system/jni_generator_tool/jni_generator.py")

# Default python command is just `python`, can change to `python3` or a bin path
# this setting will only affect the given target
macro(byte_set_python target python_cmd)
    if (RTC_PLATFORM_ANDROID.cache)
        set(${target}_RTC_PYTHON_CMD "${python_cmd}")
    endif()
endmacro(byte_set_python)

# Set current target's default helper file
macro(byte_set_jni_helper target helper_file)
    if (RTC_PLATFORM_ANDROID.cache)
        set(${target}_RTC_JNI_HELPER "${helper_file}")
    endif()
endmacro(byte_set_jni_helper)

# Set current target's default namespace
macro(byte_set_jni_namespace target namespace)
    if (RTC_PLATFORM_ANDROID.cache)
        set(${target}_RTC_JNI_NAMESPACE "${namespace}")
    endif()
endmacro(byte_set_jni_namespace)

# Set current target's default output_dir
macro(byte_set_jni_outputdir target dirname)
    if (RTC_PLATFORM_ANDROID.cache)
        set(${target}_RTC_JNI_OUTPUTDIR "${namespace}")
    endif()
endmacro(byte_set_jni_outputdir)

# Add Jni Config to current target
macro(byte_add_jni)
    if (RTC_PLATFORM_ANDROID.cache)
        set(extra_macro_args ${ARGN})
        list(LENGTH extra_macro_args num_args)
        if ( ${num_args} LESS 2 )
            message(FATAL_ERROR "byte_add_jni need at least 2 arguments: target input_file")
        endif()

        list(GET extra_macro_args 0 target)
        list(GET extra_macro_args 1 input_file)
        set(output_dir ${${target}_RTC_JNI_OUTPUTDIR})
        set(helper ${${target}_RTC_JNI_HELPER})
        set(namespace ${${target}_RTC_JNI_NAMESPACE})

        if ( ${num_args} GREATER 2 )
            list(GET extra_macro_args 2 output_dir)
        endif()

        if ( ${num_args} GREATER 3 )
            list(GET extra_macro_args 3 helper)
        endif()

        if ( ${num_args} GREATER 4 )
            list(GET extra_macro_args 4 namespace)
        endif()

        string(REPLACE " " "@" input_file "${input_file}")
        string(REPLACE " " "@" output_dir "${output_dir}")
        string(REPLACE " " "@" helper "${helper}")
        list(APPEND ${target}_RTC_JNI_FILE_LIST "${input_file}!,${output_dir}!,${helper}!,${namespace}!")
    endif()
endmacro(byte_add_jni)

# Init all JNI related vars
macro(byte_begin_jni target)
    set(${target}_RTC_PYTHON_CMD "python")
    set(${target}_RTC_JNI_FILE_LIST "")
    set(${target}_RTC_JNI_HELPER "")
    set(${target}_RTC_JNI_NAMESPACE "")
    set(${target}_RTC_JNI_OUTPUTDIR "")
endmacro(byte_begin_jni)

# Generate a jni target
macro(byte_end_jni target)
    if (RTC_PLATFORM_ANDROID.cache)
        set(jni_target_name "${target}_jni")
        add_custom_target(${jni_target_name})
        # Our target dependended on jni target
        # So we need to generate jni file first
        add_dependencies(${target} ${jni_target_name})

        # get each jni configuration
        foreach(jni_file ${${target}_RTC_JNI_FILE_LIST})
            # Convert the jni config to list
            string(REPLACE "," ";" jni_config "${jni_file}")
            list(GET jni_config 0 input_file)
            list(GET jni_config 1 output_dir)
            list(GET jni_config 2 helper)
            list(GET jni_config 3 namespace)
            # Remove the last '!'
            string(REPLACE "!" "" input_file "${input_file}")
            string(REPLACE "@" " " input_file "${input_file}")
            string(REPLACE "!" "" output_dir "${output_dir}")
            string(REPLACE "@" " " output_dir "${output_dir}")
            string(REPLACE "!" "" helper "${helper}")
            string(REPLACE "@" " " helper "${helper}")
            string(REPLACE "!" "" namespace "${namespace}")
            string(REPLACE "@" " " namespace "${namespace}")

            get_filename_component(file_name ${input_file} NAME)
            string(REPLACE "." "_" file_name "${file_name}")
            set(jni_file_target_name "${jni_target_name}_${file_name}")
            add_custom_target(${jni_file_target_name})
            add_dependencies(${jni_target_name} ${jni_file_target_name})

            # Add --input_file
            if (input_file STREQUAL "")
                message(FATAL_ERROR "JNI Generator error! missing input file")
            endif()

            # Add --output_dir
            if (output_dir STREQUAL "")
                message(FATAL_ERROR "JNI Generator error! missing output dir")
            endif()
            if ( NOT EXISTS ${output_dir} )
                make_directory(${output_dir})
            endif()

            if ( (NOT helper STREQUAL "") AND (NOT namespace STREQUAL "") )
                add_custom_command(
                    TARGET ${jni_file_target_name}
                    COMMAND ${${target}_RTC_PYTHON_CMD} ${jni_script}
                        "--input_file" "${input_file}" 
                        "--output_dir" "${output_dir}"
                        "-n" "${namespace}"
                        "--includes" "${helper}"
                )
            elseif( (helper STREQUAL "") AND (NOT namespace STREQUAL "") )
                add_custom_command(
                    TARGET ${jni_file_target_name}
                    COMMAND ${${target}_RTC_PYTHON_CMD} ${jni_script}
                        "--input_file" "${input_file}" 
                        "--output_dir" "${output_dir}"
                        "-n" "${namespace}"
                )
            elseif( (NOT helper STREQUAL "") AND (NOT namespace STREQUAL "") )
                add_custom_command(
                    TARGET ${jni_file_target_name}
                    COMMAND ${${target}_RTC_PYTHON_CMD} ${jni_script}
                        "--input_file" "${input_file}" 
                        "--output_dir" "${output_dir}"
                        "--includes" "${helper}"
                )
            endif()
        endforeach()
    endif()
endmacro(byte_end_jni)

# Init no maven urls
set(RTC_CMAKE_GRADLE_MORE_MAVEN "")

# Create new android project configuration
set(RTC_ANDROID_PROJECTS "")
set(RTC_ANDROID_SETTING_KTS "${CMAKE_BINARY_DIR}/settings.gradle.kts")
set(RTC_GRADLE_LOCAL_PROPERTIES "")
set(RTC_CMAKE_GRADLE_BINARY_DIR "${CMAKE_BINARY_DIR}")
set(RTC_CMAKE_GRADLE_ARGUMENTS_FILE "${CMAKE_BINARY_DIR}/arguments.properties")
set(RTC_CMAKE_GRADLE_PROP_FILE "${CMAKE_BINARY_DIR}/local.properties")
set(RTC_CMAKE_GRADLE_MORE_MAVEN "")

macro(byte_add_android_local_prop key value)
    set(__lp_${key}__ ${value})
    list(APPEND RTC_GRADLE_LOCAL_PROPERTIES "${key}")
    list(REMOVE_DUPLICATES RTC_GRADLE_LOCAL_PROPERTIES)
endmacro(byte_add_android_local_prop)

macro(byte_add_android_proj proj_name proj_path)
    if (RTC_PLATFORM_ANDROID.cache)
        string(LENGTH "${proj_name}" proj_len)
        math(EXPR proj_remine "${proj_len} - 1")
        string(SUBSTRING "${proj_name}" 0 1 first_letter)
        string(SUBSTRING "${proj_name}" 1 ${proj_remine} everything_else)
        string(TOLOWER "${first_letter}" l_proj_name)
        string(APPEND l_proj_name "${everything_else}")

        make_directory(${CMAKE_BINARY_DIR}/${proj_name})
        list(APPEND RTC_ANDROID_PROJECTS "${l_proj_name}+${proj_name}+${proj_path}")
        byte_add_android_local_prop(${proj_name}BuildDir ${CMAKE_BINARY_DIR}/${proj_name})
        byte_add_android_local_prop(${proj_name}StagingDir ${CMAKE_BINARY_DIR}/${proj_name}-Staging)
        set(RTC_CMAKE_GRADLE_BINARY_DIR "${CMAKE_BINARY_DIR}/${proj_name}")
    endif()
endmacro(byte_add_android_proj)

macro(byte_add_android_maven maven)
    set(RTC_CMAKE_GRADLE_MORE_MAVEN "${RTC_CMAKE_GRADLE_MORE_MAVEN}
        maven { url = uri(\"${maven}\") }")
endmacro()

macro(byte_copy_android_helper)
    if ( RTC_PLATFORM_ANDROID.cache)
        file(WRITE "${RTC_CMAKE_GRADLE_ARGUMENTS_FILE}" "")
        file(WRITE "${RTC_CMAKE_GRADLE_PROP_FILE}" "")
        # Dump all cmd args
        foreach(arg ${CMAKE_ARGS.cache})
            byte_dump_value(arg)
            file(APPEND ${RTC_CMAKE_GRADLE_ARGUMENTS_FILE} "${arg}\n")
        endforeach()

        # Dump all local properties
        foreach(lp ${RTC_GRADLE_LOCAL_PROPERTIES})
            file(APPEND ${RTC_CMAKE_GRADLE_PROP_FILE} "${lp}=${__lp_${lp}__}\n")
        endforeach()

        # Make the helper package dir
        make_directory("${CMAKE_BINARY_DIR}/buildSrc/src/main/java/byte/cmake/helper")
        configure_file(${RTC_CMAKE_PATH}/android/build.gradle.kts.cmake ${CMAKE_BINARY_DIR}/build.gradle.kts)
        configure_file(${RTC_CMAKE_PATH}/android/settings.gradle.kts.cmake ${CMAKE_BINARY_DIR}/settings.gradle.kts)
        configure_file(${RTC_CMAKE_PATH}/android/helper.build.gradle.kts.cmake 
            ${CMAKE_BINARY_DIR}/buildSrc/build.gradle.kts)
        configure_file(${RTC_CMAKE_PATH}/android/helper.StringExtension.kt.cmake 
            ${CMAKE_BINARY_DIR}/buildSrc/src/main/java/byte/cmake/helper/StringExtension.kt)
        configure_file(${RTC_CMAKE_PATH}/android/helper.PropertyReader.kt.cmake 
            ${CMAKE_BINARY_DIR}/buildSrc/src/main/java/byte/cmake/helper/PropertyReader.kt)
        configure_file(${RTC_CMAKE_PATH}/android/helper.ArgumentsLoader.kt.cmake 
            ${CMAKE_BINARY_DIR}/buildSrc/src/main/java/byte/cmake/helper/ArgumentsLoader.kt @ONLY)
    endif()
endmacro(byte_copy_android_helper)

macro(byte_final_android_generate)
    if (RTC_PLATFORM_ANDROID.cache)
        byte_dump_value(RTC_ANDROID_PROJECTS)
        byte_copy_android_helper()

        file(APPEND ${RTC_ANDROID_SETTING_KTS} "object ProjectHolder {\n")
        file(APPEND ${RTC_ANDROID_SETTING_KTS} "    var root: ProjectDescriptor? = null\n")

        foreach(proj_info ${RTC_ANDROID_PROJECTS})
            byte_dump_value(proj_info)
            string(REPLACE "+" ";" proj_info_list "${proj_info}")
            list(GET proj_info_list 0 l_proj_name)
            file(APPEND ${RTC_ANDROID_SETTING_KTS} "    var ${l_proj_name}: ProjectDescriptor? = null\n")
        endforeach()

        file(APPEND ${RTC_ANDROID_SETTING_KTS} "}\n")

        foreach(proj_info ${RTC_ANDROID_PROJECTS})
            byte_dump_value(proj_info)
            string(REPLACE "+" ";" proj_info_list "${proj_info}")
            byte_dump_value(proj_info_list)
            list(GET proj_info_list 0 l_proj_name)
            list(GET proj_info_list 1 proj_name)
            list(GET proj_info_list 2 proj_path)
            file(APPEND ${RTC_ANDROID_SETTING_KTS} 
"
include(\":${proj_name}\")
project(\":${proj_name}\").apply {
    projectDir = File(\"${proj_path}\")
}.also {
    ProjectHolder.${l_proj_name} = it
}
")
        endforeach()

    endif()
endmacro(byte_final_android_generate)
