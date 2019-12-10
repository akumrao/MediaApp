package com.harman.vns.ui

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.ArrayAdapter
import android.widget.Spinner
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.harman.vns.R

class DownloadFragment : Fragment() {

    private external fun startTicks(name: String?)
    private external fun StopTicks()

    lateinit var rootView: View
    lateinit var spinner: Spinner
    lateinit var serverUrlId: TextView
    lateinit var filesizes:Array<String>



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

        val adapter = activity?.let {
            ArrayAdapter(it,
                android.R.layout.simple_spinner_item, filesizes)
        }
        spinner.adapter = adapter
        spinner.onItemSelectedListener = object :
                AdapterView.OnItemSelectedListener {
            override fun onItemSelected(parent: AdapterView<*>,
                view: View, position: Int, id: Long) {
                Toast.makeText(activity,
                        getString(R.string.selected_item) + " " +
                                "" + filesizes[position], Toast.LENGTH_SHORT).show()
            }

            override fun onNothingSelected(parent: AdapterView<*>) {
                // write code to perform some action
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
