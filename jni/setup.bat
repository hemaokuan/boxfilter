echo off

call ndk-build NDK_MODULE_PATH:=.  APP_ABI="armeabi-v7a" NDK_TOOLCHAIN_VERSION:=4.9

pause

call adb push ../libs/armeabi-v7a/helloneon /data/local/tmp/bruce
call adb shell chmod 777 ../data/local/tmp/bruce/helloneon
call adb shell /data/local/tmp/bruce/helloneon 

call adb pull /sdcard/bruce/insect_blur.yuv D:\test\

