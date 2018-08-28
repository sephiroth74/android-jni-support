#ifndef app_android_unittest_hpp
#define app_android_unittest_hpp

#include <android-jni-support/jni_all.hpp>

USING_NAMESPACE_ANDROID_JNI

class TestContainer {
public:
  static JNIGlobalRef<jobject> context;
};

#endif