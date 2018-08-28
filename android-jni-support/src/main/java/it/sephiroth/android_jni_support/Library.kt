package it.sephiroth.android_jni_support

import android.content.Context

object Library {
    fun initialize(context: Context) {
        initializeLibrary(context.applicationContext, context.applicationContext.classLoader)
    }

    @JvmStatic
    external fun initializeLibrary(context: Context, class_loader: ClassLoader)
}