package it.sephiroth.androidjnisupport

import android.content.Context
import android.util.Log
import java.util.*

//
//  ADOBE CONFIDENTIAL
//  __________________
//
//  Copyright 2017 Adobe
//  All Rights Reserved.
//
//  NOTICE:  All information contained herein is, and remains
//  the property of Adobe and its suppliers, if any. The intellectual
//  and technical concepts contained herein are proprietary to Adobe
//  and its suppliers and are protected by all applicable intellectual
//  property laws, including trade secret and copyright laws.
//  Dissemination of this information or reproduction of this material
//  is strictly forbidden unless prior written permission is obtained
//  from Adobe.
//

open class GTestRunner {

    constructor(context: Context, classLoader: ClassLoader) {
        initialize(context, classLoader)
    }

    fun runAllTests() {
        val observable = Observable()
        observable.addObserver { _, value ->
            Log.d("test", "observer notified with value: $value")

        }

        runTests(observable)

    }


    companion object {

        @JvmStatic
        private external fun initialize(context: Context, classLoader: ClassLoader)

        @JvmStatic
        private external fun runTests(observable: Observable)
    }

}