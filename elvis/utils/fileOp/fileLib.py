#!/usr/bin/env python
# -*- encoding: utf-8 -*-
"""
Created on 2015年3月14日

@author: Elvis
"""

from sys import stdin
from os import walk
from os.path import join
import string
import os
import re


# 打印当前根目录下所查找文件的全路径
def search_file_under_dir():
    name = stdin.readline().rstrip()
    for root, dirs, files in walk('/'):
        if name in dirs or name in files:
            print join(root, name)


def copy_from_file2file():
    fin = file('test.txt', 'r')
    fout = file('test_1.txt', 'w')
    while True:
        line = fin.readline()
        if len(line)==0:
            break

        fout.write(line)

    fin.close()
    fout.close()


def search_file_likes(file_path, name):
    rtn_path = os.path.normpath(os.path.expanduser(file_path.replace('/', os.sep)))
    path_n, file_n = os.path.split(rtn_path)
    print path_n, "\n" + file_n

    for root, dirs, files in walk(path_n):
        print root, "\n"
        print dirs
        print files  # print all of files under the dirs
        if name in files:
            print name
        else:
            return -1


if __name__ == '__main__':
    # search_file_under_dir()
    tmp_path = __file__
    search_file_likes(tmp_path, 'fileLib.py')
    # print rtn_file, "\n" + rtn_path

