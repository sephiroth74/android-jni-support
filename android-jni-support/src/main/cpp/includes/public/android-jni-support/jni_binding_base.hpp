#ifndef android_jni_support_jni_binding_base_hpp
#define android_jni_support_jni_binding_base_hpp

#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// JNIBindingBase
// All jni binding declarations must inherit from this class
// ----------------------------------------------------------------------------
struct JNIBindingBase {};

// ----------------------------------------------------------------------------
// JNIClassTraits
// ----------------------------------------------------------------------------
template <typename Type = JNIBindingBase>
struct JNIClassTraits {};

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_binding_base_hpp