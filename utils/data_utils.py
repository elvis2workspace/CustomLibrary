# -*- coding: utf-8 -*-

import random
import string
import re
import sys, os, shutil, filecmp
from random import choice

MAXVERSIONS = 100


"""
定义求质数的函数
"""


def get_prime(n):
    # 我们从3开始，提升效率，呵呵，微乎其微啦
    p = 3
    x = 0
    while x < n:
        result = True
        for i in range(2, p - 1):
            if p % i == 0:
                result = False
        if result:
            x += 1
            rst = p
        # 注意:这里加2是为了提升效率，因为能被双数肯定不是质数。
        p += 2
    print(rst)


def prime_lists():
    for n in range(2, 10):
        for x in range(2, n):
            if n % x == 0:
                print n, 'equals', x, '*', n / x
                break
        else:
            # loop fell through without finding a factor
            print n, 'is a prime number.'


def get_each_prime(m):
    for n in range(2, m):
        for x in range(2, n):
            if n % x == 0:
                print n, " equals ", x, '*', n//x
                break
        else:  # this else is matching the inside for loop.
            # loop fell through without finding a factor.
            print n, ' is a prime number.'


def found_even_number(m):
    for num in range(2, m):
        if num % 2 == 0:
            print "Found an even number", num
            continue  # 循环继续执行下一次迭代
        print "Found a number", num

"""计算乘方"""


def power_old(a, b):
    i = 0
    tmp = 1
    while i < b:
        print i,
        tmp *= a
        i += 1
    return tmp * 1


def power(a, b):
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
    return r * a


def string_opt():
    params = {"server": "mpilgrim", "database": "master", "uid": "sa", "pwd": "secret"}
    for k, v in params.items():
        print "%s=%s" % (k, v)
    for k, x in params.items():
        print ";".join("%s=%s" % (k, v))


"""
打印多行列表，乘法矩阵
"""


def multi_list(x, y):
    for i in range(0, x):
        print i,
    print "\t"
    for j in range(1, y):
        print j,
        for m in range(1, x):
            print j * m,
        print "\t"


'''
pn1: 生成随机密码
'''


def GenPasswd(length=8, chars=string.letters + string.digits):
    return ''.join([choice(chars) for i in range(length)])


"""
（1）对于每一个随机字符，我们都按照以下流程处理一遍
（2）随机找到中间点，然后分成两段，翻转合并
（3）用最后的结果的倒数maxmem个字符来做locate标记
（4）每次找到这个locate处，若找不到，locate缩减为locate[1:]，继续找，直到找到为止
（5）我们要的本次的随机字符就是找到的locate处的后一个字符self.data[where+len(locate)+1]，
     如果locate为空，那么ch就是（2）后的第一个字符，也是随机的
"""


class DataSource(object):
    def __init__(self, filename):
        self.data = open(filename).read().lower()

    def renew(self, n=8, maxmem=3):
        chars = []
        for i in range(n):
            rmd_index = random.randrange(len(self.data))
            self.data = self.data[rmd_index:] + self.data[:rmd_index]
            where = -1
            locate = chars[-maxmem:]
            while where < 0 and locate:
                where = self.data.find(str(locate))
                locate = locate[1:]
            ch = self.data[where + len(locate) + 1]
            if not ch.islower():
                ch = random.choice(string.lowercase)
            chars.append(ch)
        return ''.join(chars)

    # get长度为count的随机数列表
    @staticmethod
    def get_random_list(count):
        """
        :type count:
        :param count:
        :return:
        """
        list_tmp = [random.randrange(1, 100000, i + 1) for i in range(0, count)]
        return list_tmp


'''
备份文件
'''


def backup(tree_top, bakdir_name='bakdir'):
    for root, subdirs, files in os.walk(tree_top):
        # join链接出每个root下的子目录bakdir
        backup_dir = os.path.join(root, bakdir_name)
        # 确保每个root下都有个子目录叫bakdir
        if not os.path.exists(backup_dir):
            os.makedirs(backup_dir)
            # bakdir下的不递归处理
        subdirs[:] = [d for d in subdirs if d != bakdir_name]

        for file in files:
            filepath = os.path.join(root, file)
            destpath = os.path.join(backup_dir, file)

            # 检查版本，共有MAXVERSIONS个版本
            for index in xrange(MAXVERSIONS):
                backup = "%s.%2.2d" % (destpath, index)
                if not os.path.exists(backup):
                    break
            if index > 0:
                old_backup = "%s.%2.2d" % (destpath, index - 1)

                # abspath = os.path.abspath(filepath)#filepath本来就是绝对路径
                try:

                    # 如果备份和源文件一致，continue不处理
                    if os.path.isfile(old_backup) and filecmp.cmp(filepath, old_backup, shallow=False):
                        continue
                except OSError:
                    pass
            try:
                shutil.copy(filepath, backup)
            except OSError:
                pass


'''
pn3:统计Apache中每个IP的点击率
思想：
（1）按行读内容，正则匹配
（2）检查IP范围，min 和 max 的妙用
（3）存在+1，不存在置1：list[ip] = list.get(ip,0) + 1，这里的get中的0是指获取不到ip的时候的默认值
'''


def calcuteApacheIpHits(logfile_pathname):
    ip_specs = r'\.'.join([r'\d{1,3}'] * 4)
    re_ip = re.compile(ip_specs)

    ipHitListing = {}
    contents = open(logfile_pathname, "r")
    for line in contents:
        match = re_ip.match(line)
        if match:
            ip = match.group()
            # 检查正确性
            try:
                quad = map(int, ip.split('.'))
            except ValueError:
                pass
            else:
                # 如果ip存在就+1，不存在就设置为1
                if len(quad) == 4 and min(quad) >= 0 and max(quad) <= 255:
                    ipHitListing[ip] = ipHitListing.get(ip, 0) + 1
    return ipHitListing


if __name__ == '__main__':
    string_opt()

    print power(2, 9)

    print power_old(2, 10)

    multi_list(4, 5)

    for i in range(6):
        print GenPasswd(12)

    p = DataSource("xml2html.py")
    print 'pn2: 生成易记的伪随机密码', p.renew(12)

    list_t = p.get_random_list(100000)
    print len(list_t)
    print '生成', len(list_t), '组随机数列表：\n', list_t
