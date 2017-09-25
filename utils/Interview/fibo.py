#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 斐波拉契数不同实现方式

def fibo(n):
    if n == 0 or n == 1:
        return 1
    else:
        return fibo(n - 1) + fibo(n - 2)


def fibo1(n):
    stack = []
    if n == 0 or n == 1:
        return 1
    else:
        stack.append(1)
        stack.append(1)
        for i in range(2, n):
            stack.append(stack[i - 1] + stack[i - 2])
        return stack


def fibo2(n):
    x, y = 0, 1
    stack = []
    while y < n:
        stack.append(y)
        x, y = y, x + y
        print x, y
    return stack


def fibo3():
    x, y = 1, 1
    while True:
        yield x
        x, y = y, x + y


if __name__ == '__main__':
    for i in range(0, 10):
        print fibo(i),
    # sum = fibo(10)
    # print sum
    print '\n'
    print fibo1(10)

    print fibo2(10)

    import itertools
    print "----fibo3----"
    print list(itertools.islice(fibo3(), 10))
