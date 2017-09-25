#! /usr/bin/python
# coding:utf-8

from data_utils import binary_search_recursion, binary_search_while

if __name__ == '__main__':
    arr = [3, 9, 28, 67, 12, 45]
    arr.sort()
    print(binary_search_recursion(12, arr, 0, len(arr) - 1))
    print(binary_search_recursion(3, arr, 0, len(arr) - 1))
    print(binary_search_recursion(9, arr, 0, len(arr) - 1))
    print(binary_search_recursion(99, arr, 0, len(arr) - 1))

    print(binary_search_while(12, arr))
    print(binary_search_while(3, arr))
    print(binary_search_while(9, arr))
    print(binary_search_while(99, arr))
