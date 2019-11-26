# MediaApp
Android App for Webrtc media, RTP, SRTP, Network test, TCP, UDP, Ping, HTTP

To build source code
git clone https://github.com/akumrao/MediaApp.git

git submodule update --init

Install Android studio

open project

connect mobile phone to usb or run emulator 

adb devices

List of devices attached


emulator-5554	device

run app from Android Studio



****************************************************************************************************************
tool chain for ubuntu
apt-get install clang-6.0

apt-get install libc++-dev



update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-6.0 1000


update-alternatives --install /usr/bin/clang clang /usr/bin/clang-6.0 1000
update-alternatives --config clang
update-alternatives --config clang++
