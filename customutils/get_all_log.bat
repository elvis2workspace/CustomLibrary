@echo off
:: ================================ 修订记录 =======================================
:: 2014-01-20  3.1
::                *  中文全部替换成英文
::                *  add screencap
::                *  remove ping cmd when get d sysrq
::                *  add colors
::                *  add new modem log
::                *  add screencap
::                *  add delete option
::                *  add adb detect
:: 2014-01-27  3.2
::                *  fix ls list /sdcard directory fail
::                *  add modem_state log
::                *  add k3v3 sec os log
:: 2014-01-27  3.3
::                *  delete logservice log
::                *  add delete options for product line
:: 2014-02-19  3.4
::                *  support recovery mod
::                *  move logserver log to new directory
::                *  remove nvme
::
set ver_str=3.4
echo =====================GetLog version %ver_str% ================================
:: echo Current: %time% 即 %time:~0,2%点%time:~3,2%分%time:~6,2%秒%time:~9,2%厘秒@
echo ####Current: %date% %time%
 
set date_time="%date:~0,4%%date:~5,2%%date:~8,2%_%time:~0,2%%time:~3,2%"
set Folder="Logs_%ver_str%_%date:~0,4%%date:~5,2%%date:~8,2%_%time:~0,2%%time:~3,2%%time:~6,2%"
echo ####start to get log to (%CD%\%Folder%)...
mkdir %Folder%
 
adb start-server
adb devices|findstr /e device
if %errorlevel% EQU 0 (
  echo ####find devices ................ [device] .........................
  goto START
)
adb devices|findstr /e recovery
if %errorlevel% EQU 0 (
  echo ####find devices ................ [recovery] .........................
  goto START
)
echo  !!!!!!!!!!!!!!!!!Can't find device!!!!!!!!!!!!!!!!!!!
goto END
 
:START
echo ####
::======================board id============================================
:: get board id
echo ####get board id ...
adb shell  cat /sys/kernel/debug/boardid/boardid > %Folder%\boardid.txt
echo ####get board id common ...
adb shell  cat /sys/kernel/debug/boardid/common/common > %Folder%\boardid_common.txt
echo ####get blocks iomux ...
adb shell  cat /sys/kernel/debug/iomux/blockmux > %Folder%\iomux_blocks.txt
echo ####get pins iomux ...
adb shell  cat /sys/kernel/debug/iomux/pinmux > %Folder%\iomux_pins.txt
 
::======================Android light log===================================
:: common logs from android
echo ####get dmesg to dmesg.txt ...
adb shell  dmesg > %Folder%\dmesg.txt
echo ####get ps list to ps.txt ...
adb shell  ps > %Folder%\ps.txt
echo ####get ps thread list to ps_t.txt ...
adb shell  ps -t > %Folder%\ps_t.txt
echo ####get properties to prop.txt ...
adb shell  getprop > %Folder%\prop.txt
echo ####get cmdline to cmdline.txt ...
adb shell  cat /proc/cmdline > %Folder%\cmdline.txt
echo ####get main logcat to logcat.txt ...
adb shell  logcat -v threadtime -d -t 1000 > %Folder%\logcat.txt
echo ####get dontpanic to .\dontpanic ...
adb pull   /data/dontpanic  %Folder%\dontpanic\
echo ####get dropbox to .\dropbox ...
adb pull   /data/system/dropbox  %Folder%\dropbox
echo ####get tombstones to .\tombstones ...
adb pull   /data/tombstones  %Folder%\tombstones
echo ####get anr to .\anr ...
adb pull   /data/anr %Folder%\anr
echo ####get data file list to .\userdata_check.txt ...
adb shell ls -lR /data  > %Folder%/userdata_check.txt
echo ####get D state process to dmesg_sysrq.txt ...
adb shell "echo 1 > /proc/sys/kernel/sysrq"
adb shell "echo w > /proc/sysrq-trigger"
adb shell  dmesg > %Folder%/dmesg_sysrq.txt
echo ####get balong_power modem_state to .\modem_state.txt
adb shell cat /sys/devices/platform/balong_power/modem_state > %Folder%\modem_state.txt
:: ======================TERMINL LOG========================================
:: get huawei persist log
echo ####get android_logs to .\android_logs ...
adb pull   /data/android_logs %Folder%/android_logs
echo ####get splash2 to .\splash2 ...
adb pull   /splash2 %Folder%\splash2
echo ####get simcard offline logs to .\SimCardOfflinelogs ...
adb pull   /data/offlinelogs %Folder%/SimCardOfflinelogs
 
:: ======================LOG Server log=====================================
:: get log server log
adb shell ls /data/ | findstr /X log
if %errorlevel% EQU 0 (
    echo ####get logserver log to .\logserver-log1
    adb pull   /data/log/archive/    %Folder%/logserver-log1/archive/
    adb pull   /data/log/backup/    %Folder%/logserver-log1/backup/
    adb pull   /data/log/socket_track.txt   %Folder%/logserver-log1/socket_track.txt
    adb pull   /data/log/unzip/    %Folder%/logserver-log1/unzip/
)
adb shell ls /sdcard/ | findstr /X log
if %errorlevel% EQU 0 (
    echo ####get logserver sdcard log to .\logserver-log2
    adb pull   /sdcard/log/archive/    %Folder%/logserver-log2/archive/
    adb pull   /sdcard/log/backup/    %Folder%/logserver-log2/backup/
    adb pull   /sdcard/log/unzip/    %Folder%/logserver-log2/unzip2/
    echo ####get logserver modem log to .\modem-log
    adb pull   /sdcard/log/modem    %Folder%/modem-log/
    echo ####get logserver manual ap log to .\manual-AP
    adb pull   /sdcard/log/manual-AP/  %Folder%/manual-AP/
)
adb shell ls /sdcard/ | findstr /X LogService
if %errorlevel% EQU 0 (
    echo ####get LogService  .\LogService4.0
    adb pull   /sdcard/LogService/    %Folder%/LogService4.0/
)
 
adb shell ls /storage/sdcard1/ | findstr /X LogService
if %errorlevel% EQU 0 (
    echo ####get LogService  .\LogService4.0sdcard
    adb pull   /storage/sdcard1/LogService    %Folder%/LogService4.0sdcard/
)
 
adb shell ls /storage/sdcard1/ | findstr /X log
if %errorlevel% EQU 0 (
    echo ####get LogService log  .\LogService4.0sdcard
    adb pull   /storage/sdcard1/log/unzip/    %Folder%/LogService4.0sdcard/unzip/
)
:: =========================K3V3 LOG========================================
adb shell ls /data/ | findstr /X dumplog
if %errorlevel% EQU 0 (
    echo ####get hisi dumplog to .\dumplog
    adb pull /data/dumplog %Folder%/dumplog
)
adb shell ls /data/ | findstr /X sec_storage
if %errorlevel% EQU 0 (
    echo ####get secOS log  to .\sec_log
    adb pull /data/sec_storage/LOG@06060606060606060606060606060606 %Folder%/sec_log/LOG@06060606060606060606060606060606
)
 
adb shell ls /data/ | findstr /X rdr
if %errorlevel% EQU 0 (
    echo ####get rdr bin  to .\rdr
    adb pull /data/rdr %Folder%/rdr
)
adb shell ls /data/ | findstr /X factory-modemlog
if %errorlevel% EQU 0 (
    echo ####get factory-modemlog  to .\factory-modemlog
    adb pull /data/factory-modemlog %Folder%/factory-modemlog
)
adb pull /sdcard/SrTestLog.txt %Folder%/SrTestLog.txt
adb pull /data/testlog.txt %Folder%/testlog.txt
adb pull /data/processInfo.txt %Folder%/processInfo.txt
adb pull /sdcard/rebootLog.txt %Folder%/rebootLog.txt
adb pull /sdcard/result.txt %Folder%/result.txt
adb pull /sdcard/freq_count.txt %Folder%/freq_count.txt
adb pull /sdcard/log.txt %Folder%/log.txt
adb pull /sdcard/log.csv %Folder%/log.csv
 
:: ======================V9R1 LOG===========================================
:: get ap-log in v9r1
adb shell ls /data/ | findstr /X ap-log
if %errorlevel% EQU 0 (
  echo ####get /data/ap-log to .\ap-log ...
  adb pull  /data/ap-log/  %Folder%/ap-log/
)
:: get modem log in v9r1
adb shell ls /data/ | findstr /X cp-log
if %errorlevel% EQU 0 (
  echo ####get /data/cp-log to .\cp-log ...
  adb pull /data/cp-log/ %Folder%/cp-log/
)
:: get hifi log in v9r1
adb shell ls /data/ | findstr /X hifi-log
if %errorlevel% EQU 0 (
  echo ####get /data/hifi-log to .\hifi-log ...
  adb pull  /data/hifi-log/ %Folder%/hifi-log/
)
:: get klog in v9r1
adb shell ls /data/ | findstr /X klog
if %errorlevel% EQU 0 (
    echo ####get /data/klog to .\klog
    adb pull   /data/klog/      %Folder%/klog/
)
adb shell ls /data/ | findstr /X memorydump
if %errorlevel% EQU 0 (
    echo ####get /data/memorydump to .\memorydump
    adb pull   /data/memorydump/    %Folder%/memorydump/
)
 
echo ####get last kmsg to last_k*** ...
adb pull   /proc/last_kirq             %Folder%\proc\last_kirq
adb pull   /proc/last_kmsg             %Folder%\proc\last_kmsg
adb pull   /proc/last_ktask            %Folder%\proc\last_ktask
adb shell ls /data/ | findstr /X modemlog
if %errorlevel% EQU 0 (
    echo ####get modemlog to ./mlog_0 ...
    adb pull   /data/modemlog/sciRecord1.txt    %Folder%/mlog_0/sciRecord1.txt
    adb pull   /data/modemlog/sciRecord0.txt    %Folder%/mlog_0/sciRecord0.txt
    adb pull   /data/modemlog/DrvLog/Sim0        %Folder%/mlog_0/Sim0
    adb pull   /data/modemlog/DrvLog/Sim1        %Folder%/mlog_0/Sim1
    adb pull   /mnvm3:0/DrvLog/sciRecord1.txt         %Folder%/mlog_0/sciRecord1.txt
    adb pull   /mnvm3:0/DrvLog/sciRecord0.txt         %Folder%/mlog_0/sciRecord0.txt
    adb pull   /mnvm3:0/DrvLog/Sim0        %Folder%/mlog_0/Sim0
    adb pull   /mnvm3:0/DrvLog/Sim1        %Folder%/mlog_0/Sim1
)
::echo ####get nvme form system ...
::adb pull   /mnvm3:0/NvimDef            %Folder%/mnvm3_0_NvimDef
::adb pull   /mnvm1:0                    %Folder%/mnvm1_0
::adb pull   /mnvm2:0                    %Folder%/mnvm2_0
::adb pull   /3rdmodem/Nvxml             %Folder%/3rdmodem_Nvxml
 
::=======================common logs ========================================
::
echo ####get RunningTestII log to .\RunningTestII
adb pull    /data/log/RunningTestII                  %Folder%/RunningTestII
adb pull    /data/data/com.huawei.runningtestii/shared_prefs    %Folder%/RunningTestII_shared_prefs
adb shell ls /data/ | findstr /X adb
if %errorlevel% EQU 0 (
    echo ####get adb log to .\adb-log
    adb pull  /data/adb           %Folder%/adb-log
)
echo ####get radio logcat to logcat_ril.txt ...
adb shell  logcat -v threadtime -d -t 1000 -b radio > %Folder%\logcat_ril.txt
echo ####get AT logcat to logcat_at.txt ...
adb shell  logcat -v threadtime -d -t 1000 -b radio -s AT > %Folder%\logcat_at.txt
echo ####get bugreport to .\bugreport.txt
adb bugreport > %Folder%\bugreport.txt
echo ####Capture current screen to .\log.png
adb shell /system/bin/screencap -p /sdcard/log.png
adb pull /sdcard/log.png %Folder%/log.png
 
set del_opt=%1
 
if "%del_opt:~0,4%"=="/del" (
    set del_log=y
    echo      with [/del] options
) else (
    set /p del_log="Need to del all log(y/n)??(n)"
)
if "%del_log:~0,1%"=="y" (
echo warning delete logs .......
adb shell "rm -rf /data/android_logs/*"
adb shell "rm -rf /data/dontpanic/*"
adb shell "rm -rf /data/system/dropbox/*"
adb shell "rm -rf /data/tombstones/*"
adb shell "rm -rf /data/anr/*"
adb shell "rm -rf /data/ap-log/*"
adb shell "rm -rf /data/cp-log/*"
adb shell "rm -rf /data/hifi-log/*"
adb shell "rm -rf /data/log/archive/*"
adb shell "rm -rf /data/log/backup/*"
adb shell "rm -rf /sdcard/log/archive/*"
adb shell "rm -rf /sdcard/log/backup/*"
adb shell "rm -rf /sdcard/log/modem/balong/*"
adb shell "rm /sdcard/log/modem/balong_track.txt"
adb shell "rm /data/klog/*.log"
adb shell "rm -rf /data/memorydump/*"
adb shell "rm -rf /sdcard/log/manual-AP/*"
adb shell "rm -rf /data/adb/*"
adb shell "rm -rf /data/dumplog/*"
adb shell "rm -rf /data/rdr/*"
adb shell "rm -rf /data/factory-modemlog/*"
adb shell "rm -rf /sdcard/log.png"
adb shell "rm -rf /data/sec_storage/*"
adb shell "rm -rf /sdcard/LogService/*"
adb shell "rm -rf /storage/sdcard1/LogService/*"
)
 
echo ===============get log end; please check folder=%Folder%=========================
:END
pause
@echo on