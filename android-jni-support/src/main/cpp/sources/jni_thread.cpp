//
// Created by Alessandro Crugnola on 8/27/18.
//


#include <android-jni-support/jni_env.hpp>
#include <android-jni-support/jni_thread.hpp>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_bindings.hpp>
#include <android-jni-support/jni_config.hpp>

#include <sstream>
#include <map>
#include <unistd.h>

#include <android/looper.h>
//#include <dispatch/dispatch.h>


USING_NAMESPACE_ANDROID_JNI

static std::thread::id ui_thread_;
static std::map<std::thread::id, std::string> thread_names;

static pthread_key_t key;
static pthread_once_t tls_init_flag = PTHREAD_ONCE_INIT;

static void _destructor(void *t) {
    delete (AutoDetachThread *) t;
}

static void _create_key() {
    pthread_key_create(&key, _destructor);
}

// ----------------------------------------------------------------------------
// JNIThread::set_ui_thread:
// ----------------------------------------------------------------------------
void JNIThread::set_ui_thread(std::thread::id id) {
    ui_thread_ = id;

}

// ----------------------------------------------------------------------------
// JNIThread::ui_thread:
// ----------------------------------------------------------------------------
std::thread::id JNIThread::ui_thread() { return ui_thread_; }

// ----------------------------------------------------------------------------
// JNIThread::tid:
// ----------------------------------------------------------------------------
std::string JNIThread::tid(std::thread::id id) {
    std::ostringstream oss;
    oss << id;
    return oss.str();
}

// ----------------------------------------------------------------------------
// JNIThread::tid:
// ----------------------------------------------------------------------------
std::string JNIThread::tid() {
    if (thread_names.find(std::this_thread::get_id()) != thread_names.end()) {
        return thread_names[std::this_thread::get_id()];
    }
    return tid(std::this_thread::get_id());
}

// ----------------------------------------------------------------------------
// JNIThread::is_ui_thread:
// ----------------------------------------------------------------------------
bool JNIThread::is_ui_thread() { return ui_thread_ == std::this_thread::get_id(); }

// ----------------------------------------------------------------------------
// JNIThread::is_same_thread:
// ----------------------------------------------------------------------------
bool JNIThread::is_same_thread(std::thread::id id) { return std::this_thread::get_id() == id; }

// ----------------------------------------------------------------------------
// JNIThread::attach_if_required:
// ----------------------------------------------------------------------------
JNIEnv *JNIThread::attach_if_required(std::thread::id id) {
    if (!JNIThread::is_same_thread(id)) {
        return JNIThread::attach();
    } else {
        return JNI::env();
    }
}

// ----------------------------------------------------------------------------
// JNIThread::detach_if_required:
// ----------------------------------------------------------------------------
void JNIThread::detach_if_required(std::thread::id id) {
    if (!JNIThread::is_same_thread(id)) {
        JNIThread::detach();
    }
}

// ----------------------------------------------------------------------------
// JNIThread::set_name:
// ----------------------------------------------------------------------------
void JNIThread::set_name(const char *name) {
    thread_names[std::this_thread::get_id()] = std::string(name);
}

// ----------------------------------------------------------------------------
// JNIThread::attach:
// ----------------------------------------------------------------------------
JNIEnv *JNIThread::attach() { return JNI::attach_current_thread(); }

// ----------------------------------------------------------------------------
// JNIThread::detach:
// ----------------------------------------------------------------------------
jint JNIThread::detach() { return JNI::detach_current_thread(); }

// ----------------------------------------------------------------------------
// AutoDetachThread:ctor default
// ----------------------------------------------------------------------------
AutoDetachThread::AutoDetachThread() {
    env_ = JNIThread::attach();
    attached_ = true;
}

// ----------------------------------------------------------------------------
// AutoDetachThread:ctor
// ----------------------------------------------------------------------------
AutoDetachThread::AutoDetachThread(std::thread::id id) {
    attached_ = !JNIThread::is_same_thread(id);
    if (attached_)
        env_ = JNIThread::attach();
    else
        env_ = JNI::env();
}

// ----------------------------------------------------------------------------
// AutoDetachThread:dtor
// ----------------------------------------------------------------------------
AutoDetachThread::~AutoDetachThread() {
    if (attached_) {
        JNIThread::detach();
    }
}

// ----------------------------------------------------------------------------
// DispatchQueueUtils:set_up_main_looper
// ----------------------------------------------------------------------------
std::shared_ptr<DispatchQueueUtils> &DispatchQueueUtils::instance() {
    static std::shared_ptr<DispatchQueueUtils> s = std::make_shared<DispatchQueueUtils>();
    return s;
}

// ----------------------------------------------------------------------------
// DispatchQueueUtils:set_up_main_looper
// ----------------------------------------------------------------------------
void DispatchQueueUtils::set_up_main_looper(std::function<void(void)> main_queue) {
    // Get main looper
    this->main_queue = main_queue;
    ALooper *mainLooper = ALooper_forThread();
    ALooper_acquire(mainLooper);

    // Create file descriptors
    pipe(fileDescriptors);

    // Associate file descriptor with the looper and specify the callback
    ALooper_addFd(mainLooper, fileDescriptors[0],
                  0, ALOOPER_EVENT_INPUT, invoke_tasks_on_main_queue, nullptr);
}

// ----------------------------------------------------------------------------
// DispatchQueueUtils:dispatch_main_queue
// ----------------------------------------------------------------------------
void DispatchQueueUtils::dispatch_main_queue() {
    // Write some thing so as to invoke ALOOPER_EVENT_INPUT
    write(fileDescriptors[1], "", 1);
}

// ----------------------------------------------------------------------------
// DispatchQueueUtils:drain_main_queue
// ----------------------------------------------------------------------------
void DispatchQueueUtils::drain_main_queue() {
    if (main_queue)
        main_queue();
}

// ----------------------------------------------------------------------------
// DispatchQueueUtils:drain_main_queue
// ----------------------------------------------------------------------------
int DispatchQueueUtils::invoke_tasks_on_main_queue(int fd, int events, void *data) {
    // read message from pipe
    char msg;
    read(fd, &msg, 1);

    DispatchQueueUtils::instance()->drain_main_queue();

    return 1; // continue listening for events
}
