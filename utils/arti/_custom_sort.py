#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Created on 2015年9月24日

@author: zhang.xiuhai
"""


def set_uniq(s_list):
    if s_list:
        s_list.sort()
        last = s_list[-1]
        for i in range(len(s_list) - 2, -1, -1):
            if last == s_list[i]:
                del s_list[i]
            else:
                last = s_list[i]

    return s_list


class CustomSort(object):
    """
    classdocs
    """

    @staticmethod
    def _counting_sort(self, alist, k):
        alist3 = [0 for i in range(k)]
        alist2 = [0 for i in range(len(alist))]
        for j in alist:
            alist3[j] += 1
        for i in range(1, k):
            alist3[i] += alist3[i - 1]
        for l in alist[::-1]:
            alist2[alist3[l]-1] = l
            alist3[l] -= 1
        return alist2

    # 定义求质数的函数

    # 字符列表去重

    # 定义冒泡排序
    @classmethod
    def _bubble_sort(self, seq):
        for i in range(len(seq)):
            for j in range(i, len(seq)):
                if seq[j] < seq[i]:
                    tmp = seq[j]
                    seq[j] = seq[i]
                    seq[i] = tmp

    # 定义选择排序
    @classmethod
    def _selection_sort(self, seq):
        for i in range(len(seq)):
            position = i
            for j in range(i, len(seq)):
                if seq[position] > seq[j]:
                    position = j
            if position != i:
                tmp = seq[position]
                seq[position] = seq[i]
                seq[i] = tmp

    # 定义插入排序
    @classmethod
    def _insertion_sort(self, seq):
        if len(seq) > 1:
            for i in range(1, len(seq)):
                while i > 0 and seq[i] < seq[i - 1]:
                    tmp = seq[i]
                    seq[i] = seq[i - 1]
                    seq[i - 1] = tmp
                    i -= 1

    def __call__(self, sort_list, k=None):
        if k is None:
            import heapq
            k = heapq.nlargest(1, sort_list)[0] + 1
        return self._counting_sort(sort_list, k)


def get_prim(n):
    # 我们从3开始，提升效率，呵呵，微乎其微啦
    p = 3
    x = 0
    while x < n:
        result = True
        for i in range(2, p - 1):
            if p % i == 0:
                result = False
        if result is True:
            x += 1
            rst = p
            # 注意:这里加2是为了提升效率，因为能被双数肯定不是质数。
        p += 2
    print(rst)


# 调试调用函数
if __name__ == '__main__':
    get_prim(1000)
    seq = [22, 22, 34, 45, 23, 23, 45, 344]
    List = set_uniq(seq)

    print "after deleting the repeated element the list is : ", List
    print "--------bubble_sort-------------"
    seq = [22, 1, 33, 4, 7, 6, 8, 9, 11]
    CustomSort._bubble_sort(seq)
    print seq

    print "--------selection_sort-------------"
    seq = [88, 44, 33, 4, 7, 6, 8, 9, 11]
    CustomSort._selection_sort(seq)
    print seq

    print "--------insertion_sort-------------"
    seq = [777, 44, 33, 4, 7, 6, 1111, 100, 11]
    CustomSort._insertion_sort(seq)
    print seq

 


