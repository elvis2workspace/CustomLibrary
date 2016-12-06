# -*- coding: utf-8 -*-
import os
from time import sleep

from elvis.Web_selenium.test_selenium import webdriver

if 'HTTP_PROXY' in os.environ:
    del os.environ['HTTP_PROXY']

dr = webdriver.Chrome()
# file_path = 'file:///' + os.path.abspath('GPS管车-车辆监控.html')
file_path = 'file:///' + os.path.abspath('modal.html')
dr.get(file_path)

# 打开对话框
dr.find_element_by_id('show_modal').click()

sleep(3)

# 点击对话框中的链接
# 由于对话框中的元素被蒙板所遮挡，直接点击会报 Element is not clickable 的错误
# 所以使用js 来模拟click
link = dr.find_element_by_id('myModal').find_element_by_id('click')
dr.execute_script('$(arguments[0]).click()', link)

sleep(4)

# 关闭对话框
buttons = dr.find_element_by_class_name('modal-footer').find_elements_by_tag_name('button')
print buttons
buttons[0].click()
dr.quit()
