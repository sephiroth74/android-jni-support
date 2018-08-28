#include <jni.h>
#include <android/log.h>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_context.hpp>
#include <android-jni-support/jni_thread.hpp>

#include "gtest/gtest.h"
#include "android_unittest.hpp"

USING_NAMESPACE_ANDROID_JNI

JNIGlobalRef<jobject> TestContainer::context;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    JNIEnv *env = 0;

    __android_log_print(ANDROID_LOG_INFO, "native-lib", "JNI_OnLoad");

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "main", "ERROR: GetEnv failed");
        goto bail;
    }

    result = JNI_VERSION_1_6;


    // mandatory initialization
    JNI::set_vm(vm);
    // this must be called on the android UI thread
    JNIThread::set_ui_thread(std::this_thread::get_id());

    // optional
    DispatchQueueUtils::instance()->set_up_main_looper([]() {
      LOGD("draining main queue on main thread? %i ", JNIThread::is_ui_thread());
    });

    bail:
    return result;
}


extern "C" {

JNIEXPORT
void Java_it_sephiroth_android_1jni_1support_Library_initializeLibrary(JNIEnv *env,
                                                                       jclass clazz,
                                                                       jobject context,
                                                                       jobject class_loader) {

    __android_log_print(ANDROID_LOG_INFO, "native-lib", "initializeLibrary");
    JNIContext::set_class_loader(class_loader);
    JNIContext::set_context(context);
    JNIThread::set_name("android-ui-thread");


    // GTests

    TestContainer::context.set(context);

    int argc[1] = {1};
    char **argv = new char *[*argc + 1];
    argv[0] = "--gtest_also_run_disabled_tests";
    ::testing::InitGoogleTest(argc, argv);
    auto result = RUN_ALL_TESTS();

    LOGE("test result: %d", result);

}

} // extern C