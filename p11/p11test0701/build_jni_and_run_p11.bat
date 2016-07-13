set /p build=If rebuild jni, please input Y or y. If not, input others:
cd %~p0%jni\
if /i "%build%"=="Y" call ndk-build -B


 adb push "%~p0%libs\armeabi-v7a\p11test" /data/local/tmp/

 adb shell "chmod 777 /data/local/tmp/p11test"

adb shell "cd /data/local/tmp/ && ./p11test"
 

pause
