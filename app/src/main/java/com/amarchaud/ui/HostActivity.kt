package com.amarchaud.ui

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.amarchaud.ui.nativeexample.R

class HostActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_host)
    }
}