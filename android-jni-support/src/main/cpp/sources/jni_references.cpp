//
// Created by Alessandro Crugnola on 8/27/18.
//

#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_logging.hpp>
#include <android-jni-support/jni_env.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

int JNIGlobalRefBase::active_count_ = 0;
int JNIAutoUnrefBase::active_count_ = 0;
int JNIWeakRefBase::active_count_ = 0;
int JNILocalRefBase::active_count_ = 0;

// ----------------------------------------------------------------------------
// JNIGlobal
// ----------------------------------------------------------------------------
template<>
int JNIGlobal<jobject>::ref_count_ = 0;

template<>
jobject JNIGlobal<jobject>::release() {
    auto obj = obj_;
    obj_ = nullptr;
    return obj;
}

template<>
jobjectRefType JNIGlobal<jobject>::type() {
    jobjectRefType type = JNIInvalidRefType;
    if (obj_) {
        auto env = JNI::env();
        try {
            type = env->GetObjectRefType(obj_);
        } catch (...) {
            type = JNIInvalidRefType;
        }
    }
    return type;
}

template<>
void JNIGlobal<jobject>::set(jobject obj) {
    auto env = JNI::env();
    if (obj_) {
        delete_reference(env, obj_);
    }
    obj_ = obj ? new_reference(env, obj) : nullptr;
    assert(!obj || env->IsSameObject(obj_, obj));
}

template<>
void JNIGlobal<jobject>::reset() {
    if (obj_) {
        auto env = JNI::env();
        delete_reference(env, obj_);
        obj_ = nullptr;
    }
}

// ----------------------------------------------------------------------------
// JNIGlobalRefBase
// ----------------------------------------------------------------------------

// explicit ctor
JNIGlobalRefBase::JNIGlobalRefBase(jobject obj) : JNIGlobal<jobject>() { set(obj); }

// copy ctor
JNIGlobalRefBase::JNIGlobalRefBase(const JNIGlobalRefBase &other) : JNIGlobal<jobject>() {
    set(other.obj_);
}

jobject JNIGlobalRefBase::new_reference(JNIEnv *env, jobject origin) {
    JNIGlobalRefBase::active_count_++;
    return env->NewGlobalRef(origin);
}

void JNIGlobalRefBase::delete_reference(JNIEnv *env, jobject origin) {
    JNIGlobalRefBase::active_count_--;
    env->DeleteGlobalRef(origin);
}

// move ctor
JNIGlobalRefBase::JNIGlobalRefBase(JNIGlobalRefBase &&other) : JNIGlobal<jobject>() {
    auto env = JNI::env();
    obj_ = other.release();
    assert(!ther.obj_ || env->IsSameObject(obj_, other.obj_));
}

// copy assignment
JNIGlobalRefBase &JNIGlobalRefBase::operator=(const JNIGlobalRefBase &ref) {
    if (this != &ref) {
        reset();
        set(ref.obj_);
    }
    return *this;
}

// move assignment
JNIGlobalRefBase &JNIGlobalRefBase::operator=(JNIGlobalRefBase &&ref) {
    reset();
    obj_ = ref.release();
    return *this;
}

// valid
bool JNIGlobalRefBase::valid() { return type() == JNIGlobalRefType; }

// destructor
JNIGlobalRefBase::~JNIGlobalRefBase() { reset(); }

// ----------------------------------------------------------------------------
// JNILocalRefBase
// ----------------------------------------------------------------------------
JNILocalRefBase::JNILocalRefBase(jobject obj) : JNIGlobal<jobject>() { set(obj); }

jobject JNILocalRefBase::new_reference(JNIEnv *env, jobject origin) {
    JNILocalRefBase::active_count_++;
    return env->NewLocalRef(origin);
}

void JNILocalRefBase::delete_reference(JNIEnv *env, jobject origin) {
    JNILocalRefBase::active_count_--;
    env->DeleteLocalRef(origin);
}

// copy ctor
JNILocalRefBase::JNILocalRefBase(const JNILocalRefBase &other) : JNIGlobal<jobject>() {
    set(other.obj_);
}

// move ctor
JNILocalRefBase::JNILocalRefBase(JNILocalRefBase &&other) : JNIGlobal<jobject>() {
    auto env = JNI::env();
    obj_ = other.release();
    assert(!other.obj_ || env->IsSameObject(obj_, other.obj_));
}

// copy assignment
JNILocalRefBase &JNILocalRefBase::operator=(const JNILocalRefBase &ref) {
    if (this != &ref) {
        reset();
        set(ref.obj_);
    }
    return *this;
}

// move assignment
JNILocalRefBase &JNILocalRefBase::operator=(JNILocalRefBase &&ref) {
    reset();
    obj_ = ref.release();
    return *this;
}

// valid
bool JNILocalRefBase::valid() { return type() == JNILocalRefType; }

// destructor
JNILocalRefBase::~JNILocalRefBase() { reset(); }

// ----------------------------------------------------------------------------
// JNIWeakRefBase
// ----------------------------------------------------------------------------
JNIWeakRefBase::JNIWeakRefBase(jobject obj) : JNIGlobal<jweak>() { set(obj); }

JNIWeakRefBase::JNIWeakRefBase(const JNIWeakRefBase &other) : JNIGlobal<jweak>() {
    set(other.obj_);
}

jobject JNIWeakRefBase::new_reference(JNIEnv *env, jobject origin) {
    JNIWeakRefBase::active_count_++;
    return env->NewWeakGlobalRef(origin);
}

void JNIWeakRefBase::delete_reference(JNIEnv *env, jobject origin) {
    JNIWeakRefBase::active_count_--;
    env->DeleteWeakGlobalRef(origin);
}

JNIWeakRefBase::JNIWeakRefBase(JNIWeakRefBase &&other) : JNIGlobal<jweak>() {
    auto env = JNI::env();
    obj_ = other.release();
    assert(!other.obj_ || env->IsSameObject(obj_, other.obj_));
}

JNIWeakRefBase &JNIWeakRefBase::operator=(const JNIWeakRefBase &ref) {
    if (this != &ref) {
        auto env = JNI::env();
        reset();
        set(ref.obj_);
    }
    return *this;
}

JNIWeakRefBase &JNIWeakRefBase::operator=(JNIWeakRefBase &&ref) {
    reset();
    obj_ = ref.release();
    return *this;
}

// valid
bool JNIWeakRefBase::valid() { return type() == JNIWeakGlobalRefType; }

// destructor
JNIWeakRefBase::~JNIWeakRefBase() { reset(); }

// ----------------------------------------------------------------------------
// JNIAutoUnrefBase
// ----------------------------------------------------------------------------
JNIAutoUnrefBase::JNIAutoUnrefBase(jobject obj) : JNIGlobal<jobject>() {
    JNIAutoUnrefBase::active_count_++;
    obj_ = obj;
}

JNIAutoUnrefBase::JNIAutoUnrefBase(const JNIAutoUnrefBase &other) : JNIGlobal<jobject>() {
    // LOGI("JNIAutoUnrefBase copy ctor");
    set(other.obj_);
}

jobject JNIAutoUnrefBase::new_reference(JNIEnv *env, jobject origin) {
    JNIAutoUnrefBase::active_count_++;
    return env->NewLocalRef(origin);
}

void JNIAutoUnrefBase::delete_reference(JNIEnv *env, jobject origin) {
    JNIAutoUnrefBase::active_count_--;
    env->DeleteLocalRef(origin);
}

JNIAutoUnrefBase::JNIAutoUnrefBase(JNIAutoUnrefBase &&other) : JNIGlobal<jweak>() {
    auto env = JNI::env();
    obj_ = other.release();
    assert(!other.obj_ || env->IsSameObject(obj_, other.obj_));
}

JNIAutoUnrefBase &JNIAutoUnrefBase::operator=(const JNIAutoUnrefBase &ref) {
    if (this != &ref) {
        reset();
        set(ref.obj_);
    }
    return *this;
}

JNIAutoUnrefBase &JNIAutoUnrefBase::operator=(JNIAutoUnrefBase &&ref) {
    reset();
    obj_ = ref.release();
    return *this;
}

// valid
bool JNIAutoUnrefBase::valid() { return type() == JNILocalRefType; }

// destructor
JNIAutoUnrefBase::~JNIAutoUnrefBase() { reset(); }

// ----------------------------------------------------------------------------
// JNIStringAutoUnref
// ----------------------------------------------------------------------------
JNIStringAutoUnref::JNIStringAutoUnref(jstring string) {
    JNIStringAutoUnref(JNI::env(), string);
}

JNIStringAutoUnref::JNIStringAutoUnref(JNIEnv *env, jstring string) : source_(string) {
    if (string) {
        buffer_ = env->GetStringUTFChars(string, 0);
    } else {
        buffer_ = 0;
    }
}

JNIStringAutoUnref::JNIStringAutoUnref(const JNIStringAutoUnref &other) : source_(nullptr) {
    char *tmp = (char *) malloc(strlen(other.buffer_) + 1);
    strcpy(tmp, other.buffer_);
    buffer_ = tmp;
}

JNIStringAutoUnref::JNIStringAutoUnref(JNIStringAutoUnref &&other)
    : source_(other.source_), buffer_(other.buffer_) {
    other.release();
}

JNIStringAutoUnref &JNIStringAutoUnref::operator=(const JNIStringAutoUnref &o) {

    if (&o == this)
        return *this;
    reset();

    char *tmp = (char *) malloc(strlen(o.buffer_) + 1);
    strcpy(tmp, o.buffer_);
    buffer_ = tmp;

    return *this;
}

JNIStringAutoUnref &JNIStringAutoUnref::operator=(JNIStringAutoUnref &&o) {

    reset();

    buffer_ = o.buffer_;
    source_ = o.source_;

    o.release();

    return *this;
}

const char *JNIStringAutoUnref::c_str() const { return buffer_; }

std::string JNIStringAutoUnref::str() const { return std::string(buffer_); }

void JNIStringAutoUnref::release() {
    buffer_ = 0;
    source_ = 0;
}

void JNIStringAutoUnref::reset() {
    if (buffer_) {
        if (source_)
            JNI::env()->ReleaseStringUTFChars(source_, buffer_);
        else
            delete buffer_;
    }
    release();
}

JNIStringAutoUnref::~JNIStringAutoUnref() {
    reset();
}

void dumpJNIReferences() {
    LOGI("***************** JNI REFERENCES TABLE *****************");
    LOGD("JNIGlobalRefBase = %d", JNIGlobalRefBase::active_count());
    LOGD("JNILocalRefBase  = %d", JNILocalRefBase::active_count());
    LOGD("JNIWeakRefBase   = %d", JNIWeakRefBase::active_count());
    LOGD("JNIAutoUnrefBase = %d", JNIAutoUnrefBase::active_count());
    LOGI("********************************************************");
}

ANDROID_JNI_NAMESPACE_END_DECL
