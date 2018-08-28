//
// Created by Alessandro Crugnola on 8/27/18.
//



#include <android-jni-support/jni_shared_preferences.hpp>
#include <android-jni-support/jni_logging.hpp>

USING_NAMESPACE_ANDROID_JNI

// ----------------------------------------------------------------------------
// JNISharedPreferences::get_int:
// ----------------------------------------------------------------------------
void JNISharedPreferences::get_int(const char *key, int &value, int defaultValue) {
    if (prefs_.obj() && prefs_.valid()) {
        int newValue =
            prefs_.getClass().getInt(JNI::env(), prefs_.obj(), std::string(key), defaultValue);
        value = newValue;
    } else {
        LOGE("invalid SharedPreferences reference");
        value = defaultValue;
    }
}

// ----------------------------------------------------------------------------
// JNISharedPreferences::get_string:
// ----------------------------------------------------------------------------
void JNISharedPreferences::get_string(const char *key,
                                      std::string &value,
                                      const std::string &defaultStr) {
    LOGW("Non implemented yet");
    value = defaultStr;
}