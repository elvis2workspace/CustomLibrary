#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2016年5月11日

@author: zhang.xiuhai
'''

import os

def launch_local_exe(default=None):
    os.system(default)
    
if __name__ == '__main__':
    launch_cmd = "E:\Appium\node.exe E:\Appium\node_modules\appium\bin\appium.js -a 192.168.20.114 -p 4723 --no-reset"
    launch_local_exe(launch_cmd)