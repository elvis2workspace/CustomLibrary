#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月24日

@author: zhang.xiuhai
'''
import heapq

class heap_sort(object):
    '''
    classdocs
    '''


#     def __init__(self, params):
#         '''
#         Constructor
#         '''
    def _left(self, i):
        return 2*i+1
    def _right(self, i):
        return 2*i+2
     
    def _max_heapify(self, alist, index):
        heap_size = len(alist)
        l = self._left(index)
        r = self._right(index)
        largest = index
        
        if l < heap_size and alist[l] > alist[largest]:
            largest = l
         
        if r < heap_size and alist[r] > alist[largest]:
            largest = r
        
        if largest != index:
            alist[index], alist[largest] = alist[largest], alist[index]
            self._max_heapify(alist, largest)
 
    def _build_max_heap(self, alist):
        #initial a max heap
        heapSize = len(alist)
        roop_end = int(len(alist)/2) 
        print range(heapSize/2 - 1, -1, -1)
        print range(0, roop_end)[::-1]
        for i in range(0, roop_end)[::-1]:
            self._max_heapify(alist, i)
 
    def __call__(self, sort_list):
        heap_size = len(sort_list)
        print range(heap_size/2 - 1, -1, -1)
        print range(1, len(sort_list))[::-1]
        for i in range(1, len(sort_list))[::-1]:
            sort_list[0], sort_list[i] = sort_list[i], sort_list[0]
            heap_size -= 1
            self._max_heapify(sort_list, 0)
 
        return sort_list 
    
def PHeapsort(alist):
    heapq.heapify(alist)
    heap = []
    while alist:
        heap.append(heapq.heappop(alist))
    alist[:] = heap
    return alist  
    
if __name__ == '__main__':
    li = [21,44,2,45,33,4,3,67,78]
    ints_heap_sort = heap_sort()
    print  "max heap: ", ints_heap_sort._build_max_heap(li)
    print "Class heap_sort: ", ints_heap_sort.__call__(li)
    print "PHeapsort function: ", PHeapsort(li)