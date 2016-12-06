#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月7日

@author: zhangxiuhai
'''
import os

from app.AndroidTools.common import check_appium, check_app

# Returns abs path relative to this file and not cwd
PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)

try:
    check_appium()
    driver = check_app('selendroid-test-app.apk')
except:
    os.sys.exit()
else:
    print "Appium have lauched successfully!"

# desired_caps = {}
# desired_caps['platformName'] = 'Android'
# desired_caps['platformVersion'] = '4.4'
# desired_caps['deviceName'] = 'Android Emulator'
# desired_caps['device'] = 'android'
# desired_caps['browserName'] = ''
# desired_caps['app-package'] = 'com.android.calculator2'
# desired_caps['app-activity'] = '.Calculator'
# desired_caps['app'] = PATH('../../../sample-code/apps/selendroid-test-app.apk')

# driver = webdriver.Remote('http://localhost:4723/wd/hub', desired_caps)

# 点击计算器上的数字和运算符
el = driver.find_element_by_name("1")
el.click()
el = driver.find_element_by_name("5")
el.click()
el = driver.find_element_by_name("9")
el.click()
el = driver.find_element_by_name("delete")
el.click()
el = driver.find_element_by_name("9")
el.click()
el = driver.find_element_by_name("5")
el.click()
el = driver.find_element_by_name("+")
el.click()
el = driver.find_element_by_name("6")
el.click()
el = driver.find_element_by_name("=")
el.click()

driver.quit()