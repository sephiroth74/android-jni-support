
#ifndef android_jni_support_jni_base_references_hpp
#define android_jni_support_jni_base_references_hpp

#include <jni.h>
#include <type_traits>
#include <android-jni-support/jni_config.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

// ----------------------------------------------------------------------------
// JNIGlobal<T>
// ----------------------------------------------------------------------------
template<typename T = jobject>
class JNIGlobal {
  static_assert(std::is_pointer<T>::value, "T must derive from jobject");

protected:
  T obj_;
  int id_;
  static int ref_count_;

public:
  JNIGlobal<T>() : id_(ref_count_++), obj_(nullptr) {}

  int id() { return id_; }

  T obj() const { return obj_; }

  // will delete the local/global/weak reference to the current obj_
  // and set to null
  void reset();

  // will remove the reference to the obj_ without removing
  // the local/global/weak reference
  T release();

  // will assign a new local/global/weak reference and remove
  // the current obj_ reference if any
  void set(T ref);

  // returns if the current obj_ is a valid jni reference
  virtual bool valid() = 0;

  // returns the jobject reference type
  jobjectRefType type();

  // create a new jni local/global/weak reference
  virtual T new_reference(JNIEnv *env, T origin) = 0;
//
  // delete the current jni reference
  virtual void delete_reference(JNIEnv *env, T origin) = 0;

  explicit operator bool() const { return obj_; }
};

// ----------------------------------------------------------------------------
// JNIGlobalRefBase
// ----------------------------------------------------------------------------
class JNIGlobalRefBase : public JNIGlobal<jobject> {
private:
  static int active_count_;
public:
  JNIGlobalRefBase() : JNIGlobal<jobject>() {}

  ~JNIGlobalRefBase();

  explicit JNIGlobalRefBase(jobject);

  JNIGlobalRefBase(const JNIGlobalRefBase &); // copy ctor

  JNIGlobalRefBase &operator=(const JNIGlobalRefBase &ref); // copy operator

  JNIGlobalRefBase(JNIGlobalRefBase &&); // move ctor

  JNIGlobalRefBase &operator=(JNIGlobalRefBase &&); // move operator

  jobject new_reference(JNIEnv *env, jobject origin) override;

  void delete_reference(JNIEnv *env, jobject origin) override;

  static int active_count() { return active_count_; }

  bool valid() override;
};

// ----------------------------------------------------------------------------
// JNILocalRefBase
// ----------------------------------------------------------------------------
class JNILocalRefBase : public JNIGlobal<jobject> {
private:
  static int active_count_;
public:
  JNILocalRefBase() : JNIGlobal<jobject>() {}

  ~JNILocalRefBase();

  explicit JNILocalRefBase(jobject);

  JNILocalRefBase(const JNILocalRefBase &); // copy ctor

  JNILocalRefBase &operator=(const JNILocalRefBase &ref); // copy operator

  JNILocalRefBase(JNILocalRefBase &&); // move ctor

  JNILocalRefBase &operator=(JNILocalRefBase &&ref); // move operator

  jobject new_reference(JNIEnv *env, jobject origin) override;

  void delete_reference(JNIEnv *env, jobject origin) override;

  static int active_count() { return active_count_; }

  bool valid() override;
};

// ----------------------------------------------------------------------------
// JNIWeakRefBase
// ----------------------------------------------------------------------------
class JNIWeakRefBase : public JNIGlobal<jweak> {
private:
  static int active_count_;

public:
  JNIWeakRefBase() : JNIGlobal<jweak>() {}

  ~JNIWeakRefBase();

  explicit JNIWeakRefBase(jobject);

  JNIWeakRefBase(const JNIWeakRefBase &); // copy ctor

  JNIWeakRefBase &operator=(const JNIWeakRefBase &ref); // copy operator

  JNIWeakRefBase(JNIWeakRefBase &&); // move ctor

  JNIWeakRefBase &operator=(JNIWeakRefBase &&ref); // move operator

  jobject new_reference(JNIEnv *env, jobject origin) override;

  void delete_reference(JNIEnv *env, jobject origin) override;

  static int active_count() { return active_count_; }

  bool valid() override;
};

// ----------------------------------------------------------------------------
// JNIAutoUnrefBase
//
// Unlike the others, a jobject passed to JNIAutoUnrefBase won't be referenced
// using NewLocalRef. The reference from the passed jobject is used instead.
// On destruction, the reference to the jobject will be deleted using
// DeleteLocalRef.
// ----------------------------------------------------------------------------
class JNIAutoUnrefBase : public JNIGlobal<jobject> {
private:
  static int active_count_;

public:
  JNIAutoUnrefBase() = delete;

  ~JNIAutoUnrefBase();

  explicit JNIAutoUnrefBase(jobject);

  JNIAutoUnrefBase(const JNIAutoUnrefBase &); // copy ctor

  JNIAutoUnrefBase &operator=(const JNIAutoUnrefBase &); // copy operator

  JNIAutoUnrefBase(JNIAutoUnrefBase &&other); // move ctor

  JNIAutoUnrefBase &operator=(JNIAutoUnrefBase &&other); // move operator

  jobject new_reference(JNIEnv *env, jobject origin) override;

  void delete_reference(JNIEnv *env, jobject origin) override;

  static int active_count() { return active_count_; }

  bool valid() override;
};

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_base_references_hpp