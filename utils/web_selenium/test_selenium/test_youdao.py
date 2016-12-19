# -*- coding: utf-8 -*-

import unittest

from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains


class Youdao(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Chrome()
        self.driver.implicitly_wait(30)
        self.base_url = "http://www.youdao.com"
        self.verificationErrors = []
        self.accept_next_alert = True

    # def test_youdao_search(self):
    #     u"""有道测试搜索"""
    #
    #     driver = self.driver
    #     driver.get(self.base_url+"/")
    #     try:
    #         driver.find_element_by_id("query").send_keys(u"虫师")
    #         driver.find_element_by_id("qb").click()
    #         time.sleep(5)
    #     except:
    #         driver.get_screenshot_as_file("D:\\selenium_use_case\\error_png\\kw.png")  # 如未找到元素就截取当前页
    #
    #     finally:
    #         driver.close()

    def test_youdao_more(self):
        driver = self.driver
        driver.get(self.base_url + "/")
        driver.maximize_window()
        try:
            more = driver.find_element_by_xpath("//*[@id=\"more\"]")
            print more
            # rtn = driver.execute_script('arguments[0].click()', more)
            chain = ActionChains(driver)
            chain.move_to_element(more).perform()
            driver.find_element_by_partial_link_text(u"有道口语大师").click()
            # print "rtn:", rtn
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
    # suite.addTest(Youdao("test_youdao_search"))
    suite.addTest(Youdao("test_youdao_more"))

    # 这里可以添加更多的用例,如：
    # suite.addTest(Youdao("aaaa"))
    unittest.TextTestRunner().run(suite)
