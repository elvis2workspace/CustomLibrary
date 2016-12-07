# -*- coding: utf-8 -*-
# !/usr/bin/env python

import time
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

print "开始"
print "......"

# 实例化一个驱动类
profiledir = webdriver.FirefoxProfile(r"/Users/sunying/Library/Application Support/Firefox/Profiles/sr6smerq.default")

# 打开火狐浏览器
driver = webdriver.Firefox(profiledir)
# 登录监控宝
driver.get("http://www.jiankongbao.com")
driver.find_element_by_id("dropdown-signin").click()
driver.find_element_by_id("email").clear()
driver.find_element_by_id("email").send_keys("**@yunzhihui.com")
driver.find_element_by_id("pwd").clear()
driver.find_element_by_id("pwd").send_keys("*** ")
driver.find_element_by_id("sigin_btn").click()
time.sleep(3)
driver.close()
driver.quit()

print "结束"
