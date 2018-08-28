#include <jni.h>
#include <android/log.h>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_context.hpp>
#include <android-jni-support/jni_thread.hpp>

#include <thread>

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


    // (MANDATORY-1) initialization of the vm
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

void runAllTests() {

}

extern "C" {

JNIEXPORT
void Java_it_sephiroth_androidjnisupport_GTestRunner_initialize(JNIEnv *env,
                                                                jclass clazz,
                                                                jobject context,
                                                                jobject class_loader) {

    // (MANDATORY-2) set the current class loader
    JNIContext::set_class_loader(class_loader);
    // (MANDATORY-3) set the application context
    JNIContext::set_context(context);

    JNIThread::set_name("android-ui-thread");


    // Prepare GTests
    TestContainer::context.set(context);
}

JNIEXPORT
void Java_it_sephiroth_androidjnisupport_GTestRunner_runTests(JNIEnv *env,
                                                              jobject clazz,
                                                              jobject observable) {

    JNIGlobalRef<jobject> observableRef{observable};

    LOGI("runTests...");
    JNIClass<kJavaObservable>::instance().setChanged(env, observable);

    new std::thread([_observable = JNIGlobalRef<jobject>(observableRef.obj())] {
      JNIThread::set_name("gtest-thread");
      AutoDetachThread auto_thread;

      sleep(2);

      int argc[1] = {1};
      char **argv = new char *[*argc + 1];
      argv[0] = const_cast<char *>("--gtest_also_run_disabled_tests");
      ::testing::InitGoogleTest(argc, argv);
      auto result = RUN_ALL_TESTS();
      LOGV("result: %d", result);

      JNI_INSTANCE(kJavaObservable).notifyObserversWithObject(JNI::env(), _observable.obj(), java_boxed(result).obj());
    });

}

} // extern C