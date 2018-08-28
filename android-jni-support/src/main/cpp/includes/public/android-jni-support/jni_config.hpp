
//
// Created by Alessandro Crugnola on 8/27/18.
//

#ifndef android_jni_support_jni_config_hpp
#define android_jni_support_jni_config_hpp

#ifndef ANDROID_JNI_NAMESPACE_BEGIN_DECL
#define ANDROID_JNI_NAMESPACE_BEGIN_DECL \
namespace android { namespace jni { \

#endif

#ifndef ANDROID_JNI_NAMESPACE_END_DECL
#define ANDROID_JNI_NAMESPACE_END_DECL \
}; \
}; \
   \

#endif

#ifndef ANDROID_JNI_NS
#define ANDROID_JNI_NS(x) android::jni::x
#endif

#ifndef USING_NAMESPACE_ANDROID_JNI
#define USING_NAMESPACE_ANDROID_JNI using namespace android::jni; \
\

#endif


#endif // android_jni_support_jni_config_hpp
