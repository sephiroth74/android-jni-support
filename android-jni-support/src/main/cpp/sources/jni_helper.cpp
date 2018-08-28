//
// Created by Alessandro Crugnola on 8/27/18.
//

// (c) Copyright 2004-2007.  Adobe Systems, Incorporated.  All rights reserved.
// //photoshop/workarea/pie_android/photoshop/android/pie/include/JNIHelper.h#2

#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_context.hpp>
#include <android-jni-support/jni_helper.hpp>
//#include <android-jni-support/jni_conversions.hpp>
#include <android-jni-support/jni_logging.hpp>

USING_NAMESPACE_ANDROID_JNI

jmethodID JNIClassBase::findClass_ = nullptr;


// ----------------------------------------------------------------------------
// jstringToString:
// ----------------------------------------------------------------------------
std::string ANDROID_JNI_NS(jstringToString)(JNIEnv* env, jstring obj) {
    if (!obj) {
        return std::string();
    }

    auto chars = env->GetStringUTFChars(obj, nullptr);
    if (!chars) {
        return std::string();
    }

    std::string r(chars);
    env->ReleaseStringUTFChars(obj, chars);
    return r;
}

// ----------------------------------------------------------------------------
// jstringArrayToVector:
// ----------------------------------------------------------------------------
std::vector<std::string> ANDROID_JNI_NS(jstringArrayToVector)(JNIEnv *env, jobjectArray stringArray) {
   std::vector<std::string> v;

   if(!stringArray) {
       return v;
   }

   int stringCount = env->GetArrayLength(stringArray);
   if(stringCount) {
       for (int i=0; i < stringCount; i++) {
           jstring javaString = (jstring) (env->GetObjectArrayElement(stringArray, i));
           v.push_back(jstringToString(env, javaString));
       }
   }
   return v;
}

// ----------------------------------------------------------------------------
// jobjectArrayToVector:
// ----------------------------------------------------------------------------
std::vector<JNIAutoUnref<jobject>> ANDROID_JNI_NS(jobjectArrayToVector)(JNIEnv *env, jobjectArray objectArray) {
   std::vector<JNIAutoUnref<jobject>> v;

   if(!objectArray) {
       return v;
   }

   int stringCount = env->GetArrayLength(objectArray);
   if(stringCount) {
       for (int i=0; i < stringCount; i++) {
           jobject javaObject = (jobject) (env->GetObjectArrayElement(objectArray, i));
           v.push_back(JNIAutoUnref<jobject>(javaObject));
       }
   }
   return v;
}


// ----------------------------------------------------------------------------
// JNIMethod::getMethodID
// ----------------------------------------------------------------------------
jmethodID JNIMethod::getMethodID(jclass clazz) {
    jmethodID id = cachedId_;
    if (!id) {
        auto env = JNI::env();
        id = env->GetMethodID(clazz, methodName_, methodSignature_);

        if (!id) {
            LOGE("Failed to find method: %s", methodName_);
        }
        assert(id);
        cachedId_ = id;
    }
    return id;
}

// ----------------------------------------------------------------------------
// JNIStaticMethod::getMethodID
// ----------------------------------------------------------------------------
jmethodID JNIStaticMethod::getMethodID(jclass clazz) {
    jmethodID id = cachedId_;
    if (!id) {
        auto env = JNI::env();
        id = env->GetStaticMethodID(clazz, methodName_, methodSignature_);

        if (!id) {
            LOGE("Failed to find method: %s", methodName_);
        }
        assert(id);
        cachedId_ = id;
    }
    return id;
}

// ----------------------------------------------------------------------------
// JNIField::getFieldID
// ----------------------------------------------------------------------------
jfieldID JNIField::getFieldID(jclass clazz) {
    jfieldID id = cachedId_;
    if (!id) {
        auto env = JNI::env();
        id = env->GetFieldID(clazz, fieldName_, fieldSignature_);

        if (!id) {
            LOGE("Failed to find field: %s", fieldName_);
        }

        assert(id);
        cachedId_ = id;
    }
    return id;
}

// ----------------------------------------------------------------------------
// JNIStaticField::getFieldID
// ----------------------------------------------------------------------------
jfieldID JNIStaticField::getFieldID(jclass clazz) {
    jfieldID id = cachedId_;
    if (!id) {
        auto env = JNI::env();
        id = env->GetStaticFieldID(clazz, fieldName_, fieldSignature_);

        if (!id) {
            LOGE("Failed to find field: %s", fieldName_);
        }

        assert(id);
        cachedId_ = id;
    }
    return id;
}

// ----------------------------------------------------------------------------
// JNIClassBase::ctor:
// ----------------------------------------------------------------------------
JNIClassBase::JNIClassBase(const char *className) {
    LOGV("JNIClassBase(%s)", className);
    JNIEnv *env = JNI::env();
    auto classLoader = JNIContext::class_loader();

    if (!JNIClassBase::findClass_) {
        auto classLoaderClass = env->GetObjectClass(classLoader);
        JNIClassBase::findClass_ = env->GetMethodID(
            classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    }

    auto strClassName = JNIAutoUnref<const char *>(JNI::env(), className);
    // jclass classObj = env->FindClass(className);
    jclass classObj =
        (jclass) env->CallObjectMethod(classLoader, JNIClassBase::findClass_, strClassName.obj());

    if (classObj) {
        class_ = (jclass) env->NewGlobalRef(classObj);
    } else {
        class_ = nullptr;
        LOGE("Class Not Found!");
    }

    env->DeleteLocalRef(classObj);
}

// ----------------------------------------------------------------------------
// JNIClassBase::dtor:
// ----------------------------------------------------------------------------
JNIClassBase::~JNIClassBase() {
    JNIEnv *env = JNI::env();
    if (env && class_) {
        env->DeleteGlobalRef((jobject) class_);
    }
}


