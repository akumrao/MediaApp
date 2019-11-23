#base script for Pixel_3_XL_API_27.avd for Andriod Orio 8.1 86-64
#!/bin/bash
set -x




#./emulator -list-avds

#./gradlew clean

#./gradlew build

if ps ax | grep -v grep | grep emulator > /dev/null
then
    echo "emulator service running, everything is fine"
else    java.lang.UnsatisfiedLinkError: dalvik.system    java.lang.UnsatisfiedLinkError: dalvik.system.PathClassLoader[DexPathList[[zip file "/system/app/HelloApp/app-hello.apk"],nativeLibraryDirectories=[/system/app/HelloApp/lib/x86_64, /system/app/HelloApp/app-hello.apk!/lib/x86_64, /system/lib64, /vendor/lib64, /system/lib64, /vendor/lib64]]] couldn't find "libhello-jnicallback.so"
.PathClassLoader[DexPathList[[zip file "/system/app/HelloApp/app-hello.apk"],nativeLibraryDirectories=[/system/app/HelloApp/lib/x86_64, /system/app/HelloApp/app-hello.apk!/lib/x86_64, /system/lib64, /vendor/lib64, /system/lib64, /vendor/lib64]]] couldn't find "libhello-jnicallback.so"

    emulator -writable-system  -selinux permissive -avd  Pixel_3_XL_API_27 -selinux permissive &
fi



# CHANGE THESE FOR YOUR APP
app_package="com.example.helloapp"

MAIN_ACTIVITY="MainActivity"

ADB="adb" # how you execute adb
ADB_SH="$ADB shell" # this script assumes using `adb root`. for `adb su` see `Caveats`
#ADB_SH="$ADB shell su -c"

path_sysapp="/system/app" # assuming the app is priviledged
apk_host="./app/build/outputs/apk/debug/app-debug.apk"


#apk_name=$dir_app_name".apk"

apk_name="app-hello.apk"
dir_app_name="HelloApp"

apk_target_dir="$path_sysapp/$dir_app_name"
apk_target_sys="$apk_target_dir/$apk_name"

# Delete previous APK
rm -f $apk_host

# Compile the APK: you can adapt this for production build, flavors, etc.
./gradlew assembleDebug || exit -1 # exit on failure

# Install APK: using adb root
$ADB root 2> /dev/null
$ADB remount # mount system

$ADB_SH "mkdir -p $apk_target_dir"
$ADB shell "am force-stop $app_package"

$ADB push $apk_host $apk_target_sys

#if first time enable below code
#install by reboot
#$ADB reboot
#exit 1

#other wise reinstall apk . -t for debug apk
$ADB shell "am force-stop $app_package"
$ADB shell "pm install -r -t $apk_target_sys"

# Install APK: using adb su
#$ADB_SH "mount -o rw,remount /system"
#$ADB_SH "chmod 777 /system/lib/"
#$ADB_SH "mkdir -p /sdcard/tmp" 2> /dev/null
#$ADB_SH "mkdir -p $apk_target_dir" 2> /dev/null
#$ADB push $apk_host /sdcard/tmp/$apk_name 2> /dev/null
#$ADB_SH "mv /sdcard/tmp/$apk_name $apk_target_sys"
#$ADB_SH "rmdir /sdcard/tmp" 2> /dev/null



# Give permissions
# $ADB_SH "chmod 755 $apk_target_dir"
# $ADB_SH "chmod 644 $apk_target_sys"

#Unmount system
# $ADB_SH "mount -o remount,ro /"

# Stop the app


# Re execute the app
$ADB shell "am start -n \"$app_package/$app_package.$MAIN_ACTIVITY\" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER"

