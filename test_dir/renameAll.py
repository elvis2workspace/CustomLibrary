#!/usr/bin/env python
# -*- coding: utf8 -*-


import os
import sys

__author__ = "elvis"


def add_name():
    try:
        fileNames = os.listdir(dir)
    except BaseException, e:
        print '#### ERROR: Your input is not a file path! ####'
        sys.exit()
        ex = raw_input('input the ex:\t')
    for fileName in fileNames:
        newName = dir + '\\' + ex + fileName
        print newName
        os.rename(dir + '\\' + fileName, newName)


def re_name():
    try:
        fileNames = os.listdir(dir)
    except BaseException, e:
        print '#### ERROR: Your input is not a file path! ####'
        sys.exit()
    while True:
        ex = raw_input('input the ex:\t')
        x = len(ex)
        if x <= 3:
            print '\"ex is too short\"'
            continue

        for fileName in fileNames:

            if ex in fileName:
                newName = dir + '\\' + fileName[x:]
                print newName
                os.rename(dir + '\\' + fileName, newName)
            else:
                print 'This \"%s\" is not exist!' % ex

        choice = raw_input('type \"y\" or \"Y\" for retry or more files rename ?\t')
        if choice in ('y', 'Y'):
            pass
        else:
            print '*' * 30 + '\nBye!'
            break


if __name__ == '__main__':
    choice = int(raw_input('what do you want? addName press \'1\',rename press \'2\'\t'))
    dir = raw_input('The path of your files:\t')
    if choice == 1:
        add_name()
    elif choice == 2:
        re_name()
    else:
        print 'choose a right choice!!!'
