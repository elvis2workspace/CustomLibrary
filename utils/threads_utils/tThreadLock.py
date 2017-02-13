#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月22日

@author: zhang.xiuhai
'''
import thread
from time import sleep, ctime


LOOPS = [4,2]

def loop(nloop, nsec, lock):
    print "start loop ", nloop, " at :", ctime()
    sleep(nsec)
    print "loop ", nloop, " at :", ctime()
    lock.release()
    
def main():
    print "starting at :", ctime()
    locks = []
    nloops = range(len(LOOPS))
    
    for i in nloops:
        lock = thread.allocate_lock()
        lock.acquire()
        locks.append(lock)
        
    for i in nloops:
        thread.start_new_thread(loop, (i, LOOPS[i], locks[i]))
    
    for i in nloops:
        while locks[i].locked():pass
        print "all done at :", ctime()

if __name__ == '__main__':
    main()