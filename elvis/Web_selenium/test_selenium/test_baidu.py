# -*- coding: utf-8 -*-

import time
import unittest

from selenium import webdriver
from selenium.webdriver.common.keys import Keys


class Baidu(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Chrome()
        self.driver.implicitly_wait(10)
        self.base_url = "https://www.baidu.com"
        self.driver.get(self.base_url + "/")
        self.verificationErrors = []
        self.accept_next_alert = True
        self.driver.maximize_window()

    # # 百度搜索用例
    # def test_baidu_search(self):
    #     u"""百度搜索测试用例"""
    #     driver = self.driver
    #
    #     try:
    #         # 是一个无法找到的元素 id
    #         driver.find_element_by_id("kw").send_keys("selenium webdriver")
    #     except:
    #         driver.get_screenshot_as_file("E:\\selenium_use_case\\error_png\\kw.png")
    #         # 如果没有找到上面的元素就截取当前页面。
    #
    #     driver.find_element_by_id("su").click()
    #     time.sleep(5)
    #     driver.close()

    # baidu login
    # def test_baidu_login(self):
    #     u"""baidu login case"""
    #
    #     driver = self.driver
    #     try:
    #         driver.find_element_by_partial_link_text("登录").click()
    #     except:
    #         driver.get_screenshot_as_file("E:\\selenium_use_case\\error_png\\ul.png")
    #
    #     userName = driver.find_element_by_id("TANGRAM__PSP_8__userName")
    #     userName.clear()
    #     userName.send_keys(u"双鱼座a恋人")
    #     pwd = driver.find_element_by_id("TANGRAM__PSP_8__password")
    #     pwd.send_keys("zhangxiuhai")
    #     driver.find_element_by_id("TANGRAM__PSP_8__submit").click()

    # error
    def test_baidu_profile_setup(self):
        driver = self.driver
        try:
            ctitle = driver.title
            self.assertEqual(ctitle, u"百度一下，你就知道")
            search = driver.find_element_by_id("kw")
            search.send_keys("selenium webdriver")
            search.send_keys(Keys.ENTER)
            time.sleep(3)

            # 将页面滚动条拖到底部
            js = "var document.documentElement.scrollTop=10000"
            driver.execute_script(js)
            time.sleep(5)

            # 将页面滚动条拖到顶部
            js = "var q=document.documentElement.scrollTop=0"
            driver.execute_script(js)
            time.sleep(3)

        except:
            driver.get_screenshot_as_file("D:\ul.png")

    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


if __name__ == "__main__":
    suite = unittest.TestSuite()
    suite.addTest(Baidu("test_baidu_profile_setup"))

    # 同样的，可以在这个文件中添加更多的用例。
    # suite.addTest(Youdao("aaaa"))
    results = unittest.TextTestRunner().run(suite)
