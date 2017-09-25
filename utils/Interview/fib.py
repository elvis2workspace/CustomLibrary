#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 使用生成器生成斐波拉契数列
def fib_list(n=None):
    def fib():
        a, b = 0, 1
        while True:
            yield a
            a, b = b, a + b

    if n is not None:
        p = fib()
        return [p.next() for i in xrange(n)]



