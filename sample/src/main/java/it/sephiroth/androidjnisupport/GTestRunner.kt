package it.sephiroth.androidjnisupport

import android.content.Context
import android.util.Log
import java.io.BufferedReader
import java.io.File
import java.io.FileInputStream
import java.io.InputStreamReader
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

open class GTestRunner(context: Context, classLoader: ClassLoader) {
    var outputFile: File? = null

    init {
        val output_dir = File(context.filesDir, "gtest")
        if (!output_dir.exists()) output_dir.mkdirs()
        outputFile = File(output_dir, "test-results.xml")
        outputFile!!.createNewFile()
        initialize(context, classLoader)
    }

    fun runAllTests(observable: Observable) {
        runTests(observable, outputFile?.absolutePath)
    }


    companion object {

        @JvmStatic
        private external fun initialize(context: Context, classLoader: ClassLoader)

        @JvmStatic
        private external fun runTests(observable: Observable, absolutePath: String?)
    }

}