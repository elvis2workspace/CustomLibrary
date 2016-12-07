# -*- coding: utf-8 -*-

import time
import unittest

from selenium import webdriver
from selenium.webdriver.common.keys import Keys

from elvis.utils.os_opt.file_action import *


class Gps_Cloud(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Chrome()
        self.driver.implicitly_wait(30)
        self.login_url = "http://gps.dev-ag.56qq.com/login.html"
        self.verificationErrors = []
        self.accept_next_alert = True

    # def test_gpscloud_login(self):
    #     u"""gps cloud login"""
    #
    #     driver = self.driver
    #     driver.get(self.base_url)
    #     try:
    #         driver.find_element_by_name("userName").send_keys("gpstest")
    #         pwd_elem = driver.find_element_by_id("JS_password")
    #         pwd_elem.send_keys("123123")
    #         pwd_elem.send_keys(Keys.ENTER)
    #         time.sleep(5)
    #     except:
    #         driver.get_screenshot_as_file("D:\\selenium_use_case\\error_png\\kw.png")  # 如未找到元素就截取当前页
    #
    #     finally:
    #         driver.close()

    # def test_gps_car(self):
    #     driver = self.driver
    #     driver.get(self.login_url)
    #     driver.maximize_window()
    #     try:
    #         # LOGIN GPS CLOUD PLATFORM
    #         driver.find_element_by_name("userName").send_keys("gpstest")
    #         pwd_elem = driver.find_element_by_id("JS_password")
    #         pwd_elem.send_keys("123123")
    #         pwd_elem.send_keys(Keys.ENTER)
    #         time.sleep(5)
    #
    #         nav_carManage = driver.find_element_by_xpath("//*[@id=\"nav_carManage\"]")
    #
    #         # rtn = driver.execute_script('arguments[0].click()', more)
    #         chain = ActionChains(driver)
    #         chain.move_to_element(nav_carManage).perform()
    #         driver.find_element_by_xpath("//*[@id=\"subnav_carManage\"]/li[1]").click()
    #
    #     except:
    #         driver.get_screenshot_as_file("D:\\kw.png")  # 如未找到元素就截取当前页
    #     # finally:
    #     #     # driver.close()
    #     #     pass

    def test_gps_car_hide(self):
        driver = self.driver
        driver.get(self.login_url)
        driver.maximize_window()
        try:
            # LOGIN GPS CLOUD PLATFORM
            driver.find_element_by_name("userName").send_keys("gpstest")
            pwd_elem = driver.find_element_by_id("JS_password")
            pwd_elem.send_keys("123123")
            pwd_elem.send_keys(Keys.ENTER)
            time.sleep(5)

            nav_carManage = driver.find_element_by_xpath("//*[@id=\"nav_carManage\"]")

            # rtn = driver.execute_script('arguments[0].click()', more)
            js = "var q=document.getElementById('subnav_carManage');q.style.display='block';"
            #  setAttribute(\"style\",\"display:block\
            driver.execute_script(js)
            driver.find_element_by_css_selector("#subnav_carManage > li:nth-child(1)").click()
            time.sleep(5)

            # 将页面滚动条拖到底部
            # js = "var q=document.documentElement.scrollTop|| window.pageYOffset || document.body.scrollTop; q=10000;"
            js = "window.scrollTo(0, document.body.scrollHeight)"
            driver.execute_script(js)
            time.sleep(3)
            print "page bottom"

            # # 移动到元素element对象的“顶端”与当前窗口的“顶部”对齐
            #
            # driver.execute_script("arguments[0].scrollIntoView();", element)
            #
            # driver.execute_script("arguments[0].scrollIntoView(true);", element)
            #
            # # 移动到元素element对象的“底端”与当前窗口的“底部”对齐
            #
            # driver.execute_script("arguments[0].scrollIntoView(false);", element)
            #
            # # 移动到页面最底部
            #
            # driver.execute_script("window.scrollTo(0, document.body.scrollHeight)")
            #
            # # 移动到指定的坐标(相对当前的坐标移动)
            #
            # driver.execute_script("window.scrollBy(0, 700)")
            # # Thread.sleep(3000)
            # # 结合上面的scrollBy语句，相当于移动到700 + 800 = 1600像素位置
            #
            # driver.execute_script("window.scrollBy(0, 800)")
            #
            # # 移动到窗口绝对位置坐标，如下移动到纵坐标1600像素位置
            #
            # driver.execute_script("window.scrollTo(0, 1600)")
            # # Thread.sleep(3000);
            # #  结合上面的scrollTo语句，仍然移动到纵坐标1200像素位置
            #
            # driver.execute_script("window.scrollTo(0, 1200)")

            #
            # # 将滚动条移动到页面的顶部
            # js = "var q=document.documentElement.scrollTop=0"
            # driver.execute_script(js)
            # time.sleep(3)
            # print "page top"

            # chains = ActionChains(driver)
            # chains.send_keys(Keys.PAGE_DOWN).perform()
            # print "bottom."

            driver.find_element_by_id("carList_export").click()

            # driver.find_element_by_xpath("//*[@id=\"subnav_carManage\"]/li[1]").click()

            check_file(download_path, u"车辆基础信息-20161027101240.xls")

        except:
            driver.get_screenshot_as_file("D:\\kw.png")  # 如未找到元素就截取当前页
            # finally:
            #     # driver.close()
            #     pass

    def tearDown(self):
        # self.driver.quit
        self.assertEqual([], self.verificationErrors)


if __name__ == "__main__":
    suite = unittest.TestSuite()
    # suite.addTest(Gps_Cloud("test_gps_car"))
    suite.addTest(Gps_Cloud("test_gps_car_hide"))

    # 这里可以添加更多的用例,如：
    # suite.addTest(Youdao("aaaa"))
    unittest.TextTestRunner().run(suite)
