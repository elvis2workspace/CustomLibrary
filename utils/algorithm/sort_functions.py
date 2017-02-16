#!/usr/bin/env python
# -*- coding: utf-8 -*-

import heapq
from CustomLibrary.utils.data_utils import *


# 字符列表去重
def uniq(s_list):
    if s_list:
        s_list.sort()
        last = s_list[-1]
        for i in range(len(s_list) - 2, -1, -1):
            if last == s_list[i]:
                del s_list[i]
            else:
                last = s_list[i]

    return s_list


# 定义插入排序
def insertion_sort(seq):
    if len(seq) > 1:
        for i in range(1, len(seq)):
            while i > 0 and seq[i] < seq[i - 1]:
                tmp = seq[i]
                seq[i] = seq[i - 1]
                seq[i - 1] = tmp
                i -= 1


def insert_sort_r(args):
    count = len(args)
    for i in range(1, count):
        key = args[i]
        j = i - 1
        while j >= 0:
            if args[j] > key:
                args[j + 1] = args[j]
                args[j] = key
            j -= 1
    return args


# 插入排序算法
def insertion_sort_r(sort_list):
    iter_len = len(sort_list)
    if iter_len <= 2:
        return sort_list

    for i in range(1, iter_len):
        key = sort_list[i]
        j = i - 1
        while j >= 0 and sort_list[j] > key:
            sort_list[j + 1] = sort_list[j]
            j -= 1
        sort_list[j + 1] = key
    return sort_list


# 定义冒泡排序
def bubble_sort(seq):
    for i in range(len(seq)):
        for j in range(i, len(seq)):
            if seq[j] < seq[i]:
                tmp = seq[j]
                seq[j] = seq[i]
                seq[i] = tmp


def bubble_sort_r(args):
    for i in range(len(args) - 1, 0, -1):
        # print i
        for j in range(0, i):
            # print j,
            if args[j] > args[j + 1]:
                args[j], args[j + 1] = args[j + 1], args[j]
                # print '\t'
    return args


# 定义选择排序
def selection_sort(seq):
    for i in range(len(seq)):
        position = i
        for j in range(i, len(seq)):
            if seq[position] > seq[j]:
                position = j
        if position != i:
            tmp = seq[position]
            seq[position] = seq[i]
            seq[i] = tmp


# 选择排序算法
def selection_sort_r(sort_list):
    iter_len = len(sort_list)
    if iter_len <= 2:
        return sort_list

    for i in range(iter_len - 1):
        smaller = sort_list[i]
        location = i

        for j in range(i, iter_len):
            if sort_list[j] < smaller:
                smaller = sort_list[j]
                location = j
        if i != location:
            sort_list[i], sort_list[location] = sort_list[location], sort_list[i]
            '''交换两个变量的值,在python中你就可以这么写：a, b = b, a，
                              因为赋值符号的左右两边都是元组，在python中，元组其实是由逗号“,”来界定的，
                              而不是括号
            '''
    return sort_list


def quick_sort_2(sort_list):
    if len(sort_list) <= 1:
        return sort_list
    return quick_sort_2([lt for lt in sort_list[1:] if lt < sort_list[0]]) + \
           sort_list[0:1] + \
           quick_sort_2([ge for ge in sort_list[1:] if ge >= sort_list[0]])


class QuickSort(object):
    """
    classdocs
    """

    def _partition(self, alist, p, r):
        i = p - 1
        x = alist[r]
        for j in range(p, r):
            if alist[j] <= x:
                i += 1
                alist[i], alist[j] = alist[j], alist[i]
        alist[i + 1], alist[r] = alist[r], alist[i + 1]
        return i + 1

    def _quicksort(self, alist, p, r):
        if p < r:
            q = self._partition(alist, p, r)
            self._quicksort(alist, p, q - 1)
            self._quicksort(alist, q + 1, r)

    def __call__(self, sort_list):
        self._quicksort(sort_list, 0, len(sort_list) - 1)
        return sort_list


class MergeSort(object):
    """
    classdocs
    """

    def _merge(self, alist, p, q, r):
        left = alist[p:q + 1]
        right = alist[q + 1:r + 1]
        for i in range(p, r + 1):
            if len(left) > 0 and len(right) > 0:
                if left[0] <= right[0]:
                    alist[i] = left.pop(0)
                else:
                    alist[i] = right.pop(0)
            elif len(right) == 0:
                alist[i] = left.pop(0)
            elif len(left) == 0:
                alist[i] = right.pop(0)

    def _merge_sort(self, alist, p, r):
        if p < r:
            q = int((p + r) / 2)
            self._merge_sort(alist, p, q)
            self._merge_sort(alist, q + 1, r)
            self._merge(alist, p, q, r)

    def __call__(self, sort_list):
        self._merge_sort(sort_list, 0, len(sort_list) - 1)
        return sort_list


def HeapSort(list):
    heapq.heapify(list)
    heap = []
    while list:
        heap.append(heapq.heappop(list))
    list[:] = heap
    return list


class heap_sort(object):
    """
    classdocs
    """

    #     def __init__(self, params):
    #         '''
    #         Constructor
    #         '''

    def _left(self, i):
        return 2 * i + 1

    def _right(self, i):
        return 2 * i + 2

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
        # initial a max heap
        heapSize = len(alist)
        roop_end = int(len(alist) / 2)
        print range(heapSize / 2 - 1, -1, -1)
        print range(0, roop_end)[::-1]
        for i in range(0, roop_end)[::-1]:
            self._max_heapify(alist, i)

    def __call__(self, sort_list):
        heap_size = len(sort_list)
        print range(heap_size / 2 - 1, -1, -1)
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


class RandomQuickSort(QuickSort):
    """
    classdocs
    """
    # def _partition(self, alist, p, r):
    #     i = p - 1
    #     x = alist[r]
    #     for j in range(p, r):
    #         if alist[j] <= x:
    #             i += 1
    #             alist[i], alist[j] = alist[j], alist[i]
    #     alist[i + 1], alist[r] = alist[r], alist[i + 1]
    #     return i + 1

    def _randomized_partition(self, alist, p, r):
        i = random.randint(p, r)
        alist[i], alist[r] = alist[r], alist[i]
        return self._partition(alist, p, r)

    def _quicksort(self, alist, p, r):
        if p < r:
            q = self._randomized_partition(alist, p, r)
            self._quicksort(alist, p, q - 1)
            self._quicksort(alist, q + 1, r)

    def __call__(self, sort_list):
        self._quicksort(sort_list, 0, len(sort_list) - 1)
        return sort_list


class CountingSort(object):
    """
    classdocs
    """

    def _counting_sort(self, alist, k):
        alist3 = [0 for i in range(k)]
        alist2 = [0 for i in range(len(alist))]
        for j in alist:
            alist3[j] += 1
        for i in range(1, k):
            alist3[i] += alist3[i - 1]
        for l in alist[::-1]:
            alist2[alist3[l] - 1] = l
            alist3[l] -= 1
        return alist2

    def __call__(self, sort_list, k=None):
        if k is None:
            import heapq
            k = heapq.nlargest(1, sort_list)[0] + 1
        return self._counting_sort(sort_list, k)


# Sorts Pancakes
class PancakeSort(object):
    def __init__(self):
        pass

    def sort_pancakes(self, stack):
        sorted_index = len(stack)
        for i in reversed(range(len(stack))):
            stack = self.flip(stack, self.find_largest_pancake(stack, i))
            print("Flip Up", stack)
            stack = self.flip(stack, i)
            print("Flip Down", stack)
        return stack

    # All of the pancakes are sorted after index
    # Returns the index of largest unsorted pancake
    def find_largest_pancake(self, stack, index):

        largest_pancake = stack[index]
        largest_index = index
        for i in range(index):
            if stack[i] > largest_pancake:
                largest_pancake = stack[i]
                largest_index = i

        print " "
        print("Insert Spatula in index", largest_index, "Size", largest_pancake)
        return largest_index

    # Slide spatula under pancake at index and flip to top
    def flip(self, stack, index):
        newStack = stack[:(index + 1)]
        newStack.reverse()
        newStack += stack[(index + 1):]
        return newStack


if __name__ == '__main__':
    # 函数：产生一个随机列表_c_random_list(count)
    # array_tmp = [1, 2, 5, 3, 6, 8, 4]
    array_tmp = DataSource.get_random_list(10)
    print "Initial random list：", array_tmp

    bubble_sort(array_tmp)
    print "bubble_sort: ", array_tmp

    insert_sort_r(array_tmp)
    print "insert_sort: ", array_tmp

    array_tmp.sort()
    print "lib_sort: ", array_tmp

    qs = QuickSort()
    print "quick_sort: ", (qs.__call__(array_tmp))

    ms = MergeSort()
    print "merge_sort: ", (ms.__call__(array_tmp))

    ints_heap_sort = heap_sort()
    print "max heap: ", ints_heap_sort._build_max_heap(array_tmp)
    print "Class heap_sort: ", ints_heap_sort.__call__(array_tmp)
    print "PHeapsort function: ", PHeapsort(array_tmp)

    rqs = RandomQuickSort()
    print "random_quick_sort: ", (rqs.__call__(array_tmp))

    cs = CountingSort()
    print "counting sort: ", (cs.__call__(array_tmp))

    print "get prime: ", get_prime(1000)

    List = uniq(['b', 'b', 'd', 'b', 'c', 'a', 'a'])
    print "after deleting the repeated element the list is : ", List

    print "--------bubble_sort-------------"
    seq = [22, 1, 33, 4, 7, 6, 8, 9, 11]
    bubble_sort(seq)
    print seq

    print "--------selection_sort-------------"
    seq = [88, 44, 33, 4, 7, 6, 8, 9, 11]
    selection_sort(seq)
    print seq

    print "--------insertion_sort-------------"
    seq = [777, 44, 33, 4, 7, 6, 1111, 100, 11]
    insertion_sort(seq)
    print seq

    print "---------pancake_sort--------------"
    print "\nUnsorted:"
    print array_tmp

    print "\nIterating:"
    ps = PancakeSort()
    stack = ps.sort_pancakes(array_tmp)

    print"\nSorted:"
    print stack
    print ""
