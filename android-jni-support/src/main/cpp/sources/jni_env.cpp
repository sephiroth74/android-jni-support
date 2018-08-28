//
// Created by Alessandro Crugnola on 8/27/18.
//

#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_exceptions.hpp>
#include <android/log.h>
#include <mutex>

#define LOG_TAG "jni"

USING_NAMESPACE_ANDROID_JNI

static JavaVM *vm_ = nullptr;

std::mutex mutex_;

#define THROW_IF_NOT_INITIALIZED \
    JNI_ASSERT(JNI::initialized(), LOG_TAG, "vm not set")

// ----------------------------------------------------------------------------
// JNI::initialized:
// ----------------------------------------------------------------------------
bool JNI::initialized() { return vm_ != nullptr; }

// ----------------------------------------------------------------------------
// JNI::set_vm:
// ----------------------------------------------------------------------------
void JNI::set_vm(JavaVM *vm) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "set_vm");
    std::lock_guard<std::mutex> lock(mutex_);
    vm_ = vm;
}

// ----------------------------------------------------------------------------
// JNI::vm:
// ----------------------------------------------------------------------------
JavaVM *JNI::vm() { return vm_; }

// ----------------------------------------------------------------------------
// JNI::env:
// ----------------------------------------------------------------------------
JNIEnv *JNI::env() {
    THROW_IF_NOT_INITIALIZED;
    JNIEnv *env = nullptr;
    jint result = vm_->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        result = vm_->AttachCurrentThread(&env, nullptr);
    }
    return env;
}

// ----------------------------------------------------------------------------
// JNI::attach_current_thread:
// ----------------------------------------------------------------------------
JNIEnv *JNI::attach_current_thread() {
    THROW_IF_NOT_INITIALIZED;
    JNIEnv *env;
    if (vm_->AttachCurrentThread(&env, nullptr) == JNI_OK) {
        return env;
    }
    return nullptr;
}

// ----------------------------------------------------------------------------
// JNI::detach_current_thread:
// ----------------------------------------------------------------------------
jint JNI::detach_current_thread() {
    THROW_IF_NOT_INITIALIZED;
    return vm_->DetachCurrentThread();
}

// ----------------------------------------------------------------------------
// JNI::HasException:
// ----------------------------------------------------------------------------
bool JNI::has_exception(JNIEnv *env) {
    THROW_IF_NOT_INITIALIZED;
    return env->ExceptionCheck() != JNI_FALSE;
}

// ----------------------------------------------------------------------------
// JNI::clear_exception:
// ----------------------------------------------------------------------------
bool JNI::clear_exception(JNIEnv *env) {
    THROW_IF_NOT_INITIALIZED;
    if (!JNI::has_exception(env))
        return false;
    env->ExceptionDescribe();
    env->ExceptionClear();
    return true;
}

// ----------------------------------------------------------------------------
// JNI::throw_exception:
// ----------------------------------------------------------------------------
jint JNI::throw_exception(const char *className, const char *message) {
    THROW_IF_NOT_INITIALIZED;
    jclass exClass;
    JNIEnv *env = JNI::env();

    exClass = env->FindClass(className);
    if (exClass == NULL) {
        // well we have a problem!
    }

    return env->ThrowNew(exClass, message);
}

// ----------------------------------------------------------------------------
// JNI::throw_exception:
// ----------------------------------------------------------------------------
template<>
jint JNI::throw_exception<java_exceptions_e::ioException>(const char *message) {
    return JNI::throw_exception("java/io/IOException", message);
}

// ----------------------------------------------------------------------------
// JNI::throw_exception:
// ----------------------------------------------------------------------------
template<>
jint JNI::throw_exception<java_exceptions_e::runtimeException>(const char *message) {
    return JNI::throw_exception("java/lang/RuntimeException", message);
}

