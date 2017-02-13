#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月24日

@author: zhang.xiuhai
'''

class merge_sort(object):
    '''
    classdocs
    '''

    def _merge(self, alist, p, q, r):
        left = alist[p:q+1]
        right = alist[q+1:r+1]
        for i in range(p, r+1):
            if len(left)>0 and len(right)>0:
                if (left[0]<=right[0]):
                    alist[i] = left.pop(0)
                else:
                    alist[i] = right.pop(0)
            elif len(right)==0:
                alist[i] = left.pop(0)
            elif len(left)==0:
                alist[i] = right.pop(0) 
    def _merge_sort(self, alist, p, r):
        if p<r:
            q = int((p+r)/2)           
            self._merge_sort(alist, p, q)
            self._merge_sort(alist, q+1, r)
            self._merge(alist, p, q, r)
            
    
    def __call__(self, sort_list):
        self._merge_sort(sort_list, 0, len(sort_list)-1)
        return sort_list