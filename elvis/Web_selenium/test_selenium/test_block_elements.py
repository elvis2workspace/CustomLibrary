# -*- coding: utf-8 -*-

import os
import time

from selenium import webdriver
from selenium.webdriver.support.select import Select

driver = webdriver.Chrome()
file_path = 'file:///' + os.path.abspath('test_block.html')
driver.get(file_path)

js = 'document.querySelectorAll("select")[0].style.display="block";'
driver.execute_script(js)

sel = driver.find_element_by_tag_name('select')
Select(sel).select_by_value('audi')
time.sleep(10)

driver.quit()
