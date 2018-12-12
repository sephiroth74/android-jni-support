
#ifndef android_jni_support_jni_env_hpp
#define android_jni_support_jni_env_hpp

#include <jni.h>
#include <android-jni-support/jni_exceptions.hpp>
#include <android-jni-support/jni_config.hpp>
#include <android/log.h>

#define JNI_ASSERT_COND(_cond, tag, ...) if (!_cond) __android_log_assert("conditional", tag, __VA_ARGS__)

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNI {
private:
  static bool initialized();
  static jint throw_exception(const char *className, const char *message);

public:
  // save the JavaVM pointer for later use.
  // JavaVM is usually generated in the JNI_OnLoad initializer
  static void set_vm(JavaVM *vm);

  // returns the current JavaVM object
  static JavaVM *vm();

  // returns a valid JNIEnv pointer,
  // or nullptr if an error occurred
  static JNIEnv *env();

  // tries to attach to the current java thread
  // and return the correct JNIEnv object
  static JNIEnv *attach_current_thread();

  // release the current thread from the java thread
  static jint detach_current_thread();

  // check if the current task has an exception pending
  static bool has_exception(JNIEnv *env);

  // clear exception stack if pending
  static bool clear_exception(JNIEnv *env);

  // throw an exception to the java vm
  template<java_exceptions_e E>
  static jint throw_exception(const char *message);
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_env_hpp