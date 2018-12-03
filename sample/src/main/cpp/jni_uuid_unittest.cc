//
// Created by Alessandro Crugnola on 8/27/18.
//

#include "gtest/gtest.h"
#include <android-jni-support/jni_all.hpp>
#include "android_unittest.hpp"

USING_NAMESPACE_ANDROID_JNI

namespace {

TEST(JniUUIDTest, TestUUIDGeneration) {
    LOGD("JniUUIDTest::TestUUIDGeneration");

    auto env = JNI::env();
    EXPECT_TRUE(env != nullptr);

    std::string uuid;
    JNIUUID::generate_uuid(uuid);

    LOGV("uuid: %s, size: %lu", uuid.c_str(), uuid.size());

    EXPECT_TRUE(!uuid.empty());
    EXPECT_EQ(uuid.size(), 36);
}

}
