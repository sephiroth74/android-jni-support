#ifndef android_jni_support_jni_uuid_hpp

#include <android-jni-support/jni_env.hpp>
#include <string.h>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNIUUID {
public:
  static void generate_uuid(std::string &out);
};

ANDROID_JNI_NAMESPACE_END_DECL

#define android_jni_support_jni_uuid_hpp
#endif // android_jni_support_jni_uuid_hpp