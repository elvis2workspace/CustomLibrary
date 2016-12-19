#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年6月4日

@author: zhangxiuhai
'''
import os
import unittest

from app.AndroidTools.common import check_app

# Returns abs path relative to this file and not cwd
PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)

class launchAppTest(unittest.TestCase):

    driver = check_app('SecureSms/SecureSms.apk')
    def setUp(self):
        try:
            pass
        except:
            os.sys.exit()
            print "The test environments have initial failed!"
        else:
            print "The test environments have initial successfully!"

    def tearDown(self):
        self.driver.quit()

    # 构造测试集  
    def suites(self):
        suite = unittest.TestSuite()  
        
        suite.addTest(launchAppTest("clickEnterElem"))
        return suite 
    
    def clickEnterElem(self):
        el = self.driver.find_element_by_id("com.raycom.securesms:id/start_messaging_button")
        print "get enter textview."
        el.click()
    def activateSms(self):
        pass

if  __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    suite = unittest.TestLoader().loadTestsFromTestCase(launchAppTest)
    unittest.TextTestRunner(verbosity=2).run(suite)