//
// Created by Alessandro Crugnola on 8/27/18.
//

#include "gtest/gtest.h"
#include <android-jni-support/jni_all.hpp>
#include "android_unittest.hpp"

USING_NAMESPACE_ANDROID_JNI

namespace {

class ReferencesTest : public ::testing::Test {
protected:
  JNIGlobalRef<jobject> globalRef;
  JNIWeakRef<jobject> weakRef;

  void SetUp() override {
      globalRef.set(JNI_INSTANCE(kAndroidRect).ctor(JNI::env(), 0, 0, 100, 100).obj());
  }

  void TearDown() override {}
};

TEST_F(ReferencesTest, TestGlobal) {
    LOGD("ReferencesTest::TestGlobal");
    EXPECT_TRUE(globalRef.obj() != nullptr);
    EXPECT_EQ(JNIGlobalRefType, globalRef.type());
}

TEST_F(ReferencesTest, TestLocal) {
    LOGD("ReferencesTest::TestLocal");

    JNILocalRef<jobject> localRef(globalRef.obj());

    EXPECT_EQ(JNILocalRefType, localRef.type());
    EXPECT_EQ(JNIGlobalRefType, globalRef.type());

    EXPECT_TRUE(localRef.obj() != nullptr);
    EXPECT_TRUE(globalRef.obj() != nullptr);
}

TEST_F(ReferencesTest, TestWeak) {
    LOGD("ReferencesTest::TestWeak");

    JNIWeakRef<jobject> localRef(globalRef.obj());

    EXPECT_EQ(JNIWeakGlobalRefType, localRef.type());
    EXPECT_EQ(JNIGlobalRefType, globalRef.type());

    EXPECT_TRUE(localRef.obj() != nullptr);
    EXPECT_TRUE(globalRef.obj() != nullptr);

    auto gRef = localRef.getGlobal();
    auto wRef = localRef.getLocal();

    EXPECT_TRUE(gRef.obj() != nullptr);
    EXPECT_TRUE(wRef.obj() != nullptr);
    EXPECT_TRUE(globalRef.obj() != nullptr);
    EXPECT_TRUE(localRef.obj() != nullptr);

    EXPECT_EQ(JNIWeakGlobalRefType, localRef.type());
    EXPECT_EQ(JNIGlobalRefType, globalRef.type());
    EXPECT_EQ(JNIGlobalRefType, gRef.type());
    EXPECT_EQ(JNILocalRefType, wRef.type());
}

TEST_F(ReferencesTest, TestRelease) {
    LOGD("ReferencesTest::TestRelease");

    JNILocalRef<jobject> localRef(globalRef.obj());
    JNILocalRef<jobject> localRef2(std::forward<JNILocalRef<jobject>>(localRef));

    EXPECT_TRUE(localRef.obj() == nullptr);
    EXPECT_TRUE(localRef2.obj() != nullptr);

    EXPECT_EQ(JNIInvalidRefType, localRef.type());
    EXPECT_EQ(JNILocalRefType, localRef2.type());

    auto obj = localRef2.release();
    EXPECT_TRUE(obj != nullptr);
    EXPECT_TRUE(localRef2.obj() == nullptr);
    EXPECT_EQ(JNIInvalidRefType, localRef2.type());

    JNI::env()->DeleteLocalRef(obj);
}

TEST_F(ReferencesTest, TestReset) {
    LOGD("ReferencesTest::TestReset");

    JNILocalRef<jobject> localRef(globalRef.obj());
    JNILocalRef<jobject> localRef2(std::forward<JNILocalRef<jobject>>(localRef));

    localRef2.reset();

    EXPECT_TRUE(localRef2.obj() == nullptr);
    EXPECT_EQ(JNIInvalidRefType, localRef2.type());
    EXPECT_EQ(JNIGlobalRefType, globalRef.type());
}


}