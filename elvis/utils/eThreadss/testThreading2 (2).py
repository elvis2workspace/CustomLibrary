#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月22日

@author: zhang.xiuhai
'''
import threading
from time import sleep, ctime

LOOPS=(4, 2)

class MyThreads(threading.Thread):
    
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.name  = name
        self.func = func
        self.args = args
        
    def run(self):
#         apply(self.func, self.args)
        self.res = self.func(*self.args)

def loop(nloop, nsec):
    print 'start loop', nloop, 'at:', ctime()
    sleep(nsec)
    print 'loop', nloop, 'done at:', ctime()
    
def main():
    print 'starting at:', ctime()
    threads = []
    nloops = range(len(LOOPS))
    
    for i in nloops:
        t = MyThreads(loop, (i, LOOPS[i]), loop.__name__)
        threads.append(t)

    for i in nloops:
        threads[i].start()

    for i in nloops:
        threads[i].join()

    print 'all DONE at:', ctime()

if __name__ == '__main__':
    main()
