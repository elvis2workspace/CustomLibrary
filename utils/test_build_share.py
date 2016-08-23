# -*- coding: utf-8 -*-

import win32net
import win32netcon

import os

def share():
    shinfo = {}
    folder = "c:\\test"
    shinfo['netname'] = os.path.basename(folder)
    shinfo['type'] = win32net.STYPE_DISKTREE
    shinfo['remark'] = 'data files'
    shinfo['premissions'] = 0
    shinfo['max_users'] = -1
    shinfo['current_users'] = 0
    shinfo['path'] = 'c:\\test'
    shinfo['passwd'] = ''
    server = r'\\'
    win32net.NetShareAdd(server, 2, shinfo)

if __name__ == '__main__':
    share()