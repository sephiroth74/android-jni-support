#ifndef android_jni_support_jni_conversions_hpp
#define android_jni_support_jni_conversions_hpp

#include <android-jni-support/jni_references.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

/**
 * auto convert a jstring to a std::string
 */
std::string jstringToString(JNIEnv* env, jstring str);

/**
 * Auto convert a java String[] into a std::vector<std::string>
 */
std::vector<std::string> jstringArrayToVector(JNIEnv* env, jobjectArray stringArray);

/**
 * Auto convert a java Object[] into a std::vector<JNIObjectAutoUnref>
 */
std::vector<JNIAutoUnref<jobject>> jobjectArrayToVector(JNIEnv* env, jobjectArray objectArray);

ANDROID_JNI_NAMESPACE_END_DECL


#endif// android_jni_support_jni_conversions_hpp