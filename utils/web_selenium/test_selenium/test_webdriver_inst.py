# -*- coding:utf-8 -*-

import logging

from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

logging.basicConfig(level=logging.DEBUG)
driver = webdriver.Remote(desired_capabilities=DesiredCapabilities.CHROME)
driver.get("http://www.youdao.com")
driver.maximize_window()
driver.find_element_by_name("q").send_keys("hello")
driver.find_element_by_name("q").send_keys("key.ENTER")

driver.close()
