#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Created on 2016年5月16日
@author: zhang.xiuhai
"""


import zipfile
import os, os.path
import subprocess
import shutil
import subprocess
import time
import zipfile
from datetime import *

import os
import os.path
from robot.api import logger

from ..config import config



PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))


def unzip_file(zipfilename=None, unzipdir=None):
    if not os.path.exists(unzipdir): os.mkdir(unzipdir, 0777)
    zfobj = zipfile.ZipFile(zipfilename)
    print zfobj
    
    for name in zfobj.namelist():
        print name
        name = name.replace('\\', '/')
        print name

        if name.endswith('/'):
            if os.path.exists(os.path.join(unzipdir, name)):
                pass
            else:
                os.mkdir(os.path.join(unzipdir, name))
        else:
            ext_filename = os.path.join(unzipdir, name)
            ext_dir = os.path.dirname(ext_filename)
            if not os.path.exists(ext_dir):
                os.mkdir(ext_dir, 0777)
            outfile = open(ext_filename, 'wb')
            outfile.write(zfobj.read(name))
            outfile.close()


# 初始化本地文件目录，便于执行预安装, 包括修改批处理文件以便于自动化执行

def initial_env():
    src_file_path = "Z:\VoLTE_ECM\DailyBuild\\release\\" + datetime.now().strftime('%Y%m%d') + "\\releasePack.zip"
    print src_file_path
    print PATH(r"./releasePack.zip")

    if os.path.exists(PATH(r"./EncryptCardManager/")):
        logger.info("%s have existed, goiong to remove...." % PATH(r"./EncryptCardManager/"), html=True, also_console=True)
        shutil.rmtree(PATH(r"./EncryptCardManager/"))

    if shutil.copy(src_file_path, PATH(r"./releasePack.zip")) is None:
        logger.info("%s have copied to destination path." % src_file_path, html=True, also_console=True)
    else:
        logger.error("%s failed to copy." % src_file_path, html=True)


    unzip_file(PATH(r"./releasePack.zip"), "./")
    if not os.path.exists(PATH(r"./releasePack/")): return -1
    tmp_file = open(r'releasePack/clean_all.bat', 'r+')
    file_lit = tmp_file.readlines()
    print file_lit[-2]
    file_lit[-2] = ""
    tmp_file.seek(0)
    tmp_file.writelines(file_lit)
    tmp_file.close()
    
    tmp_file1 = open(r'releasePack/install_32lib_app.bat', 'r+')
    file_lit1 = tmp_file1.readlines()
    tmp_file1.close()
    print file_lit1[-3], file_lit1[-9]
    file_lit1[-3] = ""
    file_lit1[-9] = ""
    tmp_file2 = open(r'releasePack/install_32lib_app.bat', 'w')
    tmp_file2.writelines(file_lit1)
    tmp_file2.close()

    shutil.copy(r'releasePack/EncryptCardManager.apk', './')
    shutil.move(r'releasePack/VoLTE_libs/', './VoLTE_libs/')
    return 0


# 执行批处理命令
def run_bat(batfile=None):
    bat_path = PATH(r"./releasePack/"+batfile)
    if not os.path.exists(bat_path):
        logger.error("%s is not existed." % bat_path, html=True)
        return -1
    child_pid = subprocess.Popen(bat_path, shell=True, stdout=subprocess.PIPE)
    ret_stdout_str = child_pid.stdout.readlines()
    print "ret_stdout_str: \n", ret_stdout_str

    for re_str in ret_stdout_str:

        if re_str.find("failed!") is not -1:
            print "Failed reason: ", re_str
            shutil.rmtree(PATH(r"./releasePack/"))
            shutil.rmtree(PATH(r"./VoLTE_libs/"))
            # shutil.rmtree(PATH(r"./bak/"))
            os.remove(PATH(r"./EncryptCardManager.apk"))
            os.remove(PATH(r"./releasePack.zip"))
            return -1
    return 0

# 执行预安装命令
def run_pre_install():
    if initial_env() is not 0:
        # 预拷贝
        print "Check your environment please!"

    if run_bat(r'install_32lib_app.bat') is -1:
        logger.error("Run run_bat function error.", html=True)
        return -1
    else:
        shutil.rmtree(PATH(r"./releasePack/"))
        shutil.rmtree(PATH(r"./VoLTE_libs/"))
        # shutil.rmtree(PATH(r"./bak/"))
        os.remove(PATH(r"./EncryptCardManager.apk"))
        os.remove(PATH(r"./releasePack.zip"))
        logger.info("All install relative files have been removed.", html=True, also_console=True)

    time.sleep(30)
    return 0

if __name__ == '__main__':

    # 拷贝安装包进行安装
    if run_pre_install() is not 0:
        logger.error("Execute run_pre_install function failed!", html=True)
    else:
        logger.console("Execute run_pre_install successfully.")

    time.sleep(20)

    # 执行冒烟测试用例 Robot framework

    # custom_utils.check_dir(config.ROBOTLOGPATH)

    pybot_cmd = "pybot.bat -d "+ config.ROBOTLOGPATH + " -o output.xml -r report.html -l log.html -L TRACE " \
                "--argumentfile " + PATH(r"./argfile_rf.txt") + " D:\\PS_auto_project\\Scripts\\RobotframeworkProject"
    print "pybot_cmd: ", pybot_cmd

    os.system(pybot_cmd)

