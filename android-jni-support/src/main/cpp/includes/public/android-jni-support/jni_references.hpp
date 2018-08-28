
#ifndef android_jni_support_jni_references_hpp
#define android_jni_support_jni_references_hpp

//#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_helper.hpp>
#include <android-jni-support/jni_binding_base.hpp>
#include <android-jni-support/jni_base_references.hpp>
#include <string>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// JNIGlobalRef<JNIBindingBase>:
// ----------------------------------------------------------------------------
template<typename Type = JNIBindingBase>
class JNIGlobalRef : public JNIGlobalRefBase {
public:
  JNIGlobalRef() : JNIGlobalRefBase() {}

  explicit JNIGlobalRef(jobject obj) : JNIGlobalRefBase(obj) {}

  JNIGlobalRef(const JNIGlobalRef &other) : JNIGlobalRefBase(other) {}

  JNIGlobalRef(JNIGlobalRef &&other) : JNIGlobalRefBase(std::move(other)) {}

  JNIGlobalRef &operator=(const JNIGlobalRef &o) {
      return static_cast<JNIGlobalRef &>(JNIGlobalRefBase::operator=(std::move(o)));
  }

  JNIGlobalRef &operator=(JNIGlobalRef &&o) {
      return static_cast<JNIGlobalRef &>(JNIGlobalRefBase::operator=(std::move(o)));
  }

  static JNIClass<Type> &getClass() { return JNIClass<Type>::instance(); }
};

// ----------------------------------------------------------------------------
// JNILocalRef<JNIBindingBase>:
// ----------------------------------------------------------------------------
template<typename Type = JNIBindingBase>
class JNILocalRef : public JNILocalRefBase {
public:
  JNILocalRef() : JNILocalRefBase() {}

  explicit JNILocalRef(jobject obj) : JNILocalRefBase(obj) {}

  JNILocalRef(const JNILocalRef &other) : JNILocalRefBase(other) {}

  JNILocalRef(JNILocalRef &&other) : JNILocalRefBase(std::move(other)) {}

  JNILocalRef &operator=(const JNILocalRef &o) {
      return static_cast<JNILocalRef &>(JNILocalRefBase::operator=(std::move(o)));
  }

  JNILocalRef &operator=(JNILocalRef &&o) {
      return static_cast<JNILocalRef &>(JNILocalRefBase::operator=(std::move(o)));
  }

  static JNIClass<Type> &getClass() { return JNIClass<Type>::instance(); }
};

// ----------------------------------------------------------------------------
// JNIWeakRef<JNIBindingBase>:
// ----------------------------------------------------------------------------
template<typename Type = JNIBindingBase>
class JNIWeakRef : public JNIWeakRefBase {
public:

  JNIWeakRef() : JNIWeakRefBase() {}

  explicit JNIWeakRef(jobject obj) : JNIWeakRefBase(obj) {}

  JNIWeakRef(const JNIWeakRef &other) : JNIWeakRefBase(other) {}

  JNIWeakRef(JNIWeakRef &&other) : JNIWeakRefBase(std::move(other)) {}

  JNIWeakRef &operator=(const JNIWeakRef &o) {
      return static_cast<JNIWeakRef &>(JNIWeakRefBase::operator=(std::move(o)));
  }

  JNIWeakRef &operator=(JNIWeakRef &&o) {
      return static_cast<JNIWeakRef &>(JNIWeakRefBase::operator=(std::move(o)));
  }

  static JNIClass<Type> &getClass() { return JNIClass<Type>::instance(); }

  JNILocalRef<Type> getLocal() const {
      JNILocalRef<Type> ref;
      ref.set(obj());
      return ref;
  }

  JNIGlobalRef<Type> getGlobal() const {
      JNIGlobalRef<Type> ref;
      ref.set(obj());
      return ref;
  }
};

// ----------------------------------------------------------------------------
// JNIAutoUnref:
// ----------------------------------------------------------------------------
template<typename T>
class JNIAutoUnref;

// ----------------------------------------------------------------------------
// JNIAutoUnref<jobject>:
// ----------------------------------------------------------------------------
template<>
class JNIAutoUnref<jobject> : public JNIAutoUnrefBase {
public:
  JNIAutoUnref(jobject obj) : JNIAutoUnrefBase(obj) {}
};

// ----------------------------------------------------------------------------
// JNIAutoUnref<std::string>:
// ----------------------------------------------------------------------------
template<>
class JNIAutoUnref<const std::string &> : public JNIAutoUnrefBase {
public:
  JNIAutoUnref(JNIEnv *env, const std::string &obj)
      : JNIAutoUnrefBase(env->NewStringUTF(obj.c_str())) {}
};

// ----------------------------------------------------------------------------
// JNIAutoUnref<const char*>:
// ----------------------------------------------------------------------------
template<>
class JNIAutoUnref<const char *> : public JNIAutoUnrefBase {
public:
  JNIAutoUnref(JNIEnv *env, const char *obj) : JNIAutoUnrefBase(env->NewStringUTF(obj)) {}
};

// ----------------------------------------------------------------------------
// JNIStringAutoUnref:
// ----------------------------------------------------------------------------
class JNIStringAutoUnref {
private:
  const char *buffer_;

  jstring source_;

  void release();

  JNIStringAutoUnref(const char *buffer, jstring source) : buffer_(buffer), source_(source) {}

public:
  JNIStringAutoUnref(jstring string);

  JNIStringAutoUnref(JNIEnv *env, jstring string);

  JNIStringAutoUnref(const JNIStringAutoUnref &other);

  JNIStringAutoUnref(JNIStringAutoUnref &&other);

  JNIStringAutoUnref &operator=(const JNIStringAutoUnref &o);

  JNIStringAutoUnref &operator=(JNIStringAutoUnref &&o);

  const char *c_str() const;

  void reset();

  ~JNIStringAutoUnref();
};

void dumpJNIReferences();

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_references_hpp