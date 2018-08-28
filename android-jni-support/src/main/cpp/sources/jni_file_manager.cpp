//
// Created by Alessandro Crugnola on 8/27/18.
//


#include <android-jni-support/jni_context.hpp>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_file_manager.hpp>
#include <android-jni-support/jni_io.hpp>
#include <android-jni-support/jni_logging.hpp>

#include <string>
#include <vector>

USING_NAMESPACE_ANDROID_JNI

// ----------------------------------------------------------------------------
// JNIFileManager::get_available_files_dirs:
// ----------------------------------------------------------------------------
std::vector<std::string>
JNIFileManager::get_available_files_dirs(const char *type) {
    assert(type);

    auto env = JNI::env();
    auto string_type = env->NewStringUTF(type);

    std::vector<std::string> result;
    std::vector<JNIAutoUnref<jobject>> files_dirs =
        JNI_INSTANCE(kAndroidContextCompat).static_getExternalFilesDirs(
            env, JNIContext::context(), string_type);

    std::vector<JNIAutoUnref<jobject>> valid = JNIFileManager::filter_available_dirs(files_dirs);

    for (JNIAutoUnref<jobject> dir : valid) {
        std::string path =
            JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
        result.push_back(path.c_str());
    }

    result.push_back(JNIContext::get_files_dir());

    env->DeleteLocalRef(string_type);

    return result;
}

// ----------------------------------------------------------------------------
// JNIFileManager::get_available_cache_dirs:
// ----------------------------------------------------------------------------
std::vector<std::string> JNIFileManager::get_available_cache_dirs() {
    auto env = JNI::env();
    std::vector<std::string> result;

    std::vector<JNIAutoUnref<jobject>> files_dirs =
        JNI_INSTANCE(kAndroidContextCompat).static_getExternalCacheDirs(
            env, JNIContext::context());

    std::vector<JNIAutoUnref<jobject>> valid = JNIFileManager::filter_available_dirs(files_dirs);

    for (JNIAutoUnref<jobject> dir : valid) {
        std::string path =
            JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
        result.push_back(path.c_str());
    }

    result.push_back(JNIContext::get_cache_dir());

    return result;
}

// ----------------------------------------------------------------------------
// JNIFileManager::filter_available_dirs:
// ----------------------------------------------------------------------------
std::vector<JNIAutoUnref<jobject>>
JNIFileManager::filter_available_dirs(std::vector<JNIAutoUnref<jobject>> &input) {
    std::vector<JNIAutoUnref<jobject>> result;
    auto env = JNI::env();
    auto it =
        std::copy_if(input.begin(), input.end(), std::back_inserter(result),
                     [env](JNIAutoUnref<jobject> object) {
                       auto state =
                           JNI_INSTANCE(kAndroidEnvironment)
                               .static_getExternalStorageState(env, object.obj());
                       return strcmp(state.c_str(), "mounted") == 0 ||
                           strcmp(state.c_str(), "checking") == 0;
                     });
    return result;
}
