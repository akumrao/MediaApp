package com.harman.vns.ui.adapter

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.harman.vns.R
import com.harman.vns.ui.WebsocketModel

class WebsocketAdapter(val list: ArrayList<WebsocketModel>) : RecyclerView.Adapter<WebsocketViewHolder>() {

    lateinit var listener: OnSendItemClickListener

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): WebsocketViewHolder {
        val itemContainer: ViewGroup = LayoutInflater.from(parent.context)
                .inflate(R.layout.list_times, parent, false) as ViewGroup
        return WebsocketViewHolder(itemContainer)
    }

    override fun getItemCount(): Int {
        return list.size
    }

    override fun onBindViewHolder(holder: WebsocketViewHolder, position: Int) {
        holder.userName.text = list[position].clientid
        if (list[position].status == "online") holder.status.setImageResource(R.drawable.online)
        else holder.status.setImageResource(R.drawable.offline)
        holder.sendMsg.setOnClickListener {
            listener.onItemClicked(holder.itemView, position)
        }
    }

    interface OnSendItemClickListener {
        fun onItemClicked(view: View, position: Int)
    }

    fun setOnSendItemClickListener(itemClick: OnSendItemClickListener) {
        listener = itemClick
    }
}