#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Created on 2015年5月8日
@author: zhang.xiuhai
"""

import sys
import time
from _outputresult import _OutputResultKeywords


class _OutputRunnerKeywords:
    def __init__(self, stream=sys.stderr, descriptions=1, verbosity=1):
        # self.stream = _ColorWritelnDecorator(stream)
        self.stream = stream
        self.descriptions = descriptions
        self.verbosity = verbosity

    def run(self, test):
        result = _OutputResultKeywords(self.stream, self.descriptions, self.verbosity)
        self.stream.yellow('Note: Your Unit Tests Starts')
        self.stream.writeln()

        start_time = time.time()
        test(result)
        stop_time = time.time()
        time_taken = stop_time - start_time
        self.stream.green(result.separator2)
        run = result.testsRun
        self.stream.writeln("Ran %d test%s in %.3fs" % (run, run != 1 and "s" or "", time_taken))


        failed, errored = map(len, (result.failures, result.errors))
        self.stream.green("[  PASSED  ] %d tests" % (run - failed - errored))
        self.stream.writeln()

        if not result.wasSuccessful():

            error_summary = ""

            if failed:
                self.stream.red("[  FAILED  ] %d tests, listed below:" % failed)
                self.stream.writeln()
                for failedtest, failederorr in result.failures:
                    self.stream.red("[  FAILED  ] %s" % failedtest)
                    self.stream.writeln()
            if errored:
                self.stream.red("[  ERRORED ] %d tests" % errored)
                for erroredtest, erorrmsg in result.errors:
                    self.stream.red("[  ERRORED ] %s" % erroredtest)
                    self.stream.writeln()

            self.stream.writeln()
            if failed:
                self.stream.write("%d ERRORED TEST" % failed)
            if errored:
                self.stream.write("%d ERRORED TEST" % errored)
        return result
