::set /p build=If rebuild jni, please input Y or y. If not, input others:
cd %~p0%jni\
::%if /i "%build%"=="Y" call ndk-build -B%
::%if /i "%build%"=="y" call ndk-build -B%
call ndk-build -B

 adb push "%~p0%libs\armeabi-v7a\p11test" /data/

 adb shell "chmod 777 /data//p11test"

 adb shell "cd /data/ && ./p11test"

pause
