package it.sephiroth.androidjnisupport

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.content_main.*
import java.util.*

class MainActivity : AppCompatActivity() {

    val observable = Observable()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        System.loadLibrary("native-lib")

        val testRunner = GTestRunner(applicationContext, applicationContext.classLoader)

        observable.addObserver { observable, any ->
            run {
                when (any) {
                    is Int -> displayResult(any)
                }
            }
        }

        button.setOnClickListener { _ ->
            run {
                button.text = "Running..."
                button.isEnabled = false

                textView.text = ""
                testRunner.runAllTests(observable)
            }
        }
    }

    fun displayResult(result: Int) {
        runOnUiThread {
            textView.text = "Result Code: $result"
            button.isEnabled = true
            button.text = getString(R.string.button_text)
        }
    }


}
