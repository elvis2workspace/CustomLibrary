#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2016年5月19日

@author: zhang.xiuhai
'''
import pexpect

class my_spawn(pexpect.spawn):
    def __init__(self, command=None):
        self.args = None
        self.command = command
        pexpect.spawn.__init__(self, None, None, timeout=1)

        self.child_fd = -1
        self.own_fd = False
        self.closed = False

    def read_nonblocking (self, size=1, timeout=-1):
        return ""

child = my_spawn('sleep 10')
child.expect('foo', 1)