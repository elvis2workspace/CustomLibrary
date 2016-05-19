#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月19日

@author: zhang.xiuhai
'''
import os

RUNTIMETAG = "robot-runlog-%date:~0,4%%date:~5,2%%date:~8,2%"


def runPybot():
    pybot_cmd = u"pybot.bat -d D:\\Logs\\robotf-runlog-%date:~0,4%%date:~5,2%%date:~8,2%\ -T -o output.xml -r report.html -l log.html \
    -L TRACE --argumentfile E:\Python27\Lib\site-packages\CustomLibrary\jenkins\\argfile.txt --listener \
    E:\\Python27\\lib\\site-packages\\robotide\\contrib\\testrunner\\TestRunnerAgent.py:59463:False \
    F:\\Myspace\\GitHub\\OPython\\robotframework"
    print pybot_cmd
    os.system(pybot_cmd)
    
if __name__ == '__main__':
    runPybot()
#     print os.system(RUNTIMETAG)