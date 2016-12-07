# -*- coding: utf-8 -*-

import time
import unittest

from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities


class Baidu(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(
            command_executor='http://192.168.20.247:4444/wd/hub',
            desired_capabilities=DesiredCapabilities.CHROME
        )
        # self.driver = webdriver.Chrome()
        self.driver.implicitly_wait(30)
        self.base_url = "http://www.baidu.com"
        self.verificationErrors = []
        self.accept_next_alert = True

    # 百度搜索用例
    def test_baidu_search(self):
        u"""百度搜索测试用例"""

        driver = self.driver
        driver.get(self.base_url + "/")

        try:
            # 是一个无法找到的元素 id
            driver.find_element_by_id("kw").send_keys("selenium webdriver")
        except:
            driver.get_screenshot_as_file("D:\\selenium_use_case\\error_png\\kw.png")
            # 如果没有找到上面的元素就截取当前页面。

        driver.find_element_by_id("su").click()
        time.sleep(5)
        driver.close()

    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


if __name__ == "__main__":
    suite = unittest.TestSuite()
    suite.addTest(Baidu("test_baidu_search"))

    # 同样的，可以在这个文件中添加更多的用例。
    # suite.addTest(Youdao("aaaa"))
    results = unittest.TextTestRunner().run(suite)
