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
import android.widget.RadioGroup
import android.widget.Spinner
import android.widget.TextView
import android.widget.Toast
import androidx.annotation.Keep
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import com.harman.vns.R
import com.harman.vns.databinding.FragmentDownloadBinding
import com.harman.vns.utils.DownloadSize
import com.harman.vns.utils.TransferType
import org.json.JSONObject


class DownloadFragment : Fragment() {

    lateinit var rootView: View
    lateinit var spinner: Spinner
    lateinit var serverUrlId: TextView
    lateinit var filesizes: Array<String>
    lateinit var toggleStart: Button
    lateinit var throughputValue: TextView
    lateinit var latencyValue: TextView
    lateinit var transferType: RadioGroup

    lateinit var uploadLatency: TextView
    lateinit var uploadSpeed: TextView

    lateinit var adapter: ArrayAdapter<String>

    lateinit var binding: FragmentDownloadBinding
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

        //0-> DOWNLOAD
        //1 -> UPLOAD
        //2-> BOTH
        var DOWNLOAD_TYPE = 0
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_download, container, false)
        rootView = binding.root
        initView()
        return rootView
    }

    private fun initView() {
        spinner = rootView.findViewById(R.id.fileSizeSpinnerId)
        serverUrlId = rootView.findViewById(R.id.serverUrlId)
        filesizes = resources.getStringArray(R.array.filesize)
        toggleStart = rootView.findViewById(R.id.startDownload)
        throughputValue = rootView.findViewById(R.id.downloadSpeed)
        latencyValue = rootView.findViewById(R.id.downloadLatency)
        transferType = rootView.findViewById(R.id.radioGroup)

        uploadSpeed = rootView.findViewById(R.id.uploadSpeed)
        uploadLatency = rootView.findViewById(R.id.uploadLatency)

        transferType.setOnCheckedChangeListener { radioGroup, index ->
            DOWNLOAD_TYPE = TransferType.valueOf(resources.getResourceEntryName(index).toString()).index
            when(DOWNLOAD_TYPE) {
                0 -> {
                    spinner.isEnabled = true
                    spinner.setSelection(0)
                    spinner.adapter = adapter
                }
                1 -> {
                    spinner.isEnabled = false
                    serverUrlId.text = resources.getString(R.string.uploadUrl)
                }
                2 -> {
                    Toast.makeText(activity,
                            "bi - directional data transfer Under development",
                            Toast.LENGTH_SHORT).show()
                }
            }
        }

        toggleStart.setOnClickListener {
            if (toggleStart.text == resources.getString(R.string.start)) {
                if (DOWNLOAD_TYPE == 0) {
                    toggleStart.text = resources.getString(R.string.stop)
                    startTicks("Download", serverUrlId.text.toString())
                } else if (DOWNLOAD_TYPE == 1) {
                    //TODO: API not available display toast
                    toggleStart.text = resources.getString(R.string.stop)
                    startTicks("Upload", serverUrlId.text.toString())
//                    Toast.makeText(activity,
//                            "upload data transfer Under development",
//                            Toast.LENGTH_SHORT).show()
                } else {
                    //TODO: API not available display toast
//                    Toast.makeText(activity,
//                            "bi - directional data transfer Under development",
//                            Toast.LENGTH_SHORT).show()
                }
            } else if (toggleStart.text == resources.getString(R.string.stop)) {
                toggleStart.text = resources.getString(R.string.start)
                StopTicks()
            }
        }

        adapter = activity?.let {
            ArrayAdapter(it,
                    android.R.layout.simple_spinner_item, filesizes)
        }!!
        spinner.adapter = adapter
        spinner.onItemSelectedListener = object :
                AdapterView.OnItemSelectedListener {
            override fun onItemSelected(
                parent: AdapterView<*>,
                view: View, position: Int, id: Long
            ) {
                val size = "_${filesizes[position]}"
                serverUrlId.text = DownloadSize.valueOf(size).url
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
                if (json.has("done")) {
                    StopTicks()
                    toggleStart.text = resources.getString(R.string.start)
                    Toast.makeText(activity, json.getString("done"), Toast.LENGTH_SHORT).show()
                    return@runOnUiThread
                }
                when(binding.radioGroup.checkedRadioButtonId) {
                    R.id.download -> {
                        binding.downloadSpeed.text = json.getString("DownloadSpeed_MBS")
                        binding.downloadLatency.text = json.getString("latency_ms")
                    }
                    R.id.upload -> {
                        binding.uploadSpeed.text = json.getString("UplaodSpeed_MBS")
                        binding.uploadLatency.text = json.getString("latency_ms")
                    }
                    R.id.bidirection -> {

                    }
                }


                Log.d("JniHandler1", "download speed: $json")
            } catch (e: Exception) {
//                if (e.localizedMessage.contains("done")) {
                    StopTicks()
                    toggleStart.text = resources.getString(R.string.start)
                    Toast.makeText(activity, "Error", Toast.LENGTH_SHORT).show()
//                }
                e.printStackTrace()
            }
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
    }

    override fun onDetach() {
        super.onDetach()
    }
}
