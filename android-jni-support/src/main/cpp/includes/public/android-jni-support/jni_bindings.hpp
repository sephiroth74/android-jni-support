
#ifndef android_jni_support_jni_bindings_hpp
#define android_jni_support_jni_bindings_hpp

#include <string.h>

#include <android-jni-support/jni_references.hpp>
#include <android-jni-support/jni_macros.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

struct kJavaEnum : JNIBindingBase {};

struct kJavaObject : JNIBindingBase {};
struct kAndroidContext : JNIBindingBase {};
struct kAndroidClassLoader : JNIBindingBase {};
struct kJavaRuntime : JNIBindingBase {};
struct kAndroidRect : JNIBindingBase {};
struct kAndroidRectF : JNIBindingBase {};
struct kJavaBoolean : JNIBindingBase {};
struct kJavaInteger : JNIBindingBase {};
struct kJavaLocale : JNIBindingBase {};
struct kAndroidSharedPreferences : JNIBindingBase {};
struct kAndroidContextCompat : JNIBindingBase {};
struct kJavaFile : JNIBindingBase {};
struct kJavaHashMap : JNIBindingBase {};
struct kAndroidEnvironment : JNIBindingBase {};
struct kJavaThread : JNIBindingBase {};

// definitions

// ----------------------------------------------------------------------------
// java.lang.Object
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kJavaObject, "java/lang/Object")
  JNI_METHOD(toString, "()Ljava/lang/String;", std::string)
  JNI_METHOD(wait, "()V", void)
  JNI_METHOD(notify, "()V", void)
  JNI_METHOD(notifyAll, "()V", void)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// android.content.Context
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kAndroidContext, "android/content/Context", JNIClassTraits<kJavaObject>)
  JNI_METHOD(getExternalCacheDir, "()Ljava/io/File;", JNIAutoUnref<jobject>)
  JNI_METHOD(getExternalFilesDir, "(Ljava/lang/String;)Ljava/io/File;", JNIAutoUnref<jobject>)
  JNI_METHOD(getExternalCacheDirs, "()[Ljava/io/File;", std::vector<JNIAutoUnref<jobject>>)
  JNI_METHOD(getSharedPreferences, "(Ljava/lang/String;I)Landroid/content/SharedPreferences;", JNIAutoUnref<jobject>)
  JNI_METHOD(getCacheDir, "()Ljava/io/File;", JNIAutoUnref<jobject>)
  JNI_METHOD(getFilesDir, "()Ljava/io/File;", JNIAutoUnref<jobject>)
  JNI_METHOD(getAssets, "()Landroid/content/res/AssetManager;", JNIAutoUnref<jobject>)
  JNI_METHOD(getResources, "()Landroid/content/res/Resources;", JNIAutoUnref<jobject>)
  JNI_METHOD(getClassLoader, "()Ljava/lang/ClassLoader;", JNIAutoUnref<jobject>)
  JNI_METHOD(getPackageName, "()Ljava/lang/String;", std::string)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.lang.Runtime
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kJavaRuntime, "java/lang/Runtime", JNIClassTraits<kJavaObject>)
  JNI_STATIC_METHOD(getRuntime, "()Ljava/lang/Runtime;", JNIAutoUnref<jobject>)
  JNI_METHOD(totalMemory, "()J", long)
  JNI_METHOD(maxMemory, "()J", long)
  JNI_METHOD(freeMemory, "()J", long)
  JNI_METHOD(availableProcessors, "()I", int)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// android.graphics.RectF
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kAndroidRectF, "android/graphics/RectF", JNIClassTraits<kJavaObject>)
  JNI_CTOR(ctor, "(FFFF)V")
  JNI_CTOR_RAW(ctorRaw, "(FFFF)V")
  JNI_CTOR_RAW(ctor2, "(Landroid/graphics/Rect;)V")
  JNI_CTOR(ctor2Raw, "(Landroid/graphics/Rect;)V")
  JNI_FIELD(left, "F", float)
  JNI_FIELD(top, "F", float)
  JNI_FIELD(right, "F", float)
  JNI_FIELD(bottom, "F", float)
  JNI_METHOD(round, "(Landroid/graphics/Rect;)V", void)
  JNI_METHOD(width, "()F", float)
  JNI_METHOD(height, "()F", float)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.lang.Boolean
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kJavaBoolean, "java/lang/Boolean", JNIClassTraits<kJavaObject>)
  JNI_STATIC_FIELD(TRUE, "Ljava/lang/Boolean;", JNIAutoUnref<jobject>)
  JNI_STATIC_FIELD(FALSE, "Ljava/lang/Boolean;", JNIAutoUnref<jobject>)
  JNI_METHOD(booleanValue, "()Z", bool)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.lang.Integer
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kJavaInteger, "java/lang/Integer", JNIClassTraits<kJavaObject>)
  JNI_STATIC_METHOD(valueOf, "(I)Ljava/lang/Integer;", JNIAutoUnref<jobject>)
  JNI_METHOD(intValue, "()I", int)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.lang.Enum
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kJavaEnum, "java/lang/Enum")
  JNI_METHOD(ordinal, "()I", int)
  JNI_METHOD(name, "()Ljava/lang/String;", std::string)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.util.Locale
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kJavaLocale, "java/util/Locale")
  JNI_STATIC_METHOD(getDefault, "()Ljava/util/Locale;", JNIAutoUnref<jobject>)
  JNI_METHOD(getLanguage, "()Ljava/lang/String;", std::string)
  JNI_METHOD(getCountry, "()Ljava/lang/String;", std::string)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// android.content.SharedPreferences
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kAndroidSharedPreferences, "android/content/SharedPreferences")
  JNI_METHOD(getInt, "(Ljava/lang/String;I)I", int)
  JNI_METHOD(getString, "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", std::string)
JNI_CLASS_END()


// ----------------------------------------------------------------------------
// android.support.v4.content.ContextCompat
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kAndroidContextCompat, "android/support/v4/content/ContextCompat")
  JNI_STATIC_METHOD(getExternalCacheDirs,
                    "(Landroid/content/Context;)[Ljava/io/File;",
                    std::vector<JNIAutoUnref<jobject>>)
  JNI_STATIC_METHOD(getExternalFilesDirs,
                    "(Landroid/content/Context;Ljava/lang/String;)[Ljava/io/File;",
                    std::vector<JNIAutoUnref<jobject>>)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.io.File
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kJavaFile, "java/io/File", JNIClassTraits<kJavaObject>)
  JNI_METHOD(getAbsolutePath, "()Ljava/lang/String;", std::string)
  JNI_CTOR(ctor, "(Ljava/io/File;Ljava/lang/String;)V")
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.util.HashMap
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN_IMPL(kJavaHashMap, "java/util/HashMap", JNIClassTraits<kJavaObject>)
  JNI_CTOR(ctor, "()V")
  JNI_CTOR(ctor2, "(I)V")
  JNI_METHOD(put, "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", JNIAutoUnref<jobject>)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// android.os.Environment
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kAndroidEnvironment, "android/os/Environment")
  JNI_STATIC_METHOD(getExternalStorageState, "(Ljava/io/File;)Ljava/lang/String;", std::string)
JNI_CLASS_END()

// ----------------------------------------------------------------------------
// java.lang.Thread
// ----------------------------------------------------------------------------
JNI_CLASS_BEGIN(kJavaThread, "java/lang/Thread")
  JNI_STATIC_METHOD(currentThread, "()Ljava/lang/Thread;", JNIAutoUnref<jobject>)
  JNI_METHOD(setName, "(Ljava/lang/String;)V", void)
JNI_CLASS_END()

ANDROID_JNI_NAMESPACE_END_DECL

#endif // android_jni_support_jni_bindings_hpp