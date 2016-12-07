#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月10日

@author: zhangxiuhai
'''
from appium import webdriver
from common.tools import launch_appium

import desired_capabilities


class Driver():
    '''
    获取driver属性信息
    '''


    def __init__(self):
        '''
        Constructor
        '''
        
    def getAndroidDriverInfo(self):
        '''  定义获取driver属性信息的函数--安卓     '''

        desired_caps = desired_capabilities.get_desired_capabilities('SeceMail/secemail.apk')

        driver = webdriver.Remote('http://127.0.0.1:4723/wd/hub', desired_caps)
        print '\n for test',driver,'\n'
        return driver
    
    def iOSDriverInfo(self):
        pass
    
if __name__ == '__main__':
    launch_appium()
    driverInfo = Driver()
    driver = driverInfo.getAndroidDriverInfo()
    print driver.__in
    