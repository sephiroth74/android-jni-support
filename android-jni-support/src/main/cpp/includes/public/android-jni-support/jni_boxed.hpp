// (c) Copyright 2004-2007.  Adobe Systems, Incorporated.  All rights reserved.

#ifndef android_jni_support_jni_boxed_hpp
#define android_jni_support_jni_boxed_hpp

#include <jni.h>
#include <assert.h>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_references.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// Javaboxed:
// convert a primitive type into a java boxed type (Boolean, Integer, etc.)
// ----------------------------------------------------------------------------
template <typename T>
JNIAutoUnref<jobject> java_boxed(T value);

template <typename R>
R java_boxed_to_native(jobject obj);

// retrieve the ordinal value of a java enum
int java_enum_ordinal(jobject obj);

// ----------------------------------------------------------------------------
// Return the enum value from the given java enum
// ----------------------------------------------------------------------------
template <typename T>
T java_enum_to_native(jobject obj) {
    assert(obj && "null jobject!");
    auto env = JNI::env();
    auto ordinal = java_enum_ordinal(obj);
    return static_cast<T>(ordinal);
}

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_boxed_hpp
