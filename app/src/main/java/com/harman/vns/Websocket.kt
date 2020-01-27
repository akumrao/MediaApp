package com.harman.vns

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.DividerItemDecoration
import com.harman.vns.databinding.FragmentWebsocketBinding
import com.harman.vns.ui.WebsocketModel
import com.harman.vns.ui.adapter.WebsocketAdapter

class Websocket : Fragment() {
    lateinit var binding: FragmentWebsocketBinding

    lateinit var websocketAdapter: WebsocketAdapter
    lateinit var model: WebsocketModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_websocket, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        observerLiveObjects()
    }

    private fun observerLiveObjects() {
        val listModel = arrayListOf<WebsocketModel>()
        for (i in 1..10) {
            if (i % 2 == 0)
                model = WebsocketModel(clientid = "user$i", status = "online")
            else
                model = WebsocketModel(clientid = "user$i", status = "offline")
            listModel.add(model)
        }
        websocketAdapter = WebsocketAdapter(listModel)
        val dividerItemDecoration = DividerItemDecoration(binding.userList.context,
                DividerItemDecoration.VERTICAL)
        binding.userList.addItemDecoration(dividerItemDecoration)
        binding.userList.adapter = websocketAdapter

        websocketAdapter.setOnSendItemClickListener(object : WebsocketAdapter.OnSendItemClickListener {
            override fun onItemClicked(view: View, position: Int) {
                Toast.makeText(activity, "Hello ${listModel[position].clientid}", Toast.LENGTH_SHORT).show()
            }
        })
    }
}
