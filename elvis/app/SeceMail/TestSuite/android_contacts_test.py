#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月7日

@author: zhangxiuhai
'''
import unittest
import os
from appium import webdriver
from time import sleep
from tools import check_appium, check_app

# Returns abs path relative to this file and not cwd
PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)

# print os.path.dirname(__file__)
# print os.path.join(os.path.dirname(__file__), '../sample-code/apps/ContactManager/ContactManager.apk')
# print os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../sample-code/apps/ContactManager/ContactManager.apk'))
# 
# sleep
class ContactsAndroidTests(unittest.TestCase):

    driver = check_app('ContactManager/ContactManager.apk')
    def setUp(self):
        try:
            check_appium()
        except:
            os.sys.exit()
            print "The test environments have initial failed!"
        else:
            print "The test environments have initial successfully!"

#         #dictionary
#         desired_caps = {}
#         desired_caps['platformName'] = 'Android'
#         desired_caps['platformVersion'] = '4.4'
#         desired_caps['deviceName'] = 'nubia'
#         desired_caps['app'] = PATH(
#             '../../../sample-code/apps/ContactManager/ContactManager.apk'
#         )
# #         print PATH('../../../sample-code/apps/ContactManager/ContactManager.apk')
#        
#         desired_caps['appPackage'] = 'com.example.android.contactmanager'
#         desired_caps['appActivity'] = '.ContactManager'
# 
#         self.driver = webdriver.Remote('http://127.0.0.1:4723/wd/hub', desired_caps)



    def tearDown(self):
        self.driver.quit()


    def test_add_contacts(self):
        el = self.driver.find_element_by_name("Add Contact")
        el.click()

        textfields = self.driver.find_elements_by_class_name("android.widget.EditText")
        print self.driver.find_elements_by_class_name("android.widget.EditText")
        
        textfields[0].send_keys("Appium User")
        textfields[2].send_keys("someone@appium.io")

        self.assertEqual('Appium User', textfields[0].text)
        self.assertEqual('someone@appium.io', textfields[2].text)

        self.driver.find_element_by_name("Save").click()

        # for some reason "save" breaks things
        alert = self.driver.switch_to_alert()

        # no way to handle alerts in Android
        self.driver.find_element_by_android_uiautomator('new UiSelector().clickable(true)').click()

        self.driver.keyevent(3)


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    suite = unittest.TestLoader().loadTestsFromTestCase(ContactsAndroidTests)
    unittest.TextTestRunner(verbosity=2).run(suite)