#coding = utf-8

import time
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

driver = webdriver.Remote(desired_capabilities=DesiredCapabilities.FIREFOX)
driver.get("http://www.baidu.com")
driver.find_element_by_name("q").send_keys("hello")
driver.find_element_by_name("q").send_keys("KEY.ENTER")

driver.close()