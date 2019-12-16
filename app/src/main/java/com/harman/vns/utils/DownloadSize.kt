package com.harman.vns.utils

enum class DownloadSize(val url: String) {
    _10MB("http://speedtest.tele2.net/10MB.zip"),
    _100MB("http://speedtest.tele2.net/100MB.zip"),
    _1GB("http://speedtest.tele2.net/1GB.zip")
}

enum class TransferType(val index: Int) {
    download(0),
    upload(1),
    bidirection(2)
}