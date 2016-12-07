# -*- coding: utf-8 -*-

import os
import time

from selenium import webdriver

driver = webdriver.Chrome()
file_path = 'file:///' + os.path.abspath('js.html')
driver.get(file_path)

# 通过JS 隐藏选中的元素
# 第一种方法

driver.execute_script('$("#tooltip").fadeOut();')

time.sleep(5)

# 第二种方法
button = driver.find_element_by_class_name('btn')
driver.execute_script('$(arguments[0]).fadeOut()', button)
time.sleep(5)
driver.quit()


def call_js(browser):
    browser.find_element_by_xpath("//div[@id='surveyItemsWrap']/div/div[2]/button[2]").click()
    time.sleep(5)
    inputs = browser.find_element_by_xpath("//body/input")
    js = "var q=document.getElementByClassName('ts_bg alert');q.style.display='block';"
    #  setAttribute(\"style\",\"display:block\
    print "js"
    browser.execute_script(js)
    browser.find_element_by_xpath("//body/input").send_keys('F:\\Img\2.jpg')
    print "ok"


if __name__ == '__main__':
    call_js()
