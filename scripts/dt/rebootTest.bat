@echo off 
set /a count=0
echo ============重启测试=============
:A
echo ============等待设备=============
adb -s %1 wait-for-device
echo =======当前测试机器为%1=========
echo =======设备连接成功准备重启=======
adb -s %1 reboot
ping localhost -n 50 >nul
set /a count+=1
echo 重启次数为%count%
goto A
