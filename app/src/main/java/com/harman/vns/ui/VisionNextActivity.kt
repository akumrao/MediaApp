package com.harman.vns.ui

import android.content.pm.ActivityInfo
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.GravityCompat
import androidx.core.view.get
import androidx.navigation.Navigation.findNavController
import androidx.navigation.ui.NavigationUI.navigateUp
import androidx.navigation.ui.NavigationUI.setupActionBarWithNavController
import androidx.navigation.ui.NavigationUI.setupWithNavController
import com.harman.vns.R
import kotlinx.android.synthetic.main.activity_vison_next.drawerLayout
import kotlinx.android.synthetic.main.activity_vison_next.navigationView
import kotlinx.android.synthetic.main.activity_vison_next.toolbar

class VisionNextActivity : AppCompatActivity() {

    companion object {
        // Used to load the 'native-lib' library on application startup.
//        init {
//            System.loadLibrary("native-lib")
//        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vison_next)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        setSupportActionBar(toolbar)

        setupNavigation()
    }

    override fun onBackPressed() {
        if (drawerLayout.isDrawerOpen(GravityCompat.START)) {
            drawerLayout.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        return navigateUp(findNavController(this,
                R.id.nav_host_fragment), drawerLayout)
    }

    private fun setupNavigation() {
        val navController = findNavController(this, R.id.nav_host_fragment)

        // Update action bar to reflect navigation
        setupActionBarWithNavController(this, navController, drawerLayout)

        // Handle nav drawer item clicks
        navigationView.setNavigationItemSelectedListener { menuItem ->
            menuItem.isChecked = true
            drawerLayout.closeDrawers()
            true
        }
//        navigationView.menu[3].setOnMenuItemClickListener {
//            Toast.makeText(this@VisionNextActivity, "logout", Toast.LENGTH_SHORT).show()
//            false
//        }

        // Tie nav graph to items in nav drawer
        setupWithNavController(navigationView, navController)
    }
}
