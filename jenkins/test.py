#!/usr/bin/env python
# -*- coding: utf-8 -*-

<<<<<<< HEAD
"""
Created on 2016年5月19日

@author: zhang.xiuhai
"""

=======
'''
Created on 2016年5月19日

@author: zhang.xiuhai
'''
>>>>>>> first commit
import pexpect
import shutil
import os
from datetime import *
<<<<<<< HEAD
from ..keywords._custom_android_utils import _CustomAndroidKeywords
=======
from CustomLibrary.customkeywords._custom_android_utils import _CustomAndroidKeywords
>>>>>>> first commit

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

# class my_spawn(pexpect.spawn):
#     def __init__(self, command=None):
#         self.args = None
#         self.command = command
#         pexpect.spawn.__init__(self, None, None, timeout=1)
#
#         self.child_fd = -1
#         self.own_fd = False
#         self.closed = False
#
#     def read_nonblocking (self, size=1, timeout=-1):
#         return ""
#
# child = my_spawn('sleep 10')
# child.expect('foo', 1)

# src_file_path = "Z:\VoLTE_ECM\DailyBuild\\release\\" + datetime.now().strftime('%Y%m%d') + "\\releasePack.zip"
# print src_file_path
# print PATH(r"./releasePack.zip")
# # restr = shutil.copy(r"D:\\releasePack.zip", PATH(r"./releasePack.zip"))
# # restr = shutil.copy(src_file_path, PATH(r"./releasePack.zip"))
# print os.listdir("Z:\VoLTE_ECM\DailyBuild\\release\\" + datetime.now().strftime('%Y%m%d'))
# with open(src_file_path, "rb") as fsrc:
#     print fsrc.fileno()

# print "shutil.copy rest: ", restr
timezonetag = str(datetime.now().strftime('%Y%m%d%H%M%S'))
tmpObject = _CustomAndroidKeywords()
tmpObject.set_androidlog_status(timezonetag, False)
