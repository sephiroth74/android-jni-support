//
// Created by Alessandro Crugnola on 8/27/18.
//

#include "gtest/gtest.h"
#include <android-jni-support/jni_all.hpp>
#include "android_unittest.hpp"

USING_NAMESPACE_ANDROID_JNI

namespace {

TEST(TestEnv, EnvNotNull) {
    LOGD("TestEnv::EnvNotNull");

    EXPECT_TRUE(JNI::vm() != nullptr);

    auto env = JNI::env();
    EXPECT_TRUE(env != nullptr);

    EXPECT_FALSE(JNI::has_exception(env));
}

//TEST(TestSharedPreferences, Create) {
//    LOGD("TestSharedPreferences::Create");
//    EXPECT_TRUE(TestContainer::context.obj() != nullptr);
//
//    auto env = JNI::env();
//    auto context = TestContainer::context.obj();
//
//    auto preferences = JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0);
//
//    EXPECT_TRUE(preferences.obj() != nullptr);
//
//}

TEST(TestSharedPreferences, ReadDefaults) {
    LOGD("TestSharedPreferences::ReadDefaults");
    EXPECT_TRUE(TestContainer::context.obj() != nullptr);

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    auto &prefInstance = JNIClass<kAndroidSharedPreferences>::instance();

    JNIGlobalRef<jobject> preferences(JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0).release());
//    auto p_string = prefInstance.getString(env, preferences.obj(), "key-1", "this is a test string");
//    EXPECT_EQ("this is a test string", p_string);
//
    EXPECT_TRUE(prefInstance.getBoolean(env, preferences.obj(), "key-2", true));
    EXPECT_EQ(1.5f, prefInstance.getFloat(env, preferences.obj(), "key-3", 1.5f));
    EXPECT_EQ(11, prefInstance.getInt(env, preferences.obj(), 11));
//    EXPECT_EQ(5545454L, prefInstance.getLong(env, preferences.obj(), 5545454L));

    preferences.reset();
}

}
