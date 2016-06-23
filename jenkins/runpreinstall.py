#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月16日

@author: zhang.xiuhai
'''
import zipfile
import os, os.path
import subprocess
import shutil
from datetime import *
import time
import  sys
from robot.api import logger
from CustomLibrary.customkeywords import _custom_android_utils
import threading

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

RUNTIMETAG = "robot-runlog-%date:~0,4%%date:~5,2%%date:~8,2%"

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
            if not os.path.exists(ext_dir): os.mkdir(ext_dir, 0777)
            outfile = open(ext_filename, 'wb')
            outfile.write(zfobj.read(name))
            outfile.close()

#初始化本地文件目录，便于执行预安装
def initial_env():
    src_file_path = "Z:\VoLTE_ECM\DailyBuild\\release\\" + datetime.now().strftime('%Y%m%d') + "\\releasePack.zip"
    print src_file_path
    print PATH(r"./releasePack.zip")

    restr = shutil.copy(src_file_path, PATH(r"./releasePack.zip"))

    print "shutil.copy rest: ", restr

    unzip_file(PATH(r"./releasePack.zip"), "./")
    if not os.path.exists(PATH(r"./releasePack/")): return -1
    tmpfile = open(r'releasePack/clean_all.bat', 'r+')
    filelit = tmpfile.readlines()
    print filelit[-2]
    filelit[-2] = ""
    tmpfile.seek(0)
    tmpfile.writelines(filelit)
    tmpfile.close()
    
    tmpfile1 = open(r'releasePack/install_32lib_app.bat', 'r+')
    filelit1 = tmpfile1.readlines()
    print filelit1[-3], filelit1[-9]
    filelit1[-3] = ""
    filelit1[-9] = ""
    tmpfile1.seek(0)
    tmpfile1.writelines(filelit1)
    tmpfile1.close()

    shutil.copy(r'releasePack/EncryptCardManager.apk', './')
    shutil.move(r'releasePack/VoLTE_libs/', './VoLTE_libs/')
    return 0

#执行批处理命令
def run_bat(batfile=None):
    bat_path = PATH(r"./releasePack/"+batfile)
    if not os.path.exists(bat_path): return -1
    childpid = subprocess.Popen(bat_path, shell=True, stdout=subprocess.PIPE)
    childpid.wait()
    restdoutstr = childpid.stdout.readlines()

    for restr in restdoutstr:
        if restr.find("failed!") is not -1:
            print "Failed reason: ", restr
            shutil.rmtree(PATH(r"./releasePack/"))
            shutil.rmtree(PATH(r"./VoLTE_libs/"))
            # shutil.rmtree(PATH(r"./bak/"))
            os.remove(PATH(r"./EncryptCardManager.apk"))
            os.remove(PATH(r"./releasePack.zip"))
            return -1
    return 0

#执行预安装命令        
def run_pre_install():
    if initial_env() is not 0:
        #预拷贝
        print "Check your environment please!"
     # run_bat(r'clean_all.bat')
     # time.sleep(30)
    if run_bat(r'install_32lib_app.bat') is -1:
        logger.error("Run run_bat function error.", html=True)
        return -1
    time.sleep(30)
    return 0

if __name__ == '__main__':
    #拷贝安装包进行安装
    if run_pre_install() is not 0:
        logger.error("Execute run_pre_install function failed!",html=True)

    time.sleep(20)

    #执行冒烟测试用例robotframework
    pybot_cmd = u"pybot.bat -d D:\\Logs\\robotf-runlog\\robotf-runlog-" + datetime.now().strftime('%Y%m%d%H') + \
    " -o output.xml -r report.html -l log.html -L TRACE --argumentfile " + PATH(r"./argfile.txt") + \
    " F:\\Myspace\\GitHub\\OPython\\robotframework"

    os.system(pybot_cmd)