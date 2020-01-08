package com.harman.vns


import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import com.harman.vns.databinding.FragmentLiveStreamingBinding

/**
 * A simple [Fragment] subclass.
 */
class LiveStreamingFragment : Fragment() {

    lateinit var binding: FragmentLiveStreamingBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        binding = DataBindingUtil.inflate(inflater, R.layout.fragment_live_streaming, container, false)
        initView()
        return binding.root
    }

    private fun initView() {
        binding.startVideo.setOnClickListener {
            binding.videoView.setVideoPath("https://www.radiantmediaplayer.com/media/bbb-360p.mp4")
            binding.videoView.start()
        }
        binding.stopVideo.setOnClickListener {
            binding.videoView.stopPlayback()
        }
    }
}
