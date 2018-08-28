

#ifndef android_jni_support_jni_locale_hpp
#define android_jni_support_jni_locale_hpp

#include <jni.h>
#include <string>
#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNILocale {
private:
  std::string _language;
  std::string _country;

public:
  JNILocale() {}

  JNILocale(const char *lang, const char *country)
      : _language(lang), _country(country) {}

  void set_language(const char *lang) { _language = lang; }

  void set_country(const char *country) { _country = country; }

  const std::string &language() const { return _language; }
  const std::string &country() const { return _country; }

  // returns the device default locale
  static JNILocale &default_locale();
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif //android_jni_support_jni_locale_hpp