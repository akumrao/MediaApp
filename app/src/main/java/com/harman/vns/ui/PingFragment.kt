package com.harman.vns.ui

import android.Manifest
import android.content.Context
import android.content.IntentSender
import android.content.pm.PackageManager
import android.graphics.BitmapFactory
import android.location.Address
import android.location.Geocoder
import android.location.Location
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.annotation.Keep
import androidx.core.app.ActivityCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.google.android.gms.common.api.ResolvableApiException
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationCallback
import com.google.android.gms.location.LocationRequest
import com.google.android.gms.location.LocationResult
import com.google.android.gms.location.LocationServices
import com.google.android.gms.location.LocationSettingsRequest
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.BitmapDescriptorFactory
import com.google.android.gms.maps.model.CameraPosition
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.Marker
import com.google.android.gms.maps.model.MarkerOptions
import com.harman.vns.R
import java.io.IOException

class PingFragment : Fragment(), OnMapReadyCallback, GoogleMap.OnMarkerClickListener {
    var second = 0
    lateinit var displayView: TextView
    lateinit var mButtonStop: Button
    lateinit var mButtonStart: Button
    lateinit var mHostNameId: EditText
    lateinit var mGeocoder: Geocoder
    private lateinit var googleMap: GoogleMap

    // 1
    private lateinit var locationCallback: LocationCallback
    // 2
    private lateinit var locationRequest: LocationRequest
    private var locationUpdateState = false
    private lateinit var fusedLocationClient: FusedLocationProviderClient
    private lateinit var lastLocation: Location
    lateinit var marker: Marker

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String?

    private external fun startTicks(cmd: String?, hostName: String?)
    private external fun StopTicks()
    private external fun LocationJNI(lat: Double, lng: Double)

    companion object {
        fun newInstance() = PingFragment()
        private const val LOCATION_PERMISSION_REQUEST_CODE = 1

        private const val REQUEST_CHECK_SETTINGS = 2

        //  Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    private lateinit var viewModel: PingViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mGeocoder = Geocoder(activity)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view: View = inflater.inflate(R.layout.ping_fragment, container, false)
        initView(view)
        return view
    }

    private fun initView(view: View) {
        // Example of a call to a native method
        displayView = view.findViewById(R.id.displayId)
        displayView.movementMethod = ScrollingMovementMethod()
        mHostNameId = view.findViewById(R.id.hostNameId)
        //        displayView.setText(stringFromJNI());
        mButtonStart = view.findViewById(R.id.buttonStart)
        mButtonStart.setOnClickListener(View.OnClickListener {
            mButtonStop.isEnabled = true
            mButtonStart.isEnabled = false
            displayView.text = null
            var host = mHostNameId.text.toString()
            if (host.isEmpty()) host = "www.google.com"
            startTicks("Ping", host)
        })
        mButtonStop = view.findViewById(R.id.buttonStop)
        mButtonStop.setOnClickListener(View.OnClickListener {
            mButtonStart.isEnabled = true
            mButtonStop.isEnabled = false
            StopTicks()
        })
        fusedLocationClient = LocationServices.getFusedLocationProviderClient(activity!!)

        locationCallback = object : LocationCallback() {
            override fun onLocationResult(p0: LocationResult) {
                super.onLocationResult(p0)

                lastLocation = p0.lastLocation
                val location = LatLng(lastLocation.latitude, lastLocation.longitude)
//                placeMarkerOnMap(location)
//                Toast.makeText(this, "lat ${location.latitude} ${location.longitude}", Toast.LENGTH_SHORT).show()
                LocationJNI(location.latitude, location.longitude)
//                marker = googleMap.addMarker(MarkerOptions().position(LatLng(lastLocation.latitude,
//                        lastLocation.longitude)).flat(true).icon(
//                        BitmapDescriptorFactory.fromResource(R.mipmap.ic_user_location)))

                googleMap.animateCamera(CameraUpdateFactory.newCameraPosition(
                        CameraPosition.Builder().target(
                                location).zoom(15.5f).build()))
            }
        }
        createLocationRequest()

        val mapFragment = childFragmentManager.findFragmentById(R.id.mapView) as SupportMapFragment?
        mapFragment!!.getMapAsync(this::onMapReady)
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProviders.of(this).get(PingViewModel::class.java)
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
    }

    override fun onMapReady(googleMap: GoogleMap) {
        this.googleMap = googleMap
        googleMap.uiSettings.isMyLocationButtonEnabled = true
        googleMap.setOnMarkerClickListener(this)
        setUpMap()
    }

    override fun onMarkerClick(marker: Marker): Boolean = false

    private fun setUpMap() {
        if (ActivityCompat.checkSelfPermission(
                        activity!!,
                        Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    activity!!,
                    arrayOf(Manifest.permission.ACCESS_FINE_LOCATION),
                    LOCATION_PERMISSION_REQUEST_CODE)
            return
        }

        // 1
        googleMap.isMyLocationEnabled = true

        fusedLocationClient.lastLocation.addOnSuccessListener { location ->
            // Got last known location. In some rare situations this can be null.
            // 3
            if (location != null) {
                lastLocation = location
                val currentLatLng = LatLng(location.latitude, location.longitude)
//                Log.d("LOCATION", "lat ${location.latitude} ${location.longitude}")
                placeMarkerOnMap(currentLatLng)
//                map.animateCamera(CameraUpdateFactory.newLatLngZoom(currentLatLng, 12f))
//                map.animateCamera(CameraUpdateFactory.newCameraPosition(
//                        CameraPosition.Builder().target(
//                                currentLatLng).zoom(15.5f).build()))
            }
        }
    }


    private fun placeMarkerOnMap(location: LatLng) {
        val markerOptions = MarkerOptions().position(location)

        val titleStr = getAddress(location)
        markerOptions.title(titleStr)

        markerOptions.icon(BitmapDescriptorFactory.fromBitmap(
                BitmapFactory.decodeResource(resources, R.mipmap.ic_user_location)))
        googleMap.addMarker(markerOptions)
    }

    private fun getAddress(latLng: LatLng): String {
        // 1
//        val geocoder = Geocoder(activity)
        val addresses: List<Address>?
        val address: Address?
        var addressText = ""

        try {
            // 2
            addresses = mGeocoder.getFromLocation(latLng.latitude, latLng.longitude, 1)
            // 3
            if (null != addresses && addresses.isNotEmpty()) {
                address = addresses[0]
//                for (i in 0 until address.maxAddressLineIndex) {
//                    addressText += if (i == 0) address.getAddressLine(i) else "\n" + address.getAddressLine(i)
//                }
                addressText = addresses[0].getAddressLine(0) //                + ","
//                + addresses.get(0).getAddressLine(1)

            }
        } catch (e: IOException) {
            Log.e("MapsActivity", e.localizedMessage)
        }

        return addressText
    }

    private fun createLocationRequest() {
        // 1
        locationRequest = LocationRequest()
        // 2
        locationRequest.interval = 10000
        // 3
        locationRequest.fastestInterval = 5000
        locationRequest.priority = LocationRequest.PRIORITY_HIGH_ACCURACY

        val builder = LocationSettingsRequest.Builder()
                .addLocationRequest(locationRequest)

        // 4
        val client = LocationServices.getSettingsClient(activity!!)
        val task = client.checkLocationSettings(builder.build())

        // 5
        task.addOnSuccessListener {
            locationUpdateState = true
            startLocationUpdates()
        }
        task.addOnFailureListener { e ->
            // 6
            if (e is ResolvableApiException) {
                // Location settings are not satisfied, but this can be fixed
                // by showing the user a dialog.
                try {
                    // Show the dialog by calling startResolutionForResult(),
                    // and check the result in onActivityResult().
                    e.startResolutionForResult(activity,
                            REQUEST_CHECK_SETTINGS)
                } catch (sendEx: IntentSender.SendIntentException) {
                    // Ignore the error.
                }
            }
        }
    }

    private fun startLocationUpdates() {
        //1
        if (ActivityCompat.checkSelfPermission(activity!!,
                        android.Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(activity!!,
                    arrayOf(android.Manifest.permission.ACCESS_FINE_LOCATION),
                    LOCATION_PERMISSION_REQUEST_CODE)
            return
        }
        //2
        fusedLocationClient.requestLocationUpdates(locationRequest,
                locationCallback,
                null /* Looper */)
    }

    /*
 * A function calling from JNI to update current timer
 */
    @Keep
    private fun updateTimer(msg: String) {
        activity?.runOnUiThread {
            Log.e("JniHandler1", "Native Err: $msg")
            val s = displayView.text.toString()
            displayView.append("\n" + msg)
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
    }

    override fun onDetach() {
        super.onDetach()
        fusedLocationClient.removeLocationUpdates(locationCallback)
        StopTicks()
    }
}
