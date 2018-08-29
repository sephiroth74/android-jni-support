//
// Created by Alessandro Crugnola on 8/27/18.
//

#include "gtest/gtest.h"
#include <android-jni-support/jni_all.hpp>
#include "android_unittest.hpp"

USING_NAMESPACE_ANDROID_JNI

namespace {

TEST(ContextTest, CacheDirFromContext) {
    LOGD("ContextTest::CacheDirFromContext");

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    EXPECT_TRUE(context != nullptr);

    auto cacheDir = JNI_INSTANCE(kAndroidContext).getCacheDir(env, context);
    auto cachePath = JNI_INSTANCE(kJavaFile).getAbsolutePath(env, cacheDir.obj());

    EXPECT_TRUE(cachePath.c_str() != nullptr);
    EXPECT_TRUE(filesystem::is_directory(cachePath.c_str()));
}

TEST(ContextTest, JNIContext) {
    LOGD("ContextTest::JNIContext");

    auto env = JNI::env();
    auto context = TestContainer::context.obj();
    EXPECT_TRUE(context != nullptr);

    EXPECT_TRUE(filesystem::is_directory(JNIContext::get_cache_dir()));
    EXPECT_TRUE(filesystem::is_directory(JNIContext::get_files_dir()));
    EXPECT_TRUE(JNIContext::get_asset_manager() != nullptr);
    EXPECT_EQ(JNIContext::package_name(), JNI_INSTANCE(kAndroidContext).getPackageName(env, context));

    auto list = JNIContext::get_external_cache_dirs();
    EXPECT_TRUE(list.size() > 0);

    auto list2 = JNIContext::get_external_files_dirs(nullptr);
    EXPECT_TRUE(list2.size() > 0);

    EXPECT_TRUE(JNIContext::get_resources().obj() != nullptr);
}


}
