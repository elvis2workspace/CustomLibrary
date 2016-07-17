#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年7月12日

@author: zhang.xiuhai
'''

from ctypes import cdll
import os
import time
import shutil
import zipfile
from robot.api import logger


PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

#make *.so


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


def compile_so():
    linuxstoccmd = "gcc -c -fPIC libtest.c"
    ctosocmd = "gcc -shared libtest.o -o libtest.so"
    
    os.system(linuxstoccmd)
    if not os.path.exists(PATH(r"libtest.o")):return -1
    os.system(ctosocmd)
    
    print "Complete to Compile to *.so "
    return 0


def initial_env_for_p11():
    src_file_path = "D:\\PS_auto_project\\SCS 1.2.4.zip"
    print src_file_path
    print PATH(r"./SCS.zip")

    restr = shutil.copy(src_file_path, "SCS.zip")

    print "shutil.copy rest: ", restr

    unzip_file(PATH(r"./SCS.zip"), "./SCS/")
    if not os.path.exists(PATH(r"./SCS/")):
        return -1
    print "SCS fold exist."

    # push *.so file
    if not os.path.exists(PATH(r"./SCS/libs/armeabi-v7a/libPKCS11.so")):
        return -1
    push_so_command = upload_file(PATH(r"./SCS/libs/armeabi-v7a/libPKCS11.so"), "/data")
    os.system(push_so_command)
    push_p11test_command = upload_file(PATH(r"./SCS/p11test/armeabi-v7a/p11test"), "/data")
    os.system(push_so_command)

    # check SCS service whether run or not
    check_service("ServerCenter")


def check_service(pro_alias):
    adb_cmd = "adb shell ps | grep " + str(pro_alias)
    pro_details = os.popen(adb_cmd).read()
    # print "proDetails: "+ proDetails
    # logger.info(proDetails, also_console=True)
    is_null = (len(pro_details) == 0)

    if is_null:
        logger.error(pro_alias + " is not alive.")
        return -1
    elif pro_details.endswith("./ServerCenter"):
        logger.info(pro_alias + " is alive.")
    else:
        return pro_details


def call_so():
    # load the *.so
    print PATH(r"libhelloworld.so")
    if not os.path.exists(PATH(r"libhelloworld.so")):
        return -1
    print "Loading...."
    somelibc = cdll.LoadLibrary("./libhelloworld.so")
    print somelibc
    
    print somelibc.helloworld()


def upload_file(sour_file=None, dest_path=None):
    if not os.path.exists(PATH(r"./" + sour_file)):
        return -1
    push_command = "adb push " + PATH(r"./" + sour_file) + " " + dest_path
    print push_command
    os.system(push_command)


# 执行p11测试
def run_p11test():
    run_command1 = "adb shell " + "\"chmod 777 /data/p11test\""
    print run_command1
    restr = os.system(run_command1)
    print restr
    run_command2 = "adb shell " + "\"cd /data/ && .\/p11test\""
    print run_command2
    os.system(run_command2)


def setup_xml():
    import xml.dom.minidom
    if not os.path.exists(PATH(r"./data.xml")):
        return -1
    xml_dom = xml.dom.minidom.parse(PATH(r"./data.xml"))
    xml_root = xml_dom.documentElement
    print xml_root.nodeName
    print xml_root.nodeValue
    print xml_root.nodeType
    print xml_root.ELEMENT_NODE
    Testname_value = xml_dom.getElementsByTagName('Testname')[0].firstChild.data
    Userpin_value = xml_dom.getElementsByTagName('Userpin')[0].firstChild.data
    Sopin_value = xml_dom.getElementsByTagName('Sopin')[0].firstChild.data
    Looptimes_value = xml_dom.getElementsByTagName('Looptimes')[0].firstChild.data
    Datalen_value = xml_dom.getElementsByTagName('Datalen')[0].firstChild.data
    print Testname_value
    print Userpin_value
    print Sopin_value
    print Looptimes_value
    print Datalen_value

    Testname_node = xml_dom.getElementsByTagName('Testname')
    print Testname_node[0].firstChild

if __name__ == '__main__':
    # call_so()
    # upload_file("libPKCS11.so", "/data/")
    # upload_file("p11test", "/data/")
    # time.sleep(5)
    # run_p11test()
    # setup_xml()
    initial_env_for_p11()


