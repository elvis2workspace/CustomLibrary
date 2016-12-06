# -*- coding: utf-8 -*-

import time

from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.keys import Keys

ctm_dr = webdriver.Chrome()
ctm_dr.get("http://gps.dev-ag.56qq.com/login.html")
ctm_dr.maximize_window()
ctm_dr.implicitly_wait(10)

user_elem = ctm_dr.find_element_by_name("userName")
user_elem.clear()
time.sleep(5)
user_elem.send_keys("gpstest")

# Input password
pwd_elem = ctm_dr.find_element_by_name("password")
pwd_elem.send_keys("123123")

pwd_elem.send_keys(Keys.ENTER)

# click hide ul
ctm_achain = ActionChains(ctm_dr)
ul_elem = ctm_dr.find_element_by_xpath("//*[@id='JS_left_navs']/div[2]/span")
ctm_achain.move_to_element(ul_elem).perform()
ctm_dr.find_element_by_xpath("//*[@id='JS_left_navs']/div[2]/span/ul").click()

# check进入公司车辆管理信息界面

# check 车辆管理信息界面正常清晰无异常（table检查）

# 执行js 鼠标悬浮到元素打开隐藏按钮
mouse_on_js = "var evObj = document.createEvent('MouseEvents');" + \
              "evObj.initMouseEvent(\"mouseover\",true, false, window, 0, 0, 0, 0, 0, false, false, false, false, 0, " \
              "null);" + "arguments[0].dispatchEvent(evObj);"
ctm_dr.execute_script(mouse_on_js)

time.sleep(3)

# ctm_dr.quit()
