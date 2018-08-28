//
// Created by Alessandro Crugnola on 8/27/18.
//

// (c) Copyright 2004-2007.  Adobe Systems, Incorporated.  All rights reserved.
// //photoshop/workarea/pie_android/photoshop/android/pie/include/JNIHelper.h#2

#include <android-jni-support/jni_helper.hpp>
#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_value_adapter.hpp>

USING_NAMESPACE_ANDROID_JNI

// ----------------------------------------------------------------------------
// std::map<std::string, std::string> -> HashMap<String, String>
// ----------------------------------------------------------------------------
jvalue JNIValueAdapter<std::map<std::string, std::string>>::toJNI(
    JNIEnv *env, std::map<std::string, std::string> map) {
    JNIAutoUnref<jobject> hashMap = JNIClass<kJavaHashMap>::instance().ctor(env);

    for (const auto &p : map) {
        JNIClass<kJavaHashMap>::instance().put(env, hashMap.obj(),
                                               env->NewStringUTF(p.first.c_str()),
                                               env->NewStringUTF(p.second.c_str()));
    }

    jvalue r;
    r.l = hashMap.release();
    return r;
}
