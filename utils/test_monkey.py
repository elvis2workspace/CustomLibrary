# -*- coding: utf-8 -*-

import os
import re
import time

packageName = "com.XXXX.app"
log_dir = r"d:\jenkins"
remote_path = r"\\10.21.101.100\build\android"
os.system('adb shell cat /system/build.prop > D:\jenkins\phone.text')
file_s = r"D:\jenkins\phone.text"

NullPointer = "java.lang.NullPointerException"
IllegalState = "java.lang.IllegalStateException"
IllegalArgument = "java.lang.IllegalArgumentException"
ArrayIndexOutOfBounds = "java.lang.ArrayIndexOutOfBoundsException"
RuntimeException = "java.lang.RuntimeException"
SecurityException = "java.lang.SecurityException"


def getcmd(file_p):
    file_tmp = open(file_p, "r")
    lines = file_tmp.readlines()
    for line in lines:
        line = line.split('=')

    if line[0] == 'ro.build.version.release':
        versions = line[1]
    if line[0] == 'ro.product.model':
        models = line[1]
    if line[0] == 'ro.product.brand':
        brands = line[1]

    return versions, models, brands

version, model, brand = getcmd(file_s)
print version, model, brand
os.remove(file_s)

# print "使用Logcat清空Phone中log"
os.popen("adb logcat -c")
# print"暂停2秒..."
print "wait"
time.sleep(2)

now1 = time.strftime('%Y-%m-%d-%H_%M_%S', time.localtime(time.time()))

# print"开始执行Monkey命令"
monkey_log = log_dir + "\\" + now1 + "monkey.log"
print monkey_log
cmd = "adb shell monkey -p com.XXXX -s 500 --ignore-timeouts --monitor-native-crashes -v -v 10000 >> %s" % monkey_log
os.popen(cmd)

# print"手机截屏"
os.popen("adb shell screencap -p /sdcard/monkey_run.png")

# print"拷贝截屏图片至电脑"
cmd1 = "adb pull /sdcard/monkey_run.png %s" % log_dir
os.popen(cmd1)

print "gai ming"
old_name = log_dir+"\\"+r"monkey_run.png"
if os.path.exists(old_name):
    print "file is exist"
else:
    print "file isn't exist"
new_name = log_dir+"\\" + now1 + r"monkey.png"
os.rename(old_name, new_name)

# print"使用Logcat导出日志"
logcat_name = log_dir+"\\" + now1 + r"logcat.log"
cmd2 = "adb logcat -d >%s" % logcat_name
os.popen(cmd2)

# print"导出traces文件"
traces_name = log_dir+"\\"+now1+r"traces.log"
cmd3 = "adb shell cat /data/anr/traces.txt>%s" % traces_name
os.popen(cmd3)


def get_error(logcat_name_file):
    f = open(logcat_name_file, "r")
    lines = f.readlines()
    err_file = "%s\error.log" % remote_path
    if os.path.exists(err_file):
        os.remove(err_file)

    fr = open(err_file, "a")
    fr.write(version)
    fr.write("\n")
    fr.write(model)
    fr.write("\n")
    fr.write(brand)
    fr.write("\n")
    fr.write(now1)
    fr.write("\n")
    count = 0
    for line in lines:
        if re.findall(NullPointer, line) or re.findall(IllegalState, line) or re.findall(IllegalArgument, line) or\
                re.findall(ArrayIndexOutOfBounds, line) or re.findall(RuntimeException, line) or \
                re.findall(SecurityException,line):
            a = lines.index(line)
            count += 1

    for var in range(a, a+22):
        print lines[var]
        fr.write(lines[var])
        fr.write("\n")
        f.close()
        fr.close()
    return count

number = get_error()
print number
