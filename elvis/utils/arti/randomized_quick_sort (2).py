#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月24日

@author: zhang.xiuhai
'''
import random

class randomized_quick_sort(object):
    '''
    classdocs
    '''

    def _randomized_partition(self, alist, p, r):
        i = random.randint(p, r)
        alist[i], alist[r] = alist[r], alist[i]
        return self._partition(alist, p, r)
 
    def _quicksort(self, alist, p, r):
        if p<r:
            q = self._randomized_partition(alist, p, r)
            self._quicksort(alist, p, q-1)
            self._quicksort(alist, q+1, r)