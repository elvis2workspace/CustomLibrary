#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月16日

@author: zhang.xiuhai
'''

import sys
import logging
from CustomLibrary.customkeywords import _custom_android_utils


# 提前启动appium工具
tmpObject = _custom_android_utils._CustomAndroidKeywords()

if len(sys.argv) == 1:
    logging.info("Default local address.")
    default_ip_address = tmpObject.get_local_address()
    tmpObject.launch_local_appium(default_ip_address)
elif len(sys.argv) == 2:
    ret_str = tmpObject.launch_local_appium(sys.argv[1])
else:
    pass
