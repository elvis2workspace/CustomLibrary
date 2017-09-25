#!/usr/bin/env python
# -*- coding: utf-8 -*-


import time
import random
from functools import wraps
from guppy import hpy


# 使用一个装饰器来测量函数执行时间


def fn_timer(function):
    @wraps(function)
    def function_timer(*args, **kwargs):
        t0 = time.time()
        result = function(*args, **kwargs)
        t1 = time.time()
        print("Total time running %s: %s seconds" % (function.func_name, str(t1 - t0)))
        return result

    return function_timer


@fn_timer
def random_sort(n):
    return sorted([random.random() for i in range(n)])





"""
使用 memory_profiler 模块

memory_profiler 模块被用于在逐行的基础上，测量你代码的内存使用率。尽管如此，它可能使得你的代码运行的更慢。

安装：

$pip install memory_profiler 

也建议安装 psutil 包，使得 memory_profile 模块运行的更快：

$pip install psutil 

类似 line_profiler 的方式，使用装饰器 @profile 来标记哪个函数被跟踪。下一步，键入：

$python -m memory_profiler timing_ctm.py 
"""


def random_sort2(n):
    hp = hpy()
    print"Heap at the beginning of the functionn", hp.heap()
    l = [random.random() for i in range(n)]
    l.sort()
    print"Heap at the end of the functionn", hp.heap()
    return l


if __name__ == '__main__':
    random_sort(2000000)
    random_sort2(2000000)
