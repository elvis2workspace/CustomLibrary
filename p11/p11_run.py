#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年7月12日

@author: zhang.xiuhai
'''
from ctypes import cdll
import os
import time

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

#make *.so

def compile_so():
    linuxstoccmd = "gcc -c -fPIC libtest.c"
    ctosocmd = "gcc -shared libtest.o -o libtest.so"
    
    os.system(linuxstoccmd)
    if not os.path.exists(PATH(r"libtest.o")):return -1
    os.system(ctosocmd)
    
    print "Complete to Compile to *.so "
    return 0


def callC():
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
    # callC()
    # upload_file("libPKCS11.so", "/data/")
    # upload_file("p11test", "/data/")
    # time.sleep(5)
    run_p11test()
    # setup_xml()


