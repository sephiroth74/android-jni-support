// (c) Copyright 2004-2007.  Adobe Systems, Incorporated.  All rights reserved.

#ifndef android_jni_support_jni_value_adapter_hpp
#define android_jni_support_jni_value_adapter_hpp

#include <functional>
#include <map>
#include <sstream>
#include <thread>
#include <vector>

#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

template<typename T>
struct JNIValueAdapter;

extern std::string jstringToString(JNIEnv *env, jstring str);

/*

This file uses the jvalue defined by the JNI.
typedef union jvalue {
    jboolean    z;
    jbyte       b;
    jchar       c;
    jshort      s;
    jint        i;
    jlong       j;
    jfloat      f;
    jdouble     d;
    jobject     l;
} jvalue;
*/

template<>
struct JNIValueAdapter<void> {
  static void toNative(JNIEnv *env, const jvalue &v) {}

  static jvalue toJNI(JNIEnv *env) {
      jvalue r;
      r.l = nullptr;
      return r;
  }
};

template<>
struct JNIValueAdapter<bool> {
  static bool toNative(JNIEnv *env, const jvalue &v) { return v.z; }

  static jvalue toJNI(JNIEnv *env, bool v) {
      jvalue r;
      r.z = v;
      return r;
  }
};

template<>
struct JNIValueAdapter<jvalue> {
  static jvalue toNative(JNIEnv *env, const jvalue &v) { return v; }

  static jvalue toJNI(JNIEnv *env, jvalue v) { return v; }
};

template<>
struct JNIValueAdapter<jobjectArray> {
  static jvalue toJNI(JNIEnv *env, jobjectArray v) {
      jvalue r;
      r.l = v;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::vector<int>> {
  static jvalue toJNI(JNIEnv *env, std::vector<int> v) {
      std::vector<jint> tmp(v.begin(), v.end());
      jint *outArray = &tmp[0];

      jintArray array = env->NewIntArray(tmp.size());
      env->SetIntArrayRegion(array, 0, tmp.size(), outArray);

      jvalue r;
      r.l = array;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::vector<uint32_t>> {
  static jvalue toJNI(JNIEnv *env, std::vector<uint32_t> v) {
      std::vector<jint> tmp(v.begin(), v.end());
      jint *outArray = &tmp[0];

      jintArray array = env->NewIntArray(tmp.size());
      env->SetIntArrayRegion(array, 0, tmp.size(), outArray);

      jvalue r;
      r.l = array;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::vector<float>> {
  static jvalue toJNI(JNIEnv *env, std::vector<float> v) {
      jfloat *outArray = &v[0];
      jfloatArray array = env->NewFloatArray(v.size());
      env->SetFloatArrayRegion(array, 0, v.size(), outArray);
      jvalue r;
      r.l = array;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::vector<double>> {
  static jvalue toJNI(JNIEnv *env, std::vector<double> v) {
      jdouble *outArray = &v[0];
      jdoubleArray array = env->NewDoubleArray(v.size());
      env->SetDoubleArrayRegion(array, 0, v.size(), outArray);
      jvalue r;
      r.l = array;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::thread::id> {
  static jvalue toJNI(JNIEnv *env, std::thread::id v) {
      std::stringstream ss;
      ss << v;
      long id = std::stol(ss.str());

      //    std::hash<std::thread::id> h;
      //    h(v);

      jvalue r;
      r.j = id;
      return r;
  }
};

template<>
struct JNIValueAdapter<std::map<std::string, std::string>> {
  static jvalue toJNI(JNIEnv *env, std::map<std::string, std::string> map);
};

#define JNI_VALUE_ADAPTER(numericType, fieldName)                   \
    template <>                                                     \
    struct JNIValueAdapter<numericType> {                           \
        static numericType toNative(JNIEnv* env, const jvalue& v) { \
            return static_cast<numericType>(v.fieldName);           \
        }                                                           \
        static jvalue toJNI(JNIEnv* env, numericType v) {           \
            jvalue r;                                               \
            r.fieldName = v;                                        \
            return r;                                               \
        }                                                           \
    };

JNI_VALUE_ADAPTER(jboolean, z);
JNI_VALUE_ADAPTER(jbyte, b);
JNI_VALUE_ADAPTER(jchar, c);
JNI_VALUE_ADAPTER(jshort, s);
JNI_VALUE_ADAPTER(jint, i);
JNI_VALUE_ADAPTER(long, j);

#if TARGET_CPU_32_BIT
JNI_VALUE_ADAPTER(jlong, j);
#endif

JNI_VALUE_ADAPTER(unsigned long, j);
JNI_VALUE_ADAPTER(unsigned int, i);
JNI_VALUE_ADAPTER(jfloat, f);
JNI_VALUE_ADAPTER(jdouble, d);
JNI_VALUE_ADAPTER(jobject, l);
JNI_VALUE_ADAPTER(jstring, l);

template<>
struct JNIValueAdapter<std::string> {
  static std::string toNative(JNIEnv *env, const jvalue &v) {
      return jstringToString(env, (jstring) v.l);
  }

  static jvalue toJNI(JNIEnv *env, const std::string &v) {
      jvalue r;
      r.l = env->NewStringUTF(v.c_str());
      return r;
  }
};

template<>
struct JNIValueAdapter<const char *> {
  static jvalue toJNI(JNIEnv *env, const char *str) {
      jvalue r;
      r.l = env->NewStringUTF(str);
      return r;
  }
};

template<>
struct JNIValueAdapter<decltype(nullptr)> {
  static jvalue toJNI(JNIEnv *env, void *) {
      jvalue r;
      r.l = nullptr;
      return r;
  }
};

template<typename ValueType>
ValueType toNative(JNIEnv *env, jvalue valueRef) {
    return JNIValueAdapter<ValueType>::toNative(env, valueRef);
}

template<typename ValueType>
jvalue toJNI(JNIEnv *env, ValueType value) {
    return JNIValueAdapter<ValueType>::toJNI(env, value);
}

ANDROID_JNI_NAMESPACE_END_DECL

#endif// android_jni_support_jni_value_adapter_hpp
