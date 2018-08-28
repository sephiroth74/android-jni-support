package it.sephiroth.androidjnisupport

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import it.sephiroth.android_jni_support.Library
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        fab.setOnClickListener { _ ->
            run {
                System.loadLibrary("native-lib")
                Library.initialize(this)
            }
        }



    }

}
