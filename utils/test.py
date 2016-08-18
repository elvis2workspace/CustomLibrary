# -*- coding: utf-8 -*-

"""
快速计算乘方的算法，求a的b次方
如计算2^13，则传统做法需要进行12次乘法，但是可以优化：
把2*2的结果保存起来看看，是不是成了：4*4*4*4*4*4*2
再把4*4的结果保存起来：16*16*16*2
一共5次运算，分别是2*2、4*4和16*16*16*2

这样分析，我们算法因该是只需要计算一半都不到的乘法了。
为了讲清这个算法，再举一个例子2^7：2*2*2*2*2*2*2
两两分开：(2*2)*(2*2)*(2*2)*2
如果用2*2来计算，那么指数就可以除以2了，不过剩了一个，稍后再单独乘上它。
再次两两分开，指数除以2： ((2*2)*(2*2))*(2*2)*2
实际上最后一个括号里的2 * 2是这回又剩下的，那么，稍后再单独乘上它
现在指数已经为1了，可以计算最终结果了：16*4*2=128
"""


def power_old(a, b):
    i = 0
    tmp = 1
    while i < b:
        print i,
        tmp *= a
        i += 1
    return tmp * 1


def power(a, b):
    r = 1
    if b == 0:
        return 1
    if b < 0:
        return 0
    while b > 1:
        if (b & 1) != 0:  # 判断b是否奇数，偶数的最低位必为0
            r *= a  # 若r为奇数，则把"剩下的"乘起来
        a *= a  # 主体乘方
        b /= 2  # 指数除以2
    return r*a


def multi_list(x, y):
    for i in range(0, x):
        print i,
    print "\t"
    for j in range(1, y):
        print j,
        for m in range(1, x):
            print j*m,
        print "\t"

if __name__ == '__main__':
    print power(2, 9)

    print power_old(2, 10)

    multi_list(4, 5)
