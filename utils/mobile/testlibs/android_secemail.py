#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''


@author: zhangxiuhai
'''
import os
import unittest
from time import sleep

from common.tools import check_appium

import DriverInfo

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)


class SeceMailTest(unittest.TestCase):
    def setUp(self):
        try:
            check_appium()
        except:
            print "Take attention please, check and initial test_selenium2library environment failed!"
        else:
            pass
        
        driverInfo = DriverInfo.Driver()
#         desired_caps={}
#         desired_caps['platformName'] = 'Android'
#         desired_caps['platformVersion'] = '4.4'
#         desired_caps['deviceName'] = 'Nubia'
#         desired_caps['app'] = PATH(
#             '../../../mgAuto/apps/SeceMail/secemail.apk'
#         )
#         
#         
#         #
#         desired_caps['appPackage'] = 'com.cetc30.email'
#         desired_caps['appActivity'] = 'com.cetc30.email.activity.ComposeActivityEmail'
#         desired_caps['unicodeKeyboard'] = 'true'
#         desired_caps['resetKeyboard'] = 'true'
# 
#         self.driver = webdriver.Remote('http://127.0.0.1:4723/wd/hub', desired_caps)
        self.driver = driverInfo.getAndroidDriverInfo()
    def tearDown(self):
        self.driver.quit()
    
    def test_add_account(self):
#         el = self.driver.find_element_by_name('CREATE_ACCOUNT')
#         el.click()
        
#         self.driver.find_elements_by_class_name("android.widget.ImageButton").click()
#         
#         self.driver.find_element_by_accessibility_id('android:id/title').click()
#         self.driver.find_element_by_accessibility_id('com.cetc30.email:id/add_new_account').click()
        
        textfield = self.driver.find_element_by_id("com.cetc30.email:id/account_email")
        textfield.send_keys("2583773475@qq.com")
        self.assertEqual('2583773475@qq.com', textfield.text.decode('utf-8'))
        
        mpassword = self.driver.find_element_by_id("com.cetc30.email:id/account_password")
        mpassword.send_keys("zhangxiuhai1988")
        
        
#         self.assertEqual('secemail', textfields[1].text)

        self.driver.find_element_by_id('com.cetc30.email:id/next').click()
        
        sleep(2)
        
        self.driver.find_element_by_id('com.cetc30.email:id/next').click()
        
        textfield1 = self.driver.find_element_by_id('com.cetc30.email:id/account_description')
        
        self.assertEqual('2583773475@qq.com', textfield1.text.decode('utf-8'))
        
        textfield2 = self.driver.find_element_by_id('com.cetc30.email:id/account_name')
        
        textfield2.send_keys("secemail")
        
        self.assertEqual("secemail", textfield2.text.decode('utf-8'))
        
        sleep(2)
        
        self.driver.find_element_by_id('com.cetc30.email:id/next').click()
        
#         textview = self.driver.find_element_by_id('com.cetc30.email:id/account_check_frequency')
        
#         self.assertEqual('每锟斤拷15锟斤拷锟斤拷', textview.text().decode('utf-8'))
        
#         self.driver.find_element_by_id('com.cetc30.email:id/next').click()
# 
#         self.driver.find_element_by_class_name("android.widget.Button").click()
#         
#         self.driver.find_element_by_id("com.cetc30.email:id/next").click()
#         
#         account_name = self.driver.find_element_by_id("com.cetc30.email:id/account_name")
#         account_name.send_keys('secemail')
        
        # for some reason "save" breaks things
        alert = self.driver.switch_to_alert()

        # no way to handle alerts in Android
        self.driver.find_element_by_android_uiautomator('new UiSelector().clickable(true)').click()

        self.driver.keyevent(3)


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    
    suite = unittest.TestLoader().loadTestsFromTestCase(SeceMailTest)
    unittest.TextTestRunner(verbosity=2).run(suite)