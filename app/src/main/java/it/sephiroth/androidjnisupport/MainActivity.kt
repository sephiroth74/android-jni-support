package it.sephiroth.androidjnisupport

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        System.loadLibrary("native-lib")

        val testRunner = GTestRunner(applicationContext, applicationContext.classLoader)

        fab.setOnClickListener { _ ->
            run {
                testRunner.runAllTests()
            }
        }


    }

}
