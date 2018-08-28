#ifndef android_jni_support_jni_logging_hpp
#define android_jni_support_jni_logging_hpp

#include <android-jni-support/jni_config.hpp>
#include <android/log.h>
#include <sstream>
#include <string.h>
#include <thread>

#ifndef _FILE_BASENAME_
#define _FILE_BASENAME_                                                        \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifndef _LINE_VAL
#define _LINE_VAL(x) #x
#endif

#ifndef _LINE_STR
#define _LINE_STR(x) _LINE_VAL(x)
#endif

ANDROID_JNI_NAMESPACE_BEGIN_DECL

void AndroidLog(int level,
                const char *category,
                const char *file,
                const char *line,
                const char *fmt, ...);

ANDROID_JNI_NAMESPACE_END_DECL

#if DEBUG

#if defined(ANDROID_DEBUG_EXTENDED)

#define LOGI(fmt, ...)                                                         \
  ANDROID_JNI_NS(AndroidLog)(ANDROID_LOG_INFO, "jni", _FILE_BASENAME_,         \
                             _LINE_STR(__LINE__), fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...)                                                         \
  ANDROID_JNI_NS(AndroidLog)(ANDROID_LOG_DEBUG, "jni", _FILE_BASENAME_,        \
                             _LINE_STR(__LINE__), fmt, ##__VA_ARGS__)
#define LOGV(fmt, ...)                                                         \
  ANDROID_JNI_NS(AndroidLog)(ANDROID_LOG_VERBOSE, "jni", _FILE_BASENAME_,      \
                             _LINE_STR(__LINE__), fmt, ##__VA_ARGS__)

#else

#define LOGI(fmt, ...)                                                         \
  __android_log_print(ANDROID_LOG_INFO, "jni", fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...)                                                         \
  __android_log_print(ANDROID_LOG_DEBUG, "jni", fmt, ##__VA_ARGS__)
#define LOGV(fmt, ...)                                                         \
  __android_log_print(ANDROID_LOG_VERBOSE, "jni", fmt, ##__VA_ARGS__)
#endif

#define LOGV_ONCE(label, fmt, ...)                                             \
  do {                                                                         \
    static bool label;                                                         \
    if (!label) {                                                              \
      LOGI(fmt, ##__VA_ARGS__);                                                \
      label = true;                                                            \
    }                                                                          \
  } while (false)

#else // DEBUG

#define LOGI(fmt, ...)
#define LOGD(fmt, ...)
#define LOGV(fmt, ...)
#define LOGV_ONCE(label, fmt, ...)

#endif // DEBUG

#define LOG_ASSERT(_cond, tag, ...)                                            \
  if (!_cond)                                                                  \
  __android_log_assert("conditional", tag, __VA_ARGS__)

#define LOGW(fmt, ...)                                                         \
  ANDROID_JNI_NS(AndroidLog)(ANDROID_LOG_WARN, "jni", _FILE_BASENAME_,         \
                             _LINE_STR(__LINE__), fmt, ##__VA_ARGS__)

#define LOGE(fmt, ...)                                                         \
  ANDROID_JNI_NS(AndroidLog)(ANDROID_LOG_ERROR, "jni", _FILE_BASENAME_,        \
                             _LINE_STR(__LINE__), fmt, ##__VA_ARGS__)

#endif // android_jni_support_jni_logging_hpp
