//
// Created by Alessandro Crugnola on 8/27/18.
//


#include <android-jni-support/jni_exceptions.hpp>

USING_NAMESPACE_ANDROID_JNI

// ------------------------------------------------------------------
// JNIExceptionCheck::ctor:
// ------------------------------------------------------------------
JNIExceptionCheck::JNIExceptionCheck(JNIEnv *env) : env_(env) {}

// ------------------------------------------------------------------
// JNIExceptionCheck::dtor:
// ------------------------------------------------------------------
JNIExceptionCheck::~JNIExceptionCheck() {
    if (env_->ExceptionCheck()) {
        env_->ExceptionDescribe();
        env_->ExceptionClear();
    }
}