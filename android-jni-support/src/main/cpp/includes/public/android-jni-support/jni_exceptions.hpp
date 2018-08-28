// Copyright 2002 Adobe Systems Incorporated.  All rights reserved.

#ifndef android_jni_support_jni_exceptions_hpp
#define android_jni_support_jni_exceptions_hpp

#include <jni.h>
#include <string>
#include <vector>
#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

/** Enum used to throw java Exceptions */
enum java_exceptions_e {
  /** java.io.IoException */
      ioException,
  /** java.lang.RuntimeException */
      runtimeException,
};

//
// Automatically check for any java pending exceptions and clears them
// when the instance is removed
// 
class JNIExceptionCheck {
public:
  JNIExceptionCheck(JNIEnv *env);

  ~JNIExceptionCheck();

private:
  JNIEnv *env_;
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_exceptions_hpp
