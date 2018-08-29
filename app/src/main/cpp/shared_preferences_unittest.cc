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

TEST(TestSharedPreferences, Create) {
    LOGD("TestSharedPreferences::Create");
    EXPECT_TRUE(TestContainer::context.obj() != nullptr);

    auto env = JNI::env();
    auto context = TestContainer::context.obj();

    auto preferences = JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0);

    EXPECT_TRUE(preferences.obj() != nullptr);
}

TEST(TestSharedPreferences, ReadDefaults) {
    LOGD("TestSharedPreferences::ReadDefaults");
    EXPECT_TRUE(TestContainer::context.obj() != nullptr);

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    auto &prefInstance = JNIClass<kAndroidSharedPreferences>::instance();

    auto preferences = JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0);

    auto p_string = prefInstance.getString(env, preferences.obj(), "key-1", "this is a test string");
    EXPECT_EQ("this is a test string", p_string);

    EXPECT_TRUE(prefInstance.getBoolean(env, preferences.obj(), "key-2", true));
    EXPECT_EQ(1.5f, prefInstance.getFloat(env, preferences.obj(), "key-3", 1.5f));
    EXPECT_EQ(11, prefInstance.getInt(env, preferences.obj(), "key-4", 11));
    EXPECT_EQ(5545454L, prefInstance.getLong(env, preferences.obj(), "key-5", 5545454L));
}

TEST(TestSharedPreferences, Write) {
    LOGD("TestSharedPreferences::Write");
    EXPECT_TRUE(TestContainer::context.obj() != nullptr);

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    auto &prefInstance = JNIClass<kAndroidSharedPreferences>::instance();
    auto &editorInstance = JNIClass<kAndroidSharedPreferencesEditor>::instance();

    auto preferences = JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0);
    EXPECT_TRUE(preferences.obj() != nullptr);

    auto editor = prefInstance.edit(env, preferences.obj());
    EXPECT_TRUE(editor.obj() != nullptr);

    editorInstance.putBoolean(env, editor.obj(), "key-bool", true);
    editorInstance.putFloat(env, editor.obj(), "key-float", 15.5f);
    editorInstance.putInt(env, editor.obj(), "key-int", 12);
    editorInstance.putLong(env, editor.obj(), "key-long", 1234567890L);
    editorInstance.putString(env, editor.obj(), "key-string", "this is a test string");
    EXPECT_TRUE(editorInstance.commit(env, editor.obj()));

    EXPECT_TRUE(prefInstance.contains(env, preferences.obj(), "key-bool"));
    EXPECT_TRUE(prefInstance.contains(env, preferences.obj(), "key-float"));
    EXPECT_TRUE(prefInstance.contains(env, preferences.obj(), "key-int"));
    EXPECT_TRUE(prefInstance.contains(env, preferences.obj(), "key-long"));
    EXPECT_TRUE(prefInstance.contains(env, preferences.obj(), "key-string"));

    EXPECT_TRUE(prefInstance.getBoolean(env, preferences.obj(), "key-bool", false));
    EXPECT_EQ(15.5f, prefInstance.getFloat(env, preferences.obj(), "key-float", 0.f));
    EXPECT_EQ(12, prefInstance.getInt(env, preferences.obj(), "key-int", 0));
    EXPECT_EQ(1234567890L, prefInstance.getLong(env, preferences.obj(), "key-long", 0L));
    EXPECT_EQ("this is a test string", prefInstance.getString(env, preferences.obj(), "key-string", ""));
}

TEST(TestSharedPreferences, Clear) {
    LOGD("TestSharedPreferences::Clear");
    EXPECT_TRUE(TestContainer::context.obj() != nullptr);

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    auto &prefInstance = JNIClass<kAndroidSharedPreferences>::instance();
    auto &editorInstance = JNIClass<kAndroidSharedPreferencesEditor>::instance();

    auto preferences = JNI_INSTANCE(kAndroidContext).getSharedPreferences(env, context, "gtest", 0);
    EXPECT_TRUE(preferences.obj() != nullptr);

    auto editor = prefInstance.edit(env, preferences.obj());
    EXPECT_TRUE(editor.obj() != nullptr);

    editorInstance.clear(env, editor.obj());
    EXPECT_TRUE(editorInstance.commit(env, editor.obj()));

    EXPECT_FALSE(prefInstance.contains(env, preferences.obj(), "key-bool"));
    EXPECT_FALSE(prefInstance.contains(env, preferences.obj(), "key-float"));
    EXPECT_FALSE(prefInstance.contains(env, preferences.obj(), "key-int"));
    EXPECT_FALSE(prefInstance.contains(env, preferences.obj(), "key-long"));
    EXPECT_FALSE(prefInstance.contains(env, preferences.obj(), "key-string"));
}

}
