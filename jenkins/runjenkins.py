#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月16日

@author: zhang.xiuhai
'''
from robot.api import logger
from CustomLibrary.customkeywords import _custom_android_utils

#提前启动appium工具
def lanchappiumbefore():
    tmpObject = _custom_android_utils._CustomAndroidKeywords()
    tmpObject.launch_local_appium("192.168.20.114")
    
if __name__ == '__main__':
    logger.info("[CUSTOM-JENKINS]: In the progresss of run jenkins, going to launch the appium tool.", also_console=True)
    lanchappiumbefore()