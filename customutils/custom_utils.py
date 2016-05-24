#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2016年5月11日
@author: zhang.xiuhai
'''

from __future__ import with_statement
import os
from dateutil.relativedelta import *
from dateutil.easter import *
from dateutil.rrule import *
from dateutil.parser import *
from datetime import *
import calendar
import time
import config
import ConfigParser

def test():
    NOW = datetime.now()
    TODAY = date.today()

def launch_local_exe(default=None):
    os.system(default)

def custom_dir(name=None):
    if name == None:
        dir_name = date.today()
        if os.path.exists(config.GLOBAL_LOG+str(dir_name)):
            print "have existed."
        else:
            tmp = os.makedirs(config.GLOBAL_LOG+str(dir_name))
            if tmp:
                print "mkdir failed."
    elif os.path.exists(config.GLOBAL_LOG+name):
        pass
    else:
        os.makedirs(config.GLOBAL_LOG+name)

def check_dir(name):
    if name == None:
        pass
    elif os.path.exists(name):
        print "have existed."
    else:
        os.makedirs(name)

def test_config():
    config =  ConfigParser.ConfigParser()
    with open("../cuscfg.cfg", "r+") as cfgfile:
        config.readfp(cfgfile)
        name = config.get("info", "name")
        age = config.get("info", "age")
        print name, age
        config.set("info", "name", "zxh")
        config.set("info", "sex", "female")
        config.set("info", "age", "28")
        name = config.get("info", "name")
        sex = config.get("info", "sex")
        age = config.get("info", "age")
        print name, sex, age

if __name__ == '__main__':
    launch_cmd = "E:\Appium\node.exe E:\Appium\node_modules\appium\bin\appium.js -a 192.168.20.114 -p 4723 --no-reset"
    # now = parse("Fri May 20 17:13:46 UTC 2016")
    # today = now.date()
    # year = rrule(YEARLY,dtstart=now,bymonth=8,bymonthday=13,byweekday=FR)[0].year
    # rdelta = relativedelta(easter(year), today)
    # print("Today is: %s" % today)
    # print("Year with next Aug 13th on a Friday is: %s" % year)
    # print("How far is the Easter of that year: %s" % rdelta)
    # print("And the Easter of that year is: %s" % (today+rdelta))

    #获取当前时间

    print datetime.now()
    print date.today()
    print str(date.today())+"-"+str(datetime.now().hour)
    print str(date.today())+"-"+str(datetime.now().hour)+ "-"+str(datetime.now().minute)
    custom_dir("zhangxiuhai")
    print datetime.now()+relativedelta(months=+1) # print now time in next month.
    print str(int(time.mktime(time.strptime(time.ctime(), "%a %b %d %H:%M:%S %Y"))))
    APPIUM_RUNLOG = "D:\\Logs\\appium-runlog\\appium-runlog-"+str(date.today())+"-"+str(datetime.now().hour)+ "-"+str(datetime.now().minute)+"\\"
    check_dir(APPIUM_RUNLOG)
    test_config()