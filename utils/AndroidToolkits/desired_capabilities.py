#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on 2015年4月10日

@author: zhangxiuhai
"""

import os

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))


def get_desired_capabilities(app=None):
    desired_caps = {'platformName': 'Android', 'platformVersion': '4.4', 'deviceName': 'APU0215C03002261'}
    if app is not None:
        desired_caps['app'] = PATH(
            r'../../../mgAuto/apps/' + str(app))

    # 插入字典新项（需要获取包名和组件名）
    desired_caps['appPackage'] = 'com.cetcs.ecmapplication'
    desired_caps['appActivity'] = '.LaunchActivity'
    desired_caps['unicodeKeyboard'] = 'true'
    desired_caps['resetKeyboard'] = 'true'

    return desired_caps

if __name__ == '__main__':
    app = get_desired_capabilities()
    print app
