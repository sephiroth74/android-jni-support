
#ifndef android_jni_support_jni_helper_hpp
#define android_jni_support_jni_helper_hpp

#include <jni.h>
#include <type_traits>

#include <android-jni-support/jni_binding_base.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class noncopyable {
protected:
  constexpr noncopyable() = default;
  ~noncopyable() = default;
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};

// ----------------------------------------------------------------------------
// JNIClassBase
// ----------------------------------------------------------------------------
class JNIClassBase : private noncopyable {
public:
  explicit JNIClassBase(const char *className);

  ~JNIClassBase();

  jclass clazz() const { return class_; }

protected:
  jclass class_;
  static jmethodID findClass_;
};

// ----------------------------------------------------------------------------
// JNIClass<T>
// ----------------------------------------------------------------------------
template<class Type, typename Traits = JNIClassTraits<Type>>
class JNIClass
    : public Traits, private noncopyable {
public:
  static JNIClass<Type> &instance() {
      static JNIClass<Type> instance;
      return instance;
  }

  JNIClass<Type>() {
      static_assert(std::is_base_of<JNIBindingBase, Type>::value,
                    "Type not derived from JNIBindingBase");
  }
};

// ----------------------------------------------------------------------------
// JNIMethod
// ----------------------------------------------------------------------------
class JNIMethod : private noncopyable {
public:
  JNIMethod(const char *methodName, const char *methodSignature)
      : methodName_(methodName), methodSignature_(methodSignature) {}

  virtual jmethodID getMethodID(jclass clazz);

protected:
  jmethodID cachedId_ = nullptr;
  const char *methodName_;
  const char *methodSignature_;
};

// ----------------------------------------------------------------------------
// JNIStaticMethod
// ----------------------------------------------------------------------------
class JNIStaticMethod : private JNIMethod {
public:
  JNIStaticMethod(const char *methodName, const char *methodSignature)
      : JNIMethod(methodName, methodSignature) {}

  virtual jmethodID getMethodID(jclass clazz);

protected:
};

// ----------------------------------------------------------------------------
// JNIField
// ----------------------------------------------------------------------------
class JNIField : private noncopyable {
public:
  JNIField(const char *fieldName, const char *filedType)
      : fieldName_(fieldName), fieldSignature_(filedType) {}

  virtual jfieldID getFieldID(jclass clazz);

protected:
  jfieldID cachedId_ = nullptr;
  const char *fieldName_;
  const char *fieldSignature_;
};

// ----------------------------------------------------------------------------
// JNIStaticField
// ----------------------------------------------------------------------------
class JNIStaticField : private JNIField {
public:
  JNIStaticField(const char *fieldName, const char *filedType)
      : JNIField(fieldName, filedType) {}

  virtual jfieldID getFieldID(jclass clazz);

protected:
};


#ifndef JNI_INSTANCE
#define JNI_INSTANCE(x) JNIClass<x>::instance()
#endif  // JNI_INSTANCE

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_helper_hpp