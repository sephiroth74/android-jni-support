//
// Created by Alessandro Crugnola on 8/27/18.
//

#include <assert.h>

#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_context.hpp>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_io.hpp>

USING_NAMESPACE_ANDROID_JNI

static JNIGlobalRefBase classLoader_;
static JNIWeakRef<kAndroidContext> context_;

// ----------------------------------------------------------------------------
// JNIContext::set_context
// ----------------------------------------------------------------------------
void JNIContext::set_context(jobject context) { context_.set(context); }

// ----------------------------------------------------------------------------
// JNIContext::set_class_loader
// ----------------------------------------------------------------------------
void JNIContext::set_class_loader(jobject classloader) {
    classLoader_.reset();
    classLoader_.set(classloader);
}

// ----------------------------------------------------------------------------
// JNIContext::context
// ----------------------------------------------------------------------------
jweak JNIContext::context() { return context_.obj(); }

// ----------------------------------------------------------------------------
// JNIContext::class_loader:
// ----------------------------------------------------------------------------
jobject JNIContext::class_loader() { return classLoader_.obj(); }

// ----------------------------------------------------------------------------
// JNIContext::package_name:
// ----------------------------------------------------------------------------
std::string JNIContext::package_name() {
    static std::string pname;
    if (pname.empty()) {
        auto context = JNIContext::context();
        pname.assign(JNI_INSTANCE(kAndroidContext).getPackageName(JNI::env(), context));
    }
    return pname;
}

// ----------------------------------------------------------------------------
// JNIContext::shared_preferences:
// ----------------------------------------------------------------------------
JNISharedPreferences JNIContext::shared_preferences(const char *name) {
    assert(context_.obj());
    assert(context_.valid());

    jweak context = JNIContext::context();
    if (!context) {
        LOGE("invalid context!");
        JNISharedPreferences prefs(nullptr);
        return prefs;
    }

    JNIAutoUnref<jobject> preferences =
        JNI_INSTANCE(kAndroidContext).getSharedPreferences(JNI::env(), context, name, 0);

    auto pref_jni_object = preferences.release();
    auto type = JNI::env()->GetObjectRefType(pref_jni_object);
    JNISharedPreferences prefs(pref_jni_object);
    return prefs;
}

// ----------------------------------------------------------------------------
// JNIContext::get_asset_manager:
// ----------------------------------------------------------------------------
AAssetManager *JNIContext::get_asset_manager() {
    assert(context_.obj());
    assert(context_.valid());

    auto context = JNIContext::context();
    if (!context) {
        LOGE("invalid context!");
        return nullptr;
    }

    auto env = JNI::env();
    auto asset_manager =
        JNI_INSTANCE(kAndroidContext).getAssets(env, context);
    if (asset_manager.valid() && asset_manager.obj()) {
        return AAssetManager_fromJava(
            env, asset_manager.obj()); // TODO (crugnola) - remember to release the
        // underlying jobject
    }
    return nullptr;
}

// ----------------------------------------------------------------------------
// JNIContext::get_resources:
// ----------------------------------------------------------------------------
JNIAutoUnref<jobject> JNIContext::get_resources() {
    assert(context_.obj());
    assert(context_.valid());

    auto context = JNIContext::context();
    if (!context) {
        LOGE("invalid context!");
        return nullptr;
    }

    auto env = JNI::env();
    auto resources =
        JNI_INSTANCE(kAndroidContext).getResources(env, context);
    return resources;
}

// ----------------------------------------------------------------------------
// JNIContext::get_temp_dir:
// ----------------------------------------------------------------------------
std::string JNIContext::get_temp_dir(const char *name) {
    assert(name);

    std::string fs(JNIContext::get_cache_dir());
    fs.append("/tmp/");
    fs.append(name);

    LOGV("[get_temp_dir] fs: %s", fs.c_str());

    if (!filesystem::exists(fs)) {
        filesystem::mkdirs(fs);
    }

    if (filesystem::exists(fs) && filesystem::is_directory(fs)) {
        LOGV("[get_temp_dir] fs: %s", fs.c_str());
        return fs;
    }

    LOGE("failed to create tmp!");

    return "";
}

// ----------------------------------------------------------------------------
// JNIContext::get_external_cache_dirs:
// ----------------------------------------------------------------------------
std::vector<std::string> JNIContext::get_external_cache_dirs() {
    auto env = JNI::env();
    std::vector<JNIAutoUnref<jobject>> cache_dirs = JNI_INSTANCE(kAndroidContextCompat).static_getExternalCacheDirs(
        env, JNIContext::context());
    std::vector<std::string> v;
    for (JNIAutoUnref<jobject> dir : cache_dirs) {
        std::string path = JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
        v.push_back(path.c_str());
    }
    return v;
}

// ----------------------------------------------------------------------------
// JNIContext::get_external_files_dirs:
// ----------------------------------------------------------------------------
std::vector<std::string> JNIContext::get_external_files_dirs(const char *type) {
    assert(type);
    auto env = JNI::env();
    auto string_type = env->NewStringUTF(type);
    std::vector<JNIAutoUnref<jobject>>
        files_dirs = JNI_INSTANCE(kAndroidContextCompat).static_getExternalFilesDirs(env, JNIContext::context(), string_type);
    std::vector<std::string> v;
    for (JNIAutoUnref<jobject> dir : files_dirs) {
        std::string path = JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
        v.push_back(path.c_str());
    }
    env->DeleteLocalRef(string_type);
    return v;
}

// ----------------------------------------------------------------------------
// JNIContext::get_cache_dir:
// ----------------------------------------------------------------------------
std::string JNIContext::get_cache_dir() {
    auto env = JNI::env();
    auto dir = JNI_INSTANCE(kAndroidContext).getCacheDir(env, JNIContext::context());
    return JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
}

// ----------------------------------------------------------------------------
// JNIContext::get_files_dir:
// ----------------------------------------------------------------------------
std::string JNIContext::get_files_dir() {
    auto env = JNI::env();
    auto dir = JNI_INSTANCE(kAndroidContext).getFilesDir(env, JNIContext::context());
    return JNI_INSTANCE(kJavaFile).getAbsolutePath(env, dir.obj());
}

