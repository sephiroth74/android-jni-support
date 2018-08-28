
//
// Created by Alessandro Crugnola on 8/27/18.
//


#ifndef android_jni_support_jni_context_hpp
#define android_jni_support_jni_context_hpp

#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>

#include <android-jni-support/jni_config.hpp>
#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_shared_preferences.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNIContext {
public:
  static void set_context(jobject context);

  static void set_class_loader(jobject classloader);

  static jweak context();

  static jobject class_loader();

  static std::string package_name();

  static JNISharedPreferences shared_preferences(const char *name);

  static AAssetManager *get_asset_manager();

  static JNIAutoUnref<jobject> get_resources();

  static std::string get_temp_dir(const char *name);

  static std::vector<std::string> get_external_cache_dirs();

  static std::vector<std::string> get_external_files_dirs(const char *type);

  static std::string get_cache_dir();

  static std::string get_files_dir();

};

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_context_hpp

