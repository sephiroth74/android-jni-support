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

}
