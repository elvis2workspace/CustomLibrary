# -*- coding: utf-8 -*-

"""Combine tests for gnosis.xml.objectify package (req 2.3+)"""

import doctest
import unittest

import HTMLTestRunner
import test_baidu
import test_youdao

suite = doctest.DocTestSuite()

# 罗列要执行的文件
suite.addTest(unittest.makeSuite(test_baidu.Baidu))
suite.addTest(unittest.makeSuite(test_youdao.Youdao))

file_name = u'D:\\PS_auto_project\\Logs\\result20.html'

fp = file(file_name, 'wb')

runner = HTMLTestRunner.HTMLTestRunner(stream=fp, title='Report_title', description='Report_Description')

runner.run(suite)
# unittest.TextTestRunner(verbosity=2).run(suite)
