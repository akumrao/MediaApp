package com.harman.vns.ui

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.Navigation.findNavController
import com.harman.vns.R

class VisionNextActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vison_next)
    }

    override fun onSupportNavigateUp() = findNavController(this,
            R.id.navHostFragment).navigateUp()
}
