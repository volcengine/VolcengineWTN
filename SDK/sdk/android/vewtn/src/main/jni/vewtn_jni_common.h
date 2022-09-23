/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

namespace webrtc {

#define JNI_CHECK_NULL_AND_RETURN(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return;                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_INT(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return -1;                              \
            }                                                 \
        }                                                   \
    } while(0)

// specific for return pointer int value (in jni)
#define JNI_CHECK_NULL_AND_RETURN_ZERO(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return 0;                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_BOOL(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return false;                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_NULLPTR(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return nullptr;                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_EMPTY_JNI_STRING(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return rtc::NativeToJavaString(env, "");                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_NULL_JNI_OBJECT(...)                                 \
    do {                                                  \
        long long parameters[] = { __VA_ARGS__ };                       \
        int index;                                             \
        for (index = 0; index < sizeof(parameters)/sizeof(*parameters); index++) {   \
            if (parameters[index] == 0 || parameters[index] == -1) {                             \
                return base::android::ScopedJavaLocalRef<jobject>(nullptr);                              \
            }                                                 \
        }                                                   \
    } while(0)

#define JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN(parameter)                                 \
    if (parameter.is_null() || parameter.obj() == nullptr) {                             \
        return;                              \
    }

#define JNI_CHECK_JNI_OBJEC_IS_NULL_AND_RETURN_BOOL(parameter)                                 \
    if (parameter.is_null() || parameter.obj() == nullptr) {                             \
        return false;                              \
    }

}  // namespace vewtn