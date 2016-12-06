#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月1日

@author: zhangxiuhai
'''
import logging.config

from colorama import init, Fore #命令行带颜色输出模块

logging.config.fileConfig('D:\workspace\mgAuto\src\config\logging.conf')

# create logger
logger = logging.getLogger('seceMail')

# 'application' code
logger.debug('debug message')
logger.info('info message')
logger.warn('warn message')
logger.error('error message')
logger.critical('critical message')

def LogInfo(s):
    logger.info(s)
    
def logwarn(s):
    logger.info(s)
    
def setWarnLog( s ):
    init(autoreset=True)
    return Fore.RED +'%s' % s

if __name__ == '__main__':
    setWarnLog(logger.critical('just a test_selenium2library!'))