#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月24日

@author: zhang.xiuhai
'''

class counting_sort(object):
    '''
    classdocs
    '''
    def _counting_sort(self, alist, k):
        alist3 = [0 for i in range(k)]
        alist2 = [0 for i in range(len(alist))]
        for j in alist:
            alist3[j] += 1
        for i in range(1, k):
            alist3[i] = alist3[i-1] + alist3[i]
        for l in alist[::-1]:
            alist2[alist3[l]-1] = l
            alist3[l] -= 1
        return alist2
 
    def __call__(self, sort_list, k=None):
        if k is None:
            import heapq
            k = heapq.nlargest(1, sort_list)[0] + 1
        return self._counting_sort(sort_list, k)