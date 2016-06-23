#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年5月8日

@author: zhang.xiuhai
'''

import unittest
import sys


class _OutputResultKeywords(unittest.TestResult):

    separator1 = '[----------] '

    separator2 = '[==========] '

    def __init__(self, stream=sys.stderr, descriptions=1, verbosity=1):

        unittest.TestResult.__init__(self)

        self.stream = stream

        self.showAll = verbosity > 1

        self.dots = verbosity == 1

        self.descriptions = descriptions

    # public
    def getDescription(self, test):

        if self.descriptions:
            return test.shortDescription() or str(test)
        else:
            return str(test)

    def startTest(self, test):

        self.stream.green('[ Run      ] ')
        self.stream.writeln(self.getDescription(test))
        unittest.TestResult.startTest(self, test)

        if self.showAll:
            self.stream.write(self.getDescription(test))
            self.stream.write(" ... ")

    def addSuccess(self, test):

        unittest.TestResult.addSuccess(self, test)
        if self.showAll:
            self.stream.writeln("ok")
        elif self.dots:
            self.stream.green('[       OK ] ')
            self.stream.writeln(self.getDescription(test))

    def addError(self, test, err):
        unittest.TestResult.addError(self, test, err)
        if self.showAll:
            self.stream.writeln("ERROR")
        elif self.dots:
            self.stream.write('E')

    def addFailure(self, test, err):
        unittest.TestResult.addFailure(self, test, err)
        if self.showAll:
            self.stream.writeln("FAIL")
        elif self.dots:
            self.stream.red('[  FAILED  ] ')
            self.stream.writeln(self.getDescription(test))
            self.stream.write(self._exc_info_to_string(err, test))