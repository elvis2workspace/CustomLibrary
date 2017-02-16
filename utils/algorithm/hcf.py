# coding=utf-8
"""
Created on 2015年9月22日

@author: Elvis
"""

"""最小公约数"""


def hcf(x, y):
    for i in range(1, min(x, y) + 1):
        if (x % i == 0) and (y % i == 0):
            hcf = i

    return hcf


"""最大公倍数"""


def mgb(x, y):
    hcfe = hcf(x, y)
    return x * y / hcfe


def lcm(x, y):
    greater = max(x, y)
    while True:
        if (greater % x == 0) and (greater % y == 0):
            lcm = greater
            break
        greater += 1
    return lcm

"""斐波拉契数列"""


def recur_fibo(n):
    if n <= 1:
        return n

    else:
        return recur_fibo(n - 1) + recur_fibo(n - 2)


if __name__ == '__main__':

    num1 = int(raw_input("first: "))
    num2 = int(raw_input("second: "))
    num3 = int(raw_input("third: "))

    print hcf(num1, num2)
    print mgb(num1, num2)
    print lcm(num1, num2)

    for i in range(num3):
        print recur_fibo(i)
