
@echo off
call config.bat
if exist %hpInputFile% (
del %hpInputFile% /q
)
adb pull %hpInputFileDir%/%hpInputFile% .
if not exist %hpInputFile% (
echo fail to pull %hpInputFile%
exit 1
)
if not exist %hpRoot% (
md %hpRoot%
)
Setlocal enabledelayedexpansion
set path=%path%;%cd%\lib
call genSerial
set serial=!genSerial~result!
set hpOutFile=%serial%.hprof
%tools%\hprof-conv.exe %hpInputFile% %hpRoot%\%hpOutFile%
echo success!
endlocal
