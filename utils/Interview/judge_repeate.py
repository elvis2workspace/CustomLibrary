#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on 2015年9月24日

@author: zhang.xiuhai
"""

import random
import time

"""
计算方法时间比：指定个数个随机数列表的相同元素
"""


class CompareEfficiency(object):
    def record_time(self, func, alist):
        start = time.time()
        func(alist)
        end = time.time()

        return end - start

    def normal_find_same(self, alist):
        length = len(alist)
        for i in range(length):
            for j in range(i + 1, length):
                if alist[i] == alist[j]:
                    return True
        return False

    def quick_find_same(self, alist):
        alist.sort()
        length = len(alist)
        for i in range(length - 1):
            if alist[i] == alist[i + 1]:
                return True
        return False

    """
    获取各方法耗时
    """

    def get_diff_time(self, count):
        methods = (self.normal_find_same, self.quick_find_same)
        alist = range(count)
        random.shuffle(alist)

        for m in methods:
            print 'The method %s spends %s' % (m.__name__, self.record_time(m, alist))


if __name__ == "__main__":
    ct = CompareEfficiency()
    ct.get_diff_time(10000)
