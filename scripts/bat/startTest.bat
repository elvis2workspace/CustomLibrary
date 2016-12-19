@echo off & setlocal EnableDelayedExpansion
adb devices>sn.txt

for %%a in (sn.txt) do (
for /f "skip=1 tokens=*" %%i in (%%a) do echo %%i>>sn2.txt
)

del /q /f sn.txt

for /f "delims=" %%i in (sn2.txt) do (
set ret=
for %%a in (%%i) do if defined %%a (set ret=!ret!!%%a!) else set ret=!ret! %%a 
if not "!ret!"=="" (set ret=!ret:device=! && echo !ret!) else echo.)>>sn3.txt


del /q /f sn2.txt

For /f "tokens=1* delims=:" %%i in ('Type sn3.txt^|Findstr /n ".*"') do (
If "%%i"=="1" Set str1=%%j
If "%%i"=="2" Set str2=%%j
)


del /q /f sn3.txt

set "str1=%str1: =%"
set "str2=%str2: =%"
start rebootTest.bat %str1%
start rebootTest.bat %str2%