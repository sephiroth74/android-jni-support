#ifndef android_jni_support_jni_invoke_adapter_hpp
#define android_jni_support_jni_invoke_adapter_hpp

#include <android-jni-support/jni_config.hpp>
#include <android-jni-support/jni_value_adapter.hpp>
#include <android-jni-support/jni_conversions.hpp>

#include <cstdlib>
#include <string>
#include <vector>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

template<typename ReturnType>
struct JNIInvoker {};

template<typename ReturnType>
struct JNIFieldInvoker {};

template<size_t index, typename T>
void JNIPushArguments(JNIEnv *env, std::vector<jvalue> *argsVector, T value) {
    (*argsVector)[index] = toJNI(env, value);
}

template<size_t index>
void JNIPushArguments(JNIEnv *env, std::vector<jvalue> *argsVector) {}

template<size_t index, typename T, typename... Args>
void JNIPushArguments(JNIEnv *env, std::vector<jvalue> *argsVector, T value, Args... args) {
    (*argsVector)[index] = toJNI(env, value);
    JNIPushArguments<index + 1>(env, argsVector, args...);
}


// JNIInvoker

template<>
struct JNIInvoker<void> {
  static void invoke(JNIEnv *env, jobject thisObject, jmethodID methodId) {
      return env->CallVoidMethod(thisObject, methodId);
  }

  static void invoke(JNIEnv *env, jobject thisObject, jmethodID methodId, jvalue *args) {
      return env->CallVoidMethodA(thisObject, methodId, args);
  }

  static void staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId) {
      return env->CallStaticVoidMethod(clazz, methodId);
  }

  static void staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId, jvalue *args) {
      return env->CallStaticVoidMethodA(clazz, methodId, args);
  }
};

template<>
struct JNIInvoker<std::string> {
  static std::string invoke(JNIEnv *env, jobject thisObject, jmethodID methodId) {
      return jstringToString(env, (jstring) env->CallObjectMethod(thisObject, methodId));
  }

  static std::string invoke(JNIEnv *env, jobject thisObject, jmethodID methodId, jvalue *args) {
      return jstringToString(env, (jstring) env->CallObjectMethodA(thisObject, methodId, args));
  }

  static std::string staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId) {
      return jstringToString(env, (jstring) env->CallStaticObjectMethod(clazz, methodId));
  }

  static std::string staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId, jvalue *args) {
      return jstringToString(env, (jstring) env->CallStaticObjectMethodA(clazz, methodId, args));
  }
};

template<>
struct JNIInvoker<std::vector<JNIAutoUnref<jobject>>> {
  static std::vector<JNIAutoUnref<jobject>> invoke(JNIEnv *env,
                                                   jobject thisObject,
                                                   jmethodID methodId) {
      return jobjectArrayToVector(env, (jobjectArray) env->CallObjectMethod(thisObject, methodId));
  }

  static std::vector<JNIAutoUnref<jobject>> invoke(JNIEnv *env,
                                                   jobject thisObject,
                                                   jmethodID methodId,
                                                   jvalue *args) {
      return jobjectArrayToVector(
          env, (jobjectArray) env->CallObjectMethodA(thisObject, methodId, args));
  }

  static std::vector<JNIAutoUnref<jobject>> staticInvoke(JNIEnv *env,
                                                         jclass clazz,
                                                         jmethodID methodId) {
      return jobjectArrayToVector(env,
                                  (jobjectArray) env->CallStaticObjectMethod(clazz, methodId));
  }

  static std::vector<JNIAutoUnref<jobject>> staticInvoke(JNIEnv *env,
                                                         jclass clazz,
                                                         jmethodID methodId,
                                                         jvalue *args) {
      return jobjectArrayToVector(
          env, (jobjectArray) env->CallStaticObjectMethodA(clazz, methodId, args));
  }
};

template<>
struct JNIInvoker<std::vector<std::string>> {
  static std::vector<std::string> invoke(JNIEnv *env, jobject thisObject, jmethodID methodId) {
      return jstringArrayToVector(env, (jobjectArray) env->CallObjectMethod(thisObject, methodId));
  }

  static std::vector<std::string> invoke(JNIEnv *env,
                                         jobject thisObject,
                                         jmethodID methodId,
                                         jvalue *args) {
      return jstringArrayToVector(
          env, (jobjectArray) env->CallObjectMethodA(thisObject, methodId, args));
  }

  static std::vector<std::string> staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId) {
      return jstringArrayToVector(env,
                                  (jobjectArray) env->CallStaticObjectMethod(clazz, methodId));
  }

  static std::vector<std::string> staticInvoke(JNIEnv *env,
                                               jclass clazz,
                                               jmethodID methodId,
                                               jvalue *args) {
      return jstringArrayToVector(
          env, (jobjectArray) env->CallStaticObjectMethodA(clazz, methodId, args));
  }
};

template<>
struct JNIInvoker<jobject> {
  static jobject invoke(JNIEnv *env, jobject thisObject, jmethodID methodId) {
      return ((env->CallObjectMethod(thisObject, methodId)));
  }

  static jobject invoke(JNIEnv *env, jobject thisObject, jmethodID methodId, jvalue *args) {
      return ((env->CallObjectMethodA(thisObject, methodId, args)));
  }

  static jobject staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId) {
      return ((env->CallStaticObjectMethod(clazz, methodId)));
  }

  static jobject staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId, jvalue *args) {
      return ((env->CallStaticObjectMethodA(clazz, methodId, args)));
  }
};

template<>
struct JNIInvoker<JNIAutoUnref<jobject>> {
  static JNIAutoUnref<jobject> invoke(JNIEnv *env, jobject thisObject, jmethodID methodId) {
      return JNIAutoUnref<jobject>((env->CallObjectMethod(thisObject, methodId)));
  }

  static JNIAutoUnref<jobject> invoke(JNIEnv *env,
                                      jobject thisObject,
                                      jmethodID methodId,
                                      jvalue *args) {
      return JNIAutoUnref<jobject>((env->CallObjectMethodA(thisObject, methodId, args)));
  }

  static JNIAutoUnref<jobject> staticInvoke(JNIEnv *env, jclass clazz, jmethodID methodId) {
      return JNIAutoUnref<jobject>((env->CallStaticObjectMethod(clazz, methodId)));
  }

  static JNIAutoUnref<jobject> staticInvoke(JNIEnv *env,
                                            jclass clazz,
                                            jmethodID methodId,
                                            jvalue *args) {
      return JNIAutoUnref<jobject>((env->CallStaticObjectMethodA(clazz, methodId, args)));
  }
};

// JNIFieldInvoker


template<>
struct JNIFieldInvoker<std::string> {
  static std::string invoke_get(JNIEnv *env, jobject thisObject, jfieldID fielId) {
      return jstringToString(env, (jstring) env->GetObjectField(thisObject, fielId));
  }
  static void invoke_set(JNIEnv *env, jobject thisObject, jfieldID fielId, std::string args) {
      env->SetObjectField(thisObject, fielId, env->NewStringUTF(args.c_str()));
  }

  static std::string staticInvoke_get(JNIEnv *env, jclass clazz, jfieldID fieldId) {
      return jstringToString(env, (jstring) env->GetStaticObjectField(clazz, fieldId));
  }
  static void staticInvoke_set(JNIEnv *env, jclass clazz, jfieldID fielId, jobject args) {
      env->SetStaticObjectField(clazz, fielId, args);
  }
};

template<>
struct JNIFieldInvoker<JNIAutoUnref<jobject>> {
  static JNIAutoUnref<jobject> invoke_get(JNIEnv *env, jobject thisObject, jfieldID fielId) {
      return JNIAutoUnref<jobject>(env->GetObjectField(thisObject, fielId));
  }
  static void invoke_set(JNIEnv *env, jobject thisObject, jfieldID fielId, jobject args) {
      env->SetObjectField(thisObject, fielId, args);
  }

  static JNIAutoUnref<jobject> staticInvoke_get(JNIEnv *env, jclass clazz, jfieldID fieldId) {
      return JNIAutoUnref<jobject>(env->GetStaticObjectField(clazz, fieldId));
  }
  static void staticInvoke_set(JNIEnv *env, jclass clazz, jfieldID fielId, jobject args) {
      env->SetStaticObjectField(clazz, fielId, args);
  }
};


// JNIInvokerMethod

template<typename ReturnType, typename... Args>
ReturnType JNIInvokeMethod(JNIEnv *env, jmethodID methodId, jobject thisObject, Args... args) {
    const size_t argumentCount = sizeof...(args);

    if (argumentCount) {
        std::vector<jvalue> argsVector(argumentCount);
        JNIPushArguments<0>(env, &argsVector, args...);
        return JNIInvoker<ReturnType>::invoke(env, thisObject, methodId, &argsVector[0]);
    }

    return JNIInvoker<ReturnType>::invoke(env, thisObject, methodId);
}

template<typename ReturnType, typename... Args>
ReturnType JNIInvokeStaticMethod(JNIEnv *env, jmethodID methodId, jclass clazz, Args... args) {
    const size_t argumentCount = sizeof...(args);

    if (argumentCount) {
        std::vector<jvalue> argsVector(argumentCount);
        JNIPushArguments<0>(env, &argsVector, args...);
        return JNIInvoker<ReturnType>::staticInvoke(env, clazz, methodId, &argsVector[0]);
    }

    return JNIInvoker<ReturnType>::staticInvoke(env, clazz, methodId);
}

// JNIInvokeCtorMethod
template<typename... Args>
JNIAutoUnref<jobject> JNIInvokeCtorMethod(JNIEnv *env,
                                          jmethodID methodId,
                                          jclass clazz,
                                          Args... args) {
    const size_t argumentCount = sizeof...(args);
    // JNIExceptionCheck guard(env);

    if (argumentCount) {
        std::vector<jvalue> argsVector(argumentCount);
        JNIPushArguments<0>(env, &argsVector, args...);
        return JNIAutoUnref<jobject>((env->NewObjectA(clazz, methodId, &argsVector[0])));
    }

    return JNIAutoUnref<jobject>((env->NewObject(clazz, methodId)));
}

// JNIInvokeCtorMethodSimple
template<typename... Args>
jobject JNIInvokeCtorMethodSimple(JNIEnv *env,
                                  jmethodID methodId,
                                  jclass clazz,
                                  Args... args) {
    const size_t argumentCount = sizeof...(args);

    if (argumentCount) {
        std::vector<jvalue> argsVector(argumentCount);
        JNIPushArguments<0>(env, &argsVector, args...);
        return ((env->NewObjectA(clazz, methodId, &argsVector[0])));
    }

    return ((env->NewObject(clazz, methodId)));
}

#define JNI_FIELD_INVOKER(ReturnType, FieldName)                                              \
    template <>                                                                               \
    struct JNIFieldInvoker<ReturnType> {                                                      \
        static ReturnType invoke_get(JNIEnv* env, jobject thisObject, jfieldID fieldId) {     \
            return static_cast<ReturnType>(env->Get##FieldName##Field(thisObject, fieldId));  \
        }                                                                                     \
        static void invoke_set(JNIEnv* env,                                                   \
                               jobject thisObject,                                            \
                               jfieldID fieldId,                                              \
                               ReturnType args) {                                             \
            env->Set##FieldName##Field(thisObject, fieldId, args);                            \
        }                                                                                     \
        static ReturnType staticInvoke_get(JNIEnv* env, jclass clazz, jfieldID fieldId) {     \
            return static_cast<ReturnType>(env->GetStatic##FieldName##Field(clazz, fieldId)); \
        }                                                                                     \
        static void staticInvoke_set(JNIEnv* env,                                             \
                                     jclass clazz,                                            \
                                     jfieldID fieldId,                                        \
                                     ReturnType args) {                                       \
            env->SetStatic##FieldName##Field(clazz, fieldId, args);                           \
        }                                                                                     \
    };

#define JNI_INVOKER(ReturnType, MethodName)                                                       \
    template <>                                                                                   \
    struct JNIInvoker<ReturnType> {                                                               \
        static ReturnType invoke(JNIEnv* env, jobject thisObject, jmethodID methodId) {           \
            return static_cast<ReturnType>(env->Call##MethodName##Method(thisObject, methodId));  \
        }                                                                                         \
        static ReturnType invoke(JNIEnv* env,                                                     \
                                 jobject thisObject,                                              \
                                 jmethodID methodId,                                              \
                                 jvalue* args) {                                                  \
            return static_cast<ReturnType>(                                                       \
                env->Call##MethodName##MethodA(thisObject, methodId, args));                      \
        }                                                                                         \
        static ReturnType staticInvoke(JNIEnv* env, jclass clazz, jmethodID methodId) {           \
            return static_cast<ReturnType>(env->CallStatic##MethodName##Method(clazz, methodId)); \
        }                                                                                         \
        static ReturnType staticInvoke(JNIEnv* env,                                               \
                                       jclass clazz,                                              \
                                       jmethodID methodId,                                        \
                                       jvalue* args) {                                            \
            return static_cast<ReturnType>(                                                       \
                env->CallStatic##MethodName##MethodA(clazz, methodId, args));                     \
        }                                                                                         \
    };

JNI_INVOKER(bool, Boolean)
JNI_INVOKER(jboolean, Boolean)
JNI_INVOKER(jint, Int)
JNI_INVOKER(jlong, Long)
JNI_INVOKER(jfloat, Float)

JNI_FIELD_INVOKER(jlong, Long)
JNI_FIELD_INVOKER(std::uint32_t, Long)
JNI_FIELD_INVOKER(std::int32_t, Int)
JNI_FIELD_INVOKER(bool, Boolean)
JNI_FIELD_INVOKER(jboolean, Boolean)
JNI_FIELD_INVOKER(jdouble, Double)
JNI_FIELD_INVOKER(jfloat, Float)

#if TARGET_CPU_32_BIT
JNI_INVOKER(long, Long)
JNI_FIELD_INVOKER(long, Long)
#endif

ANDROID_JNI_NAMESPACE_END_DECL

#endif// android_jni_support_jni_invoke_adapter_hpp