#ifndef android_jni_support_jni_macros_hpp
#define android_jni_support_jni_macros_hpp

#include <android-jni-support/jni_config.hpp>
#include <android-jni-support/jni_invoke_adapter.hpp>
#include <android-jni-support/jni_logging.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

#define JNI_CLASS_BEGIN_IMPL(localName, javaClassName, classBase)              \
  template <> struct JNIClassTraits<localName> : public classBase {            \
    static const char *className() { return javaClassName; }                   \
    JNIClassTraits() : classBase(className()) {}                               \
    explicit JNIClassTraits(const char *parentClassName)                       \
        : classBase(parentClassName) {}

#define JNI_CLASS_BEGIN(localName, javaClassName)                              \
  JNI_CLASS_BEGIN_IMPL(localName, javaClassName, JNIClassBase)

#define JNI_METHOD(methodName, methodSignature, returnType)                    \
  struct methodName##Sig : public JNIMethod {                                  \
    methodName##Sig() : JNIMethod(#methodName, methodSignature) {}             \
  };                                                                           \
  methodName##Sig methodName##Method;                                          \
  jmethodID methodName##Id() {                                                 \
    return methodName##Method.getMethodID(clazz());                            \
  }                                                                            \
  static const char *methodName##Name() {                                      \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string(#methodName);             \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  returnType methodName(JNIEnv *env, jobject obj, Args... args) {              \
    return JNIInvokeMethod<returnType>(env, methodName##Id(), obj, args...);   \
  }

#define JNI_METHOD2(methodName, realMethodName, methodSignature, returnType)   \
  struct methodName##Sig : public JNIMethod {                                  \
    methodName##Sig() : JNIMethod(#realMethodName, methodSignature) {}         \
  };                                                                           \
  methodName##Sig methodName##Method;                                          \
  jmethodID methodName##Id() {                                                 \
    return methodName##Method.getMethodID(clazz());                            \
  }                                                                            \
  static const char *methodName##Name() {                                      \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string(#methodName);             \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  returnType methodName(JNIEnv *env, jobject obj, Args... args) {              \
    return JNIInvokeMethod<returnType>(env, methodName##Id(), obj, args...);   \
  }

#define JNI_METHOD_NAME(methodName, cName, methodSignature, returnType)        \
  struct cName##Sig : public JNIMethod {                                       \
    cName##Sig() : JNIMethod(#methodName, methodSignature) {}                  \
  };                                                                           \
  cName##Sig cName##Method;                                                    \
  jmethodID cName##Id() { return cName##Method.getMethodID(clazz()); }         \
  static const char *cName##Name() {                                           \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string(#methodName);             \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  returnType cName(JNIEnv *env, jobject obj, Args... args) {                   \
    return JNIInvokeMethod<returnType>(env, cName##Id(), obj, args...);        \
  }

#define JNI_FIELD(fieldName, fieldType, returnType)                            \
  struct fieldName##Sig : public JNIField {                                    \
    fieldName##Sig() : JNIField(#fieldName, fieldType) {}                      \
  };                                                                           \
  fieldName##Sig fieldName##Field;                                             \
  jfieldID fieldName##Id() { return fieldName##Field.getFieldID(clazz()); }    \
  static const char *fieldName##Name() {                                       \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string(#fieldName);              \
    return name.c_str();                                                       \
  }                                                                            \
                                                                               \
  returnType getField_##fieldName(JNIEnv *env, jobject obj) {                  \
    return JNIFieldInvoker<returnType>::invoke_get(env, obj, fieldName##Id()); \
  }                                                                            \
  template <typename Arg>                                                      \
  void setField_##fieldName(JNIEnv *env, jobject obj, Arg arg) {               \
    return JNIFieldInvoker<returnType>::invoke_set(env, obj, fieldName##Id(),  \
                                                   arg);                       \
  }

#define JNI_STATIC_FIELD(fieldName, fieldType, returnType)                     \
  struct fieldName##StaticSig : public JNIStaticField {                        \
    fieldName##StaticSig() : JNIStaticField(#fieldName, fieldType) {}          \
  };                                                                           \
  fieldName##StaticSig static_##fieldName##Field;                              \
  jfieldID static_##fieldName##Id() {                                          \
    return static_##fieldName##Field.getFieldID(clazz());                      \
  }                                                                            \
  static const char *static_##fieldName##Name() {                              \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string("static_" #fieldName);    \
    return name.c_str();                                                       \
  }                                                                            \
                                                                               \
  returnType getStaticField_##fieldName(JNIEnv *env) {                         \
    return JNIFieldInvoker<returnType>::staticInvoke_get(                      \
        env, clazz(), static_##fieldName##Id());                               \
  }                                                                            \
  template <typename Arg>                                                      \
  void setStaticField_##fieldName(JNIEnv *env, jobject obj, Arg arg) {         \
    return JNIFieldInvoker<returnType>::staticInvoke_set(                      \
        env, clazz(), static_##fieldName##Id(), arg);                          \
  }

#define JNI_CTOR(ctorName, methodSignature)                                    \
  struct ctorName##Sig : public JNIMethod {                                    \
    ctorName##Sig() : JNIMethod("<init>", methodSignature) {}                  \
  };                                                                           \
  ctorName##Sig ctorName##Method;                                              \
  jmethodID ctorName##Id() { return ctorName##Method.getMethodID(clazz()); }   \
  static const char *ctorName##Name() {                                        \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string("<init>");                \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  JNIAutoUnref<jobject> ctorName(JNIEnv *env, Args... args) {                  \
    return JNIInvokeCtorMethod(env, ctorName##Id(), clazz(), args...);         \
  }

// Same as JNI_CTOR but instead of a JNIAutoUnref object it returns a simple
// jobject
#define JNI_CTOR_RAW(ctorName, methodSignature)                                \
  struct ctorName##Sig : public JNIMethod {                                    \
    ctorName##Sig() : JNIMethod("<init>", methodSignature) {}                  \
  };                                                                           \
  ctorName##Sig ctorName##Method;                                              \
  jmethodID ctorName##Id() { return ctorName##Method.getMethodID(clazz()); }   \
  static const char *ctorName##Name() {                                        \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string("<init>");                \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args> jobject ctorName(JNIEnv *env, Args... args) {    \
    return JNIInvokeCtorMethodSimple(env, ctorName##Id(), clazz(), args...);   \
  }

#define JNI_STATIC_METHOD(methodName, methodSignature, returnType)             \
  struct methodName##StaticSig : public JNIStaticMethod {                      \
    methodName##StaticSig() : JNIStaticMethod(#methodName, methodSignature) {} \
  };                                                                           \
  methodName##StaticSig static_##methodName##Method;                           \
  jmethodID static_##methodName##Id() {                                        \
    return static_##methodName##Method.getMethodID(clazz());                   \
  }                                                                            \
  static const char *static_##methodName##Name() {                             \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string("static_" #methodName);   \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  returnType static_##methodName(JNIEnv *env, Args... args) {                  \
    return JNIInvokeStaticMethod<returnType>(env, static_##methodName##Id(),   \
                                             clazz(), args...);                \
  }

#define JNI_STATIC_METHOD2(methodName, realMethodName, methodSignature,        \
                           returnType)                                         \
  struct methodName##StaticSig : public JNIMethod {                            \
    methodName##StaticSig() : JNIMethod(#realMethodName, methodSignature) {}   \
  };                                                                           \
  methodName##StaticSig static_##methodName##Method;                           \
  jmethodID static_##methodName##Id() {                                        \
    return static_##methodName##Method.getMethodID(clazz());                   \
  }                                                                            \
  static const char *static_##methodName##Name() {                             \
    static std::string name =                                                  \
        std::string(className()) + "." + std::string("static_" #methodName);   \
    return name.c_str();                                                       \
  }                                                                            \
  template <typename... Args>                                                  \
  returnType static_##methodName(JNIEnv *env, Args... args) {                  \
    return JNIInvokeStaticMethod<returnType>(env, static_##methodName##Id(),   \
                                             clazz(), args...);                \
  }

#define JNI_CLASS_END()                                                        \
  }                                                                            \
  ; // NOLINT


ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_macros_hpp
