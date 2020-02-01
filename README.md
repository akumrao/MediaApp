# MediaApp
Android App for Webrtc media, RTP, SRTP, Network test, TCP, UDP, Ping, HTTP

To build source code
git clone --recursive https://github.com/akumrao/MediaApp.git

for git 2.3 later
git clone --recurse-submodules https://github.com/akumrao/MediaApp.git

git submodule update --init

Install Android studio
akumrao-patch-1
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


update-alternatives --install /usr/bin/clang clang /usr/bin/clang-6.0 1000akumrao-patch-1
update-alternatives --config clang
update-alternatives --config clang++

apt-get install ninja



***********************************************************************************************************************

./emulator -writable-system  -netdelay none -netspeed full -avd Pixel_2_API_28 

./emulator -writable-system  -selinux permissive -avd  Pixel_3_XL_API_27 -selinux permissive

adb root

adb remount

mount -o rw,remount /system

**************************************************************************************************************

For windows

Install NDK if missing at Andriod Studio

if you get compilation error at NDK

rm -rf app/.cxx/

./gradlew clean



![GitHub Logo](/MediaServer.png)akumrao-patch-1
