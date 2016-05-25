#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月16日

@author: zhang.xiuhai
'''
import zipfile
import os, os.path
import subprocess
import time
import  shutil

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
            if not os.path.exists(ext_dir): os.mkdir(ext_dir, 0777)
            outfile = open(ext_filename, 'wb')
            outfile.write(zfobj.read(name))
            outfile.close()

#初始化本地文件目录，便于执行预安装
def initial_env():
    unzip_file(r'F:\Documents\PyCharmProjects\CustomLibrary\jenkins\releasePack1.0.4.zip', "./")
    if not os.path.exists(r'F:\Documents\PyCharmProjects\CustomLibrary\jenkins\releasePack'): return -1
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

#执行命令行
def run_bat(batfile=None):
    bat_path = PATH(r"./releasePack/"+batfile)
    print bat_path
    if not os.path.exists(bat_path): return -1
    childpid = subprocess.Popen(bat_path, shell=True)
    childpid.wait()

#执行预安装命令        
def run_pre_install():
    if 0 != initial_env():
        print "check env please."
    else:
         run_bat(r'clean_all.bat')
         time.sleep(20)
         run_bat(r'install_32lib_app.bat')

    shutil.rmtree(PATH(r"./releasePack/"))
    shutil.rmtree(PATH(r"./VoLTE_libs/"))
    shutil.rmtree(PATH(r"./bak/"))
    os.remove(PATH(r"./EncryptCardManager.apk"))

if __name__ == '__main__':
    run_pre_install()