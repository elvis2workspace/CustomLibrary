# -*- coding: utf-8 -*-

from time import sleep
from selenium import webdriver

driver = webdriver.Chrome()
driver.get("http://www.baidu.com")
assert "百度一下，你就知道".decode('utf-8') in driver.title
sleep(3)
driver.find_element_by_id('kw').send_keys("hello, Webdriver !")
driver.find_element_by_id('su').click()
print "driver.title: ", driver.title
# driver.close()
