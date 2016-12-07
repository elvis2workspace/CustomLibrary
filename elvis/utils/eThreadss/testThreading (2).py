#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月22日

@author: zhang.xiuhai

创建一个Thread 的实例，传给它一个函数(及参数)
'''
import threading
from time import sleep, ctime

LOOPS = [4, 2]

def loop(nloop, nsec):
    print "start loop ", nloop, " at :", ctime()
    sleep(nsec)
    print "loop ", nloop, " at :", ctime()
     
def main():
    print "starting at :", ctime()
    
    threads = []
    nloops = range(len(LOOPS))
    
    for i in nloops:
        t = threading.Thread(target=loop, args=(i, LOOPS[i]))
        threads.append(t)
        
    for i in nloops: #start thread
        threads[i].start()
        
    for i in nloops: #wait for all threads to finish
        threads[i].join() 
    '''
        如果你的主线程除了等线程结束外，还有其它的事情要做（如处理或等待其它的客户请求）
         ，那就不用调用join()，只有在你要等待线程结束的时候才要调用join()。
    ''' 
        
    print "All DONE at: ", ctime()
    
if __name__ == '__main__':
    main()