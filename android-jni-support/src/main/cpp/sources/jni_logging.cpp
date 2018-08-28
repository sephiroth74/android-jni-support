
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_thread.hpp>
#include <stdio.h>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// AndroidLog:
// ----------------------------------------------------------------------------
void AndroidLog(int level, const char *category, const char *file,
                const char *line, const char *fmt, ...) {
    std::ostringstream ss;
    ss << "[tid=" << JNIThread::tid() << "] [" << file << ":"
       << line << "] " << fmt;
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(level, category, ss.str().c_str(), args);
    va_end(args);
}

ANDROID_JNI_NAMESPACE_END_DECL
