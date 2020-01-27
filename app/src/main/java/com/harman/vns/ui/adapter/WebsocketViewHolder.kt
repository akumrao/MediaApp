package com.harman.vns.ui.adapter

import android.view.View
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.harman.vns.R

class WebsocketViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
    val userName: TextView = itemView.findViewById(R.id.userName)
    val sendMsg: TextView = itemView.findViewById(R.id.sendMsg)
    val status: ImageView = itemView.findViewById(R.id.status)
}