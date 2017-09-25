#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
如果一个数恰好等于它的因子之和，则称该数为“完全数”  。各个小于它的约数（真约数,列出某数的约数，去掉该数本身，剩下的就是它的真约数）
的和等于它本身的自然数叫做完全数（Perfect number），又称完美数或完备数。
例如：
第一个完全数是6，它有约数1、2、3、6，除去它本身6外，其余3个数相加，1+2+3=6。
第二个完全数是28，它有约数1、2、4、7、14、28，除去它本身28外，其余5个数相加，1+2+4+7+14=28。
第三个完全数是496，有约数1、2、4、8、16、31、62、124、248、496，除去其本身496外，其余9个数相加，1+2+4+8+16+31+62+124+248=496。
后面的完全数还有8128、33550336等等。
"""

# 阶乘

def factorials(n):

    if 0 == n or n == 1:
        return 1
    else:
        return factorials(n - 1) * n

# 计算除本身因子之和
def get_sum_factors(n):
    sum = 0
    if n <= 0:
        exit(1)
    if n == 1:
        sum = sum + 1
    else:
        for i in range(1, n):
            if n % i == 0:
                sum = sum + i
        return sum

# 完数
def perfect_number(n):
    stack = []
    for i in range(1, n+1):
        if get_sum_factors(i) == i:
            stack.append(i)
    return stack

if __name__ == '__main__':
    print factorials(4)

    print get_sum_factors(6)

    print perfect_number(100000000)
