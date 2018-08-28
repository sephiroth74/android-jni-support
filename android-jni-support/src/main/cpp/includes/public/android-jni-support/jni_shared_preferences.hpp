#ifndef android_jni_support_jni_shared_preferences_hpp
#define android_jni_support_jni_shared_preferences_hpp

#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_references.hpp>

#include <assert.h>
#include <string>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNISharedPreferences {
private:
  JNIWeakRef<kAndroidSharedPreferences> prefs_;

public:
  JNISharedPreferences(jobject prefs) {
      prefs_.set(prefs);
      assert(prefs_.valid());
  }

  ~JNISharedPreferences() { prefs_.reset(); }

  JNISharedPreferences(const JNISharedPreferences &other) {
      prefs_ = other.prefs_;
      assert(prefs_.valid());
  }

  JNISharedPreferences &operator=(const JNISharedPreferences &ref) {
      if (this != &ref) {
          prefs_.reset();
          prefs_ = ref.prefs_;
      }
      return *this;
  }

  void get_int(const char *key, int &value, int defaultValue);

  void get_string(const char *key, std::string &value, const std::string &defaultStr);
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_shared_preferences_hpp