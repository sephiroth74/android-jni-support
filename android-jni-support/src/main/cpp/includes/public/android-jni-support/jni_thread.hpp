
#ifndef android_jni_support_jni_thread_hpp
#define android_jni_support_jni_thread_hpp

#include <thread>
#include <string>
#include <jni.h>
#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

class JNIThread {
public:
  static void set_ui_thread(std::thread::id id);

  static std::thread::id ui_thread();

  static std::string tid(std::thread::id id);

  static std::string tid();

  static bool is_ui_thread();

  static bool is_same_thread(std::thread::id id);

  static JNIEnv *attach_if_required(std::thread::id id);

  static void detach_if_required(std::thread::id id);

  static void set_name(const char *name);

  static JNIEnv *attach();

  static jint detach();
};

class AutoDetachThread {
private:
  bool attached_;
  JNIEnv *env_;

public:
  AutoDetachThread();

  AutoDetachThread(std::thread::id tid);

  AutoDetachThread(const AutoDetachThread &) = delete;

  AutoDetachThread &operator=(const AutoDetachThread &) = delete;

  JNIEnv *env() { return env_; }

  ~AutoDetachThread();

};

class DispatchQueueUtils : public std::enable_shared_from_this<DispatchQueueUtils> {
private:
  int fileDescriptors[2];
  bool initialized{false};
  std::function<void(void)> main_queue;
  static int invoke_tasks_on_main_queue(int fd, int events, void *data);
public:
  static std::shared_ptr<DispatchQueueUtils> &instance();

  void set_up_main_looper(std::function<void(void)> main_queue);
  void dispatch_main_queue();
  void drain_main_queue();
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_thread_hpp