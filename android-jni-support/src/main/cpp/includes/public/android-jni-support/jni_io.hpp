#ifndef android_jni_support_jni_io_hpp
#define android_jni_support_jni_io_hpp

#include <errno.h>
#include <limits.h>
#include <string>
#include <sys/stat.h>

#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

namespace filesystem {

bool exists(const std::string &filename);

bool is_file(const std::string &filename);

bool is_directory(const std::string &filename);

int mkdirs(const std::string &filename);

} // namespace filesystem

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_io_hpp