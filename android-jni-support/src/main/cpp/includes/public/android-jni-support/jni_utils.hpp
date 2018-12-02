#ifndef android_jni_support_jni_utils_hpp

#include <android-jni-support/jni_env.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

#define JNI_ASSERT_NORETURN(condition, message)                                \
  if (!(condition)) {                                                          \
    LOGE(message);                                                             \
    JNI::throw_exception<java_exceptions_e::runtimeException>(message);        \
    return;                                                                    \
  }

#define JNI_ASSERT_RETURN(condition, message, returnType)                      \
  if (!(condition)) {                                                          \
    LOGE(message);                                                             \
    JNI::throw_exception<java_exceptions_e::runtimeException>(message);        \
    return returnType;                                                         \
  }

#define JNI_GET_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define JNI_ASSERT_MACRO_CHOOSER(...)                                          \
  JNI_GET_ARG(__VA_ARGS__, JNI_ASSERT_RETURN, JNI_ASSERT_NORETURN, )

#define JNI_ASSERT(...) JNI_ASSERT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

class JNIUtils {
public:
  static void generate_uuid(std::string &out) {
    // char uuid_str[37];
    // uuid_t uuid;
    // uuid_generate_time_safe(uuid);
    // uuid_unparse_lower(uuid, uuid_str);

    // out.assign(uuid_str);
    // uuid_clear(uuid);
  }
};

ANDROID_JNI_NAMESPACE_END_DECL

#define android_jni_support_jni_utils_hpp
#endif // android_jni_support_jni_utils_hpp