#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月22日

@author: zhang.xiuhai
'''
import threading
from time import sleep, ctime

LOOPS = [4,2]

class ThreadFunc(object):
    '''
    classdocs
    '''


    def __init__(self, func, args, name=''):
        '''
        Constructor
        '''
        self.name = name
        self.func = func
        self.args = args
        
    def __call__(self):
        self.res = self.func(*self.args)
        
def loop(nloop, nsec):
    print 'start loop', nloop, 'at:', ctime()
    sleep(nsec)
    print 'loop', nloop, 'done at:', ctime()
        
def main():
    print "starting at :", ctime()
    threads = []
    nloops = range(len(LOOPS))
    
    for i in nloops: #create all threads
        t = threading.Thread(target=ThreadFunc(loop, (i, LOOPS[i]), loop.__name__))
        threads.append(t)
        
    for i in nloops:
        threads[i].start()# start all threads
        
    for i in nloops: # wait for completion
       threads[i].join
       
    print "All done at: ", ctime()
    
if __name__ == '__main__':
    main()    
           
           