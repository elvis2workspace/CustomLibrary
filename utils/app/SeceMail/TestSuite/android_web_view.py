#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月7日

@author: zhangxiuhai
'''
import unittest
from appium import webdriver


class Test(unittest.TestCase):


    def setUp(self):
        desired_caps = {
            'device': 'selendroid',
            'app': 'D:\\workspace\\sample-code\\apps\\selendroid-test-app.apk',
            'app-package': 'io.selendroid.testapp',
            'app-activity': '.HomeScreenActivity',
            'platformName':'Android',
            'deviceName':'Android Emulator'
        }

        self.driver = webdriver.Remote('http://localhost:4723/wd/hub',desired_caps)


    def tearDown(self):
        self.driver.quit()


    def testName(self):
        button = self.driver.find_element_by_name('buttonStartWebviewCD')
        button.click()

        context_name = "WEBVIEW_1"
        self.driver.switch_to.context(context_name)

        input_field = self.driver.find_element_by_id('name_input')
        input_field.send_keys('Mathieu')
        input_field.submit()


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()