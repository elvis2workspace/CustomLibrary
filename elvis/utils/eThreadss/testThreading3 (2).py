#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月22日

@author: zhang.xiuhai
'''
import threading
from time import sleep, ctime


class MyThreads(threading.Thread):
    '''
    classdocs
    '''
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.func = func
        self.args = args
        self.name = name
        
    def GetResult(self):
        return self.res
    
    def run(self):
        print 'starting', self.name, 'at:', \
        ctime()
#         self.res = apply(self.func, self.args)
        self.res = self.func(*self.args)
        print self.name, 'finished at:', \
        ctime()


        