#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月24日

@author: zhang.xiuhai
'''

import random
import time


def record_time(func, alist):
    start = time.time()
    func(alist)
    end = time.time()
 
    return end - start

def normal_find_same(alist):
    length = len(alist)
    for i in range(length):
        for j in range(i+1, length):
            if alist[i] == alist[j]:
                return True
    return False
 
def quick_find_same(alist):
    alist.sort()
    length = len(alist)
    for i in range(length-1):
        if alist[i] == alist[i+1]:
            return True
    return False
 
if __name__ == "__main__":
    methods = (normal_find_same, quick_find_same)
    alist = range(5000)
    random.shuffle(alist)
     
    for m in methods:
        print 'The method %s spends %s' % (m.__name__, record_time(m, alist))