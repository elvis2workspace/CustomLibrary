#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年6月1日

@author: zhangxiuhai
'''
import os
import unittest

from app.AndroidTools.common import check_app

check_appium

# Returns abs path relative to this file and not cwd
PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)

class AddContactsTest(unittest.TestCase):

    driver = check_app(SecureSms/SecureSms.apk)
    def setUp(self):
        try:
            check_appium()
        except:
            os.sys.exit()
            print "The test environments have initial failed!"
        else:
            print "The test environments have initial successfully!"


    def tearDown(self):
        self.driver.quit()


    def Test_Add_Contacts(self):
        el = self.driver.find_element_by_name("Add Contact")
        el.click()


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    suite = unittest.TestLoader().loadTestsFromTestCase(AddContactsTest)
    unittest.TextTestRunner(verbosity=2).run(suite)