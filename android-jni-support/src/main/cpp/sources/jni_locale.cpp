//
// Created by Alessandro Crugnola on 8/27/18.
//


#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_locale.hpp>
#include <android-jni-support/jni_bindings.hpp>

#include <mutex>

USING_NAMESPACE_ANDROID_JNI

// static icu::Locale g_default;
static JNILocale g_default;
static std::mutex mutex_;

// ----------------------------------------------------------------------------
// JNILocale::default_locale:
// ----------------------------------------------------------------------------
JNILocale &JNILocale::default_locale() {
    std::lock_guard<std::mutex> lock(mutex_);
    static bool initialized = false;
    if (!initialized) {
        JNI_ASSERT_COND(JNI::vm(), "jni", "vm not set");

        auto locale =
            JNI_INSTANCE(kJavaLocale).static_getDefault(JNI::env());
        auto language = JNI_INSTANCE(kJavaLocale).getLanguage(JNI::env(), locale.obj());
        auto country = JNI_INSTANCE(kJavaLocale).getCountry(JNI::env(), locale.obj());

        g_default.set_language(language.c_str());
        g_default.set_country(country.c_str());

        initialized = true;
    }
    return g_default;
}