package com.harman.vns.ui

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.Spinner
import android.widget.TextView
import android.widget.Toast
import androidx.annotation.Keep
import androidx.fragment.app.Fragment
import com.harman.vns.R
import org.json.JSONObject


class DownloadFragment : Fragment() {

    lateinit var rootView: View
    lateinit var spinner: Spinner
    lateinit var serverUrlId: TextView
    lateinit var filesizes: Array<String>
    lateinit var startDownload: Button
    lateinit var throughputValue: TextView
    lateinit var latencyValue: TextView
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String?

    private external fun startTicks(cmd: String?, hostName: String?)
    private external fun StopTicks()
    private external fun LocationJNI(lat: Double, lng: Double)

    companion object {
        //  Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        rootView = inflater.inflate(R.layout.fragment_download, container, false)
        initView()
        return rootView
    }

    private fun initView() {
        spinner = rootView.findViewById(R.id.fileSizeSpinnerId)
        serverUrlId = rootView.findViewById(R.id.serverUrlId)
        filesizes = resources.getStringArray(R.array.filesize)
        startDownload = rootView.findViewById(R.id.startDownload)
        throughputValue = rootView.findViewById(R.id.throughputValue)
        latencyValue = rootView.findViewById(R.id.latencyValue)

        startDownload.setOnClickListener {
            startTicks("Download", serverUrlId.text.toString())
        }

        val adapter = activity?.let {
            ArrayAdapter(it,
                    android.R.layout.simple_spinner_item, filesizes)
        }
        spinner.adapter = adapter
        spinner.onItemSelectedListener = object :
                AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>,
                view: View, position: Int, id: Long
            ) {
                Toast.makeText(activity,
                        getString(R.string.selected_item) + " " +
                                "" + filesizes[position], Toast.LENGTH_SHORT).show()
//                startTicks("Download", serverUrlId.text.toString())
            }

            override fun onNothingSelected(parent: AdapterView<*>) {
                // write code to perform some action
            }
        }
    }

    /*
* A function calling from JNI to update current timer
*/
    @Keep
    private fun updateTimer(json: String) {
        activity?.runOnUiThread {
            try {
                val json = JSONObject(json)
                latencyValue.text = json.getString("latency_ms")
                throughputValue.text = json.getString("dowloadspeed_kbps")
                Log.d("JniHandler1", "download speed: $json")
            } catch (e: Exception) {
                if (e.localizedMessage.contains("Download done"))
                    Toast.makeText(activity, "Download completed", Toast.LENGTH_SHORT).show()
                e.printStackTrace()
            }
//            val s = displayView.text.toString()
//            displayView.append("\n" + msg)
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
    }

    override fun onDetach() {
        super.onDetach()
    }
}
