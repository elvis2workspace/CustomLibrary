#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月16日

@author: zhang.xiuhai
'''

from CustomLibrary.customkeywords import _custom_android_utils
import time

#提前启动appium工具
tmpObject = _custom_android_utils._CustomAndroidKeywords()
ret_str = tmpObject.launch_local_appium("192.168.20.114")

print "ret_str: ", ret_str
time.sleep(20)