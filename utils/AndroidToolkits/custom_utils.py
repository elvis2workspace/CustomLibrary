#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Created on 2016年5月11日
@author: zhang.xiuhai
"""

from __future__ import with_statement

import platform
import subprocess
import time

import re
from dateutil.easter import *
from dateutil.parser import *
from dateutil.relativedelta import *
from dateutil.rrule import *
from pychartdir import *
from robot.api import logger

from utils.exception_utils import custom_exception
from config.config import *

serial_number = ""

# 判断系统类型，windows使用findstr，linux使用grep
system = platform.system()
if system is "Windows":
    find_util = "findstr"
else:
    find_util = "grep"

# 判断是否设置环境变量ANDROID_HOME
if "ANDROID_HOME" in os.environ:  # os.environ 返回字典
    if system == "Windows":
        command = os.path.join(os.environ["ANDROID_HOME"], "platform-tools", "adb.exe")
    else:
        command = os.path.join(os.environ["ANDROID_HOME"], "platform-tools", "adb")
else:
    raise EnvironmentError(
        "Adb not found in $ANDROID_HOME path: %s." % os.environ["ANDROID_HOME"])


# 启动本地可执行程序
def launch_local_exe(default=None):
    if default is None:
        print "Debug: "
    else:
        os.system(default)


# make custom directory, can specify the directory name
def custom_dir(name=None):
    if name is None:
        dir_name = date.today()
        if os.path.exists(GLOBAL_LOG+str(dir_name)):
            print "have existed."
        else:
            if os.makedirs(GLOBAL_LOG+str(dir_name)) is not None:
                print "Create dir failed."
    elif os.path.exists(GLOBAL_LOG+name):
        pass
    else:
        os.makedirs(GLOBAL_LOG+name)


# to check the "name" directory whether exist or not.
def check_dir(name):
    if name is None:
        pass
    elif os.path.exists(name):
        print "%s have existed." % name
    else:
        os.makedirs(name)


# 获取设备当前应用的包名和Activity
def get_current_surface_package_activity():
    pattern = re.compile(r"[a-zA-Z0-9\.]+/.[a-zA-Z0-9\.]+")
    out = adb_shell("dumpsys window w | %s \/ | %s name=" %(find_util, find_util)).stdout.read()

    return pattern.findall(out)[0]


# 封装所有应用到的adb shell命令，支持多设备需指定设备uuid, default run adb shell command.
def adb_shell(args, by_shell=True):
    global serial_number
    if serial_number == "":
        devices = get_devices_list()
        if len(devices) == 1:
            # setup global serial_number
            serial_number = devices[0]
        else:
            print "waiting......."
            # 处理多设备显示 待实现
            # root = tk.TK()
            # window = Window(devices, root)
            # window.display_window()
    if by_shell:
        cmd = "%s -s %s shell %s" %(command, serial_number, str(args))
    else:
        cmd = "%s -s %s %s" %(command, serial_number, str(args))
    ret = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print "start process debug(Popen.poll): ", ret.poll()
    while True:
        bufsize = ret.stdout.readline()
        if bufsize == "" and ret.poll() is not None:
            print "end process debug(Popen.poll): ", ret.poll()
            break
        # print bufsize


# 获取当前连接的终端设备列表
def get_devices_list():
    import subprocess
    devices = []
    result = subprocess.Popen("adb devices", shell=True, stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE).stdout.readlines()
    result.reverse()
    for line in result[1:]:
        if "attached" not in line.strip():
            devices.append(line.split()[0])
        else:
            break
    return devices


# 获取当前连接设备状态
def get_state():
    return os.popen("adb -s %s get-state" % serial_number).read().strip()


# 时间戳
def timestamp():
    return time.strftime("%Y-%m-%d-%H-%M-%S", time.localtime(time.time()))


# 获取对应包名的pid
def get_app_pid(package_name):
    re_string = adb_shell("ps | %s -w %s" % (find_util, package_name)).stdout.read()

    if string == '':
        return "the process doesn't exist."

    pattern = re.compile(r"\d+")
    result = re_string.split(" ")
    result.remove(result[0])

    return pattern.findall(" ".join(result))[0]


# 杀掉对应包名的进程
def kill_process(package_name):
    pid = get_app_pid(package_name)
    result = adb_shell("kill %s" % str(pid)).stdout.read().split(": ")[-1]

    if result != "":
        raise custom_exception.CustomException("Operation not permitted or No such process")


# 打开待测应用，运行脚本，默认times为20次（可自己手动修改次数），获取该应用cpu、memory占用率的曲线图，图表保存至chart目录下
# 获取cpu、mem占用
def get_cpu_mem_info():
    cpu = []
    mem = []
    # top次数
    times = 20
    # 设备当前运行应用的包名
    pkg_name = get_current_surface_package_activity().split("/")[0]

    top_info = adb_shell("top -n %s | %s %s$" %(str(times), find_util, pkg_name)).stdout.readlines()

    for info in top_info:
        # temp_list = del_space(info)
        temp_list = info.split()
        cpu.append(temp_list[2])
        mem.append(temp_list[6])

    return cpu, mem


# 去除top信息中的空格，便于获取cpu、mem的值
def del_space(str_sec):
    temp_list1 = str_sec.split(" ")
    temp_list2 = []

    for s in temp_list1:
        if s != "":
            temp_list2.append(s)

    return temp_list2


# 绘制线性图表，具体接口的用法查看ChartDirector的帮助文档
def line_chart():
    times = 20
    import string
    data = get_cpu_mem_info()
    cpu_data = []
    mem_data = []
    # 设备当前运行应用的包名
    pkg_name = get_current_surface_package_activity().split("/")[0]

    # 去掉cpu占用率中的百分号，并转换为int型
    for cpu in data[0]:
        cpu_data.append(string.atoi(cpu.split("%")[0]))

    # 去掉内存占用中的单位K，并转换为int型，以M为单位
    for mem in data[1]:
        mem_data.append(string.atof(mem.split("K")[0])/1024)

    # 横坐标
    labels = []
    for i in range(1, times + 1):
        labels.append(str(i))

    # 自动设置图表区域宽度
    if times <= 50:
        x_area = times * 40
    elif 50 < times <= 90:
        x_area = times * 20
    else:
        x_area = 1800

    c = XYChart(x_area, 800, 0xCCEEFF, 0x000000, 1)
    c.setPlotArea(60, 100, x_area - 100, 650)
    c.addLegend(50, 30, 0, "arialbd.ttf", 15).setBackground(Transparent)

    c.addTitle("cpu and memory info(%s)" % pkg_name, "timesbi.ttf", 15).setBackground(0xCCEEFF, 0x000000, glassEffect())
    c.yAxis().setTitle("The numerical", "arialbd.ttf", 12)
    c.xAxis().setTitle("Times", "arialbd.ttf", 12)

    c.xAxis().setLabels(labels)

    # 自动设置X轴步长
    if times <= 50:
        step = 1
    else:
        step = times / 50 + 1

    c.xAxis().setLabelStep(step)

    layer = c.addLineLayer()
    layer.setLineWidth(2)
    layer.addDataSet(cpu_data, 0xff0000, "cpu(%)")
    layer.addDataSet(mem_data, 0x008800, "mem(M)")

    path = PATH(r"../res/chart")
    if check_dir(path) is not None:
        return 1

    # 图片保存至脚本当前目录的chart目录下
    c.makeChart(PATH("../res/chart/%s.png" % timestamp()))


def upload_file(sour_file=None, dest_path=None):
        if os.path.exists(sour_file):
            push_command = "push " + sour_file + " " + dest_path
            print push_command
            adb_shell(push_command, by_shell=False)
            logger.info("%s succeed to be uploaded to %s" % (sour_file, dest_path), html=True, also_console=True)
        else:
            logger.error("%s is not exist." % sour_file)
            return -1


class MuffledCalculator:
    def __init__(self):
        self.muffled = False  # default closed.

    def calc(self, expr):
        try:
            return eval(expr)
        except ZeroDivisionError:
            if self.muffled:
                print 'Division by zero is illegal.'
            else:
                raise

if __name__ == '__main__':
    launch_cmd = "E:\Appium\node.exe E:\Appium\node_modules\appium\bin\appium.js -a 192.168.20.114 -p 4723 --no-reset"
    now = parse("Fri May 20 17:13:46 UTC 2016")
    today = now.date()
    year = rrule(YEARLY,dtstart=now,bymonth=8,bymonthday=13,byweekday=FR)[0].year
    rdelta = relativedelta(easter(year), today)
    print("Today is: %s" % today)
    print("Year with next Aug 13th on a Friday is: %s" % year)
    print("How far is the Easter of that year: %s" % rdelta)
    print("And the Easter of that year is: %s" % (today+rdelta))

    # 获取当前时间
    print datetime.now()
    print date.today()
    cur = datetime.now()
    print cur
    print cur.strftime('%Y%m%d')
    print datetime.now().strftime('%Y%m%d%H%M%S')

    print str(date.today())+"-"+str(datetime.now().hour)
    print str(date.today())+"-"+str(datetime.now().hour) + "-"+str(datetime.now().minute)
    custom_dir("zhangxiuhai")
    print datetime.now()+relativedelta(months=+1)    # print now time in next month.
    print str(int(time.mktime(time.strptime(time.ctime(), "%a %b %d %H:%M:%S %Y"))))
    tmp_path = "D:\\PS_auto_project\Logs\\appium-runlog\\appium-runlog-"+str(date.today())+"-"+str(datetime.now().hour) + \
               "-" + str(datetime.now().minute)+"\\"
    check_dir(tmp_path)
    calculator = MuffledCalculator()
    calculator.muffled = True
    calculator.calc('10/2')
    calculator.calc('10/0')

