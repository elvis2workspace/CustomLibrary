#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月10日

@author: zhangxiuhai
'''
import os

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

def get_desired_capabilities(app):
    desired_caps={}
    desired_caps['platformName'] = 'Android'
    desired_caps['platformVersion'] = '4.4'
    desired_caps['deviceName'] = 'Nubia'
    desired_caps['app'] = PATH(
        r'../../../mgAuto/apps/' + app
    )
        
        
    #需要获取包名和组件名
    desired_caps['appPackage'] = 'com.raycom.securesms'
    desired_caps['appActivity'] = 'cetc.ns.mnd.FlashScreenActivity'
    desired_caps['unicodeKeyboard'] = 'true'
    desired_caps['resetKeyboard'] = 'true'
    
    return desired_caps

if __name__ == '__main__':
    app = get_desired_capabilities('secemail.apk')
    print app