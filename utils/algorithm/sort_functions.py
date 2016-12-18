#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ..basic_opt._c_utils import  _c_random_list


def insert_sort(args):
    count = len(args)
    for i in range(1, count):
        key = args[i]
        j = i - 1
        while j >= 0:
            if args[j] > key:
                args[j+1] = args[j]
                args[j] = key
            j -= 1
    return args


def bubble_sort(args):
    for i in range(len(args) - 1, 0, -1):
        # print i
        for j in range(0, i):
            # print j,
            if args[j] > args[j+1]:
                args[j], args[j+1] = args[j+1], args[j]
        # print '\t'
    return args


if __name__ == '__main__':

    # 函数：产生一个随机列表_c_random_list(count)
    # array_tmp = [1, 2, 5, 3, 6, 8, 4]
    array_tmp = _c_random_list(10)
    print "Initial random list：", array_tmp

    bubble_sort(array_tmp)
    print "bubble_sort: ", array_tmp

    insert_sort(array_tmp)
    print "insert_sort: ", array_tmp

    array_tmp.sort()
    print "lib_sort: ", array_tmp
