# CopyRight (c) 2020 ByteDance Inc. All Right Reserved.
# Macro for Apple(iOS and Mac)
#   byte_set_xcode_property
#   byte_add_pod_file
#   byte_generate_podfile
#   byte_apply_xcode_default_config
#   byte_copy_framework
#   byte_copy_dylib

# This little macro lets you set any Xcode specific property.
macro(byte_set_xcode_property)
    if (RTC_PLATFORM_APPLE.cache)
        set(extra_macro_args ${ARGN})
        list(LENGTH extra_macro_args num_args)
        if ( ${num_args} LESS 3 )
            message(FATAL_ERROR "byte_set_xcode_property need at least 3 arguments: target property value")
        endif()

        list(GET extra_macro_args 0 TARGET)
        list(GET extra_macro_args 1 XCODE_PROPERTY)
        list(GET extra_macro_args 2 XCODE_VALUE)

        if ( ${num_args} EQUAL 4 )
            list(GET extra_macro_args 3 _rel_)
        elseif( ${num_args} EQUAL 3 )
            set(_rel_ "All")
        endif()

        if("${_rel_}" STREQUAL "All")
            set_target_properties(${TARGET} PROPERTIES
                XCODE_ATTRIBUTE_${XCODE_PROPERTY} "${XCODE_VALUE}")
        else()
            set_target_properties(${TARGET} PROPERTIES
                XCODE_ATTRIBUTE_${XCODE_PROPERTY}[variant=${_rel_}] "${XCODE_VALUE}")
        endif()
    endif()
endmacro(byte_set_xcode_property)

# Create Podlist cache
if (RTC_PLATFORM_APPLE.cache)
    file(WRITE ${CMAKE_BINARY_DIR}/podlist "")
endif()

macro(byte_add_pod_file SUB_PODFILE)
    if (RTC_PLATFORM_APPLE.cache)
        file(APPEND ${CMAKE_BINARY_DIR}/podlist "${SUB_PODFILE}\n")
    endif()
endmacro(byte_add_pod_file)

macro(byte_generate_podfile)
    if (RTC_PLATFORM_APPLE.cache)
        # clear the Podfile first
        file(WRITE ${CMAKE_BINARY_DIR}/Podfile "")
        # load cached pod list
        file(STRINGS ${CMAKE_BINARY_DIR}/podlist POD_LISTS)
        # add podfile header
        byte_file_cat(${CMAKE_SOURCE_DIR}/cmake/ios.begin.Podfile.cmake ${CMAKE_BINARY_DIR}/Podfile)

        # concat each sub podfile
        foreach(SUB_POD_FILE ${POD_LISTS})
            byte_file_cat(${SUB_POD_FILE} ${CMAKE_BINARY_DIR}/Podfile)
        endforeach()
        
        # add podfile tail
        configure_file(${CMAKE_SOURCE_DIR}/cmake/ios.end.Podfile.cmake ${CMAKE_BINARY_DIR}/ios.end.Podfile)
        byte_file_cat(${CMAKE_BINARY_DIR}/ios.end.Podfile ${CMAKE_BINARY_DIR}/Podfile)
    endif()
endmacro(byte_generate_podfile)

macro(byte_apply_xcode_default_config target)
    if (RTC_PLATFORM_APPLE.cache AND (${CMAKE_GENERATOR} STREQUAL "Xcode"))
        byte_set_xcode_property(${target} CLANG_CXX_LANGUAGE_STANDARD "gnu++17")
        byte_set_xcode_property(${target} CLANG_CXX_LIBRARY "libc++")
        byte_set_xcode_property(${target} GCC_INLINES_ARE_PRIVATE_EXTERN "YES")
        byte_set_xcode_property(${target} GCC_SYMBOLS_PRIVATE_EXTERN "YES")
        byte_set_xcode_property(${target} DYLIB_COMPATIBILITY_VERSION 1)
        byte_set_xcode_property(${target} DYLIB_INSTALL_NAME_BASE "@rpath")
        byte_set_xcode_property(${target} STRIPFLAGS "-x" Release)
        byte_set_xcode_property(${target} DEPLOYMENT_POSTPROCESSING "NO" Debug)
        byte_set_xcode_property(${target} DEPLOYMENT_POSTPROCESSING "YES" Release)
        byte_set_xcode_property(${target} COPY_PHASE_STRIP "NO" Debug)
        byte_set_xcode_property(${target} COPY_PHASE_STRIP "YES" Release)
        byte_set_xcode_property(${target} STRIP_INSTALLED_PRODUCT "NO" Debug)
        byte_set_xcode_property(${target} STRIP_INSTALLED_PRODUCT "YES" Release)
        byte_set_xcode_property(${target} STRIP_STYLE "debugging" Debug)
        byte_set_xcode_property(${target} STRIP_STYLE "non-global" Release)
        byte_set_xcode_property(${target} STRIP_SWIFT_SYMBOLS "NO" Debug)
        byte_set_xcode_property(${target} STRIP_SWIFT_SYMBOLS "YES" Release)
        byte_set_xcode_property(${target} DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
        byte_set_xcode_property(${target} DEFINES_MODULE "YES")
        byte_set_xcode_property(${target} FRAMEWORK_SEARCH_PATHS "$(inherited)")
        byte_set_xcode_property(${target} ENABLE_STRICT_OBJC_MSGSEND "YES")
        byte_set_xcode_property(${target} GCC_OPTIMIZATION_LEVEL "${RTC_OPTIMIZE_LEVEL.cache}" Release)
        byte_set_xcode_property(${target} DYLIB_CURRENT_VERSION "${RTC_PRODUCT_VERSION}" All)
        byte_set_xcode_property(${target} CURRENT_PROJECT_VERSION "${RTC_PRODUCT_VERSION}" All)

        # Code Coverage
        if(RTC_ENABLE_COVERAGE.cache)
            byte_set_xcode_property(${target} GCC_GENERATE_TEST_COVERAGE_FILES "YES")
            byte_set_xcode_property(${target} GCC_INSTRUMENT_PROGRAM_FLOW_ARCS "YES")
        endif()

        # Bitcode 
        if(RTC_ENABLE_BITCODE.cache)
            byte_set_xcode_property(${target} BITCODE_GENERATION_MODE "bitcode")
            byte_set_xcode_property(${target} ENABLE_BITCODE "YES")
        else() 
            byte_set_xcode_property(${target} ENABLE_BITCODE "NO")
        endif()

        # Arc
        if(RTC_ENABLE_ARC.cache)
            byte_set_xcode_property(${target} CLANG_ENABLE_OBJC_ARC "YES")
        else()
            byte_set_xcode_property(${target} CLANG_ENABLE_OBJC_ARC "NO")
        endif()

        # Visibility
        if(RTC_ENABLE_VISIBILITY.cache)
            byte_set_xcode_property(${target} GCC_SYMBOLS_PRIVATE_EXTERN "YES")
        else()
            byte_set_xcode_property(${target} GCC_SYMBOLS_PRIVATE_EXTERN "NO")
        endif()

        # LTO
        if(RTC_ENABLE_LTO.cache)
            byte_set_xcode_property(${target} CLANG_FLTO "YES" Release)
            byte_set_xcode_property(${target} LLVM_LTO "YES" Release)
        else()
            byte_set_xcode_property(${target} CLANG_FLTO "NO" Release)
        endif()

        # Exception
        if (RTC_CPP_EXCEPTION.cache)
            byte_set_xcode_property(${target} GCC_ENABLE_CPP_EXCEPTIONS "YES" Release)
        else()
            byte_set_xcode_property(${target} GCC_ENABLE_CPP_EXCEPTIONS "NO" Release)
        endif()

        # RTTI
        if (RTC_CPP_RTTI.cache)
            byte_set_xcode_property(${target} GCC_ENABLE_CPP_RTTI "YES" Release)
        else()
            byte_set_xcode_property(${target} GCC_ENABLE_CPP_RTTI "NO" Release)
        endif()

        # Code Sign
        if (RTC_ENABLE_CODESIGN.cache)
            byte_set_xcode_property(${target} CODE_SIGNING_REQUIRED "YES" Release)
        else()
            byte_set_xcode_property(${target} CODE_SIGNING_REQUIRED "NO" Release)
        endif()

        if ( RTC_PLATFORM_IOS.cache AND (RTC_IOS_PLATFORM.cache STREQUAL "SIMULATOR") )
            # this link options is required for i386 to skip ld: illegal text-relocation to 'byte_1pos_table' in openh264
            byte_add_link_option(${target} "-read_only_relocs")
            byte_add_link_option(${target} "suprpress")
        endif()

        if ( RTC_PLATFORM_IOS.cache AND (NOT ${CMAKE_VERSION} VERSION_LESS "3.11"))
            byte_set_xcode_property(${target} IPHONEOS_DEPLOYMENT_TARGET "${RTC_IOS_DEPLOYMENT_TARGET.cache}")
        endif()
    endif()
endmacro(byte_apply_xcode_default_config)

macro(byte_copy_framework target framework)
    if (RTC_PLATFORM_APPLE.cache)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/build_system/install_framework.sh ${framework}
            )
    endif()
endmacro(byte_copy_framework)

macro(byte_copy_dylib target lib_path)
    if (RTC_PLATFORM_APPLE.cache)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/build_system/install_dylib.sh "${lib_path}"
            )
    endif()
endmacro(byte_copy_dylib)

# Append Pod info to current target
# byte_add_pdd 'target', 'podname', 'version', 'subspecs'
macro(byte_add_pod)
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_args)
    if ( ${num_args} LESS 2 )
        message(FATAL_ERROR "byte_add_pod need at least 2 arguments: target podname")
    endif()

    list(GET extra_macro_args 0 target)
    list(GET extra_macro_args 1 podname)
    set(version "")
    set(subspecs "")
    
    if( ${num_args} EQUAL 4 )
        list(GET extra_macro_args 2 version)
        list(GET extra_macro_args 3 subspecs)
    elseif( ${num_args} EQUAL 3 )
        list(GET extra_macro_args 2 version)
    endif()

    # Remove all white space
    if (NOT "${subspecs}" STREQUAL "")
        string(REPLACE " " "" subspecs "${subspecs}")
    endif()
    list(APPEND ${target}_pods "${podname}?${version}!?${subspecs}!")
endmacro(byte_add_pod)

macro(byte_generate_pod target)
    set(POD_FILE_PATH "${CMAKE_BINARY_DIR}/${target}.Podfile")
    # Add Podfile
    file(WRITE ${POD_FILE_PATH} "")
    file(APPEND ${POD_FILE_PATH} "target '${target}' do\n")
    if(RTC_PLATFORM_IOS.cache)
        file(APPEND ${POD_FILE_PATH} "  platform :ios, :deployment_target => '${RTC_IOS_DEPLOYMENT_TARGET.cache}'\n")
    else()
        file(APPEND ${POD_FILE_PATH} "  platform :osx, :deployment_target => '${RTC_MACOSX_DEPOLYMENT_TARGET.cmake}'\n")
    endif()

    foreach(podinfo ${${target}_pods})
        string(REPLACE "?" ";" podinfo "${podinfo}")
        list(GET podinfo 0 podname)
        list(GET podinfo 1 version)
        list(GET podinfo 2 subspecs)
        string(REPLACE "!" "" version "${version}")
        string(REPLACE "!" "" subspecs "${subspecs}")

        file(APPEND ${POD_FILE_PATH} "  pod '${podname}'")
        if (NOT "${version}" STREQUAL "")
            string(SUBSTRING ${version} 0 1 first_letter)
            if ("${first_letter}" STREQUAL ":")
                file(APPEND ${POD_FILE_PATH} ", ${version}")
            else()
                file(APPEND ${POD_FILE_PATH} ", '${version}'")
            endif()
        endif()

        if (NOT "${subspecs}" STREQUAL "")
            file(APPEND ${POD_FILE_PATH} ", :subspecs => [\n")
            string(REPLACE "," ";" subspecs "${subspecs}")
            foreach(spec ${subspecs})
                file(APPEND ${POD_FILE_PATH} "    '${spec}',\n")
            endforeach()
            file(APPEND ${POD_FILE_PATH} "  ]")
        endif()
        file(APPEND ${POD_FILE_PATH} "\n")
    endforeach()

    file(APPEND ${POD_FILE_PATH} "end\n")
    byte_add_pod_file(${POD_FILE_PATH})
endmacro(byte_generate_pod target)



