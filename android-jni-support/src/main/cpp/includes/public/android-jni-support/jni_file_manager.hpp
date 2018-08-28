
#ifndef android_jni_support_jni_file_manager_hpp
#define android_jni_support_jni_file_manager_hpp

#include <vector>
#include <string>

#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNIFileManager {
public:

  static std::vector<std::string> get_available_cache_dirs();

  static std::vector<std::string> get_available_files_dirs(const char *type);

  static std::vector<JNIAutoUnref < jobject>> filter_available_dirs(std::vector<JNIAutoUnref < jobject>>& input);

};

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_file_manager_hpp
