#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月1日

@author: zhang.xiuhai
'''


#测试报告
import os
import unittest,sys,time,re,datetime,HTMLTestRunner
from appium import webdriver
from time import sleep



# Returns abs path relative to this file and not cwd
PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)


class ContactsAndroidTests(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        print 'setUpClass'
        desired_caps = {}
        desired_caps['platformName'] = 'Android'
        desired_caps['platformVersion'] = '4.4'
        desired_caps['deviceName'] = '192.168.56.101:5555'
        desired_caps['appPackage'] = 'com.android.dialer'
        desired_caps['appActivity'] = '.DialtactsActivity'

        cls.driver = webdriver.Remote('http://127.0.0.1:4723/wd/hub', desired_caps)

    @classmethod
    def tearDownClass(cls):
        cls.driver.close_app()
        cls.driver.quit()
        print 'tearDownClass'

    def setUp(self):
        print "setup"

    def tearDown(self):
        print 'teardown'

    def test_add_contacts(self):
        print 1
        #def test_B(self):
        self.driver.find_element_by_id('com.android.dialer:id/call_history_button').click()

    def test_A(self):
        print 2
        self.driver.find_element_by_class_name("android.app.ActionBar$Tab").click()


if __name__ == '__main__':
    #unittest.main(exit=False)
    suite = unittest.TestSuite()
    suite.addTest(ContactsAndroidTests("test_add_contacts"))
    suite.addTest(ContactsAndroidTests("test_A"))
    #suite.addTest(IposCase("testmaters"))
    timestr = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
    filename = "D:\\appium\\appiumresult\\result_" + timestr + ".html"
    print (filename)
    fp = open(filename, 'wb')
    runner = HTMLTestRunner.HTMLTestRunner(
                stream=fp,
                title='测试结果',
                description='测试报告'
                )
    #suite = unittest.TestLoader().loadTestsFromTestCase(ContactsAndroidTests)
    #unittest.TextTestRunner(verbosity=2).run(suite)
    runner.run(suite)
    #g_browser.quit()
    fp.close() #测试报告关闭