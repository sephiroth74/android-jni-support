//
// Created by Alessandro Crugnola on 8/27/18.
//

#include <stdint.h>
#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_boxed.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// Boolean:
// ----------------------------------------------------------------------------
template <>
JNIAutoUnref<jobject> java_boxed(bool value) {
    auto env = JNI::env();
    if (value) return JNI_INSTANCE(kJavaBoolean).getStaticField_TRUE(env);
    return JNI_INSTANCE(kJavaBoolean).getStaticField_FALSE(env);
}

template<>
bool java_boxed_to_native(jobject obj) {
    if(!obj) return false;
    auto env = JNI::env();
    return JNI_INSTANCE(kJavaBoolean).booleanValue(env, obj);
}

// ----------------------------------------------------------------------------
// Integer:
// ----------------------------------------------------------------------------
template <>
JNIAutoUnref<jobject> java_boxed(int value) {
    auto env = JNI::env();
    return JNI_INSTANCE(kJavaInteger).static_valueOf(env, value);
}

template <>
JNIAutoUnref<jobject> java_boxed(std::uint32_t value) {
    auto env = JNI::env();
    return JNI_INSTANCE(kJavaInteger).static_valueOf(env, value);
}

template<>
int java_boxed_to_native(jobject obj) {
    auto env = JNI::env();
    return JNI_INSTANCE(kJavaInteger).intValue(env, obj);
}


template<>
std::uint32_t java_boxed_to_native(jobject obj) {
    auto env = JNI::env();
    return (std::uint32_t)(JNI_INSTANCE(kJavaLong).longValue(env, obj));
}


// ----------------------------------------------------------------------------
// java_enum_ordinal
// From a java Enum returns the ordinal value
// ----------------------------------------------------------------------------
int java_enum_ordinal(jobject obj) {
    auto env = JNI::env();
    return JNI_INSTANCE(kJavaEnum).ordinal(env, obj);
}

ANDROID_JNI_NAMESPACE_END_DECL
