package com.harman.vns


import android.annotation.SuppressLint
import android.content.Intent
import android.graphics.Bitmap
import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.webkit.WebView
import android.webkit.WebViewClient
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import com.harman.vns.databinding.FragmentHttpLiveStreamBinding

/**
 * A simple [Fragment] subclass.
 */
class HttpLiveStream : Fragment() {

    private val TAG = "HttpLiveStream"
    lateinit var binding: FragmentHttpLiveStreamBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        binding = DataBindingUtil.inflate(inflater,
                R.layout.fragment_http_live_stream,
                container,
                false)
        return binding.root
    }

    @SuppressLint("SetJavaScriptEnabled")
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        launchStreaming()
//        launchWeb()
//        binding.webView.settings.setAppCachePath(activity?.cacheDir?.absolutePath)
//        binding.webView.settings.allowFileAccess = true
//        binding.webView.settings.setAppCacheEnabled(true)
//        binding.webView.settings.javaScriptEnabled = true
//
//        binding.webView.loadUrl("https://10.99.88.127:3000/?info=true&roomId=1az7qxxp#video-producer-remote-stats")
    }

    private fun launchStreaming() {
        val openURL = Intent(Intent.ACTION_VIEW)
        openURL.data = Uri.parse("https://10.99.88.127:3000/?info=true&roomId=4rqkkybn#video-producer-remote-stats")
        startActivity(openURL)
    }

    private fun launchWeb() {
        binding.webView.webViewClient = object : WebViewClient() {
            override fun shouldOverrideUrlLoading(view: WebView, url: String?): Boolean {
                Log.d(TAG, "shouldOverrideUrlLoading")
                view.loadUrl(url)
                return true
            }

            override fun onPageStarted(view: WebView?, url: String?, favicon: Bitmap?) {
                super.onPageStarted(view, url, favicon)
                binding.progressBar.visibility = View.VISIBLE
                Log.d(TAG, "onPageStarted")
            }

            override fun onPageFinished(view: WebView?, url: String?) {
                super.onPageFinished(view, url)
                binding.progressBar.visibility = View.GONE
                Log.d(TAG, "onPageFinished")
            }

//            override fun onReceivedSslError(
//                view: WebView?,
//                handler: SslErrorHandler?,
//                error: SslError?
//            ) {
//                super.onReceivedSslError(view, handler, error)
//                handler?.proceed()
//            }
        }
    }
}
