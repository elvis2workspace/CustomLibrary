#!/usr/bin/evn python
# -*- coding: utf-8 -*-

"""
确定一个字符串中是否所有字符全部互不相同
"""

from collections import Counter


class UniqueCharsInString():
    # 将字符串中的每一个字符与剩下的字符比较，如果遇到相同的元素，则返回False，如果直到遍历结束都没有遇到相同元素，则返回True：
    def __init__(self):
        pass

    def is_unique_char(self, string):
        str_len = len(string)

        if str_len > 256:
            return True

        for pos in xrange(str_len):
            for index in xrange(pos + 1, str_len):
                if string[pos] == string[index]:
                    return False

        return True  # 时间复杂度为O(n*n)，空间复杂度为O(1)。这种解法的效率非常低下

    # 构建一个布尔值的数组，索引index表示ASCII码中值为index的字符。将初值置为False，如果某个元素第二次出现，
    # 则表示这个字符串出现了重复的字符，函数直接返回
    def is_unique_char1(self, string):
        if len(string) > 256:
            return True

        record = [False] * 256

        for ch in string:
            ch_val = ord(ch)
            if record[ch_val]:
                return False
            record[ch_val] = True

        return True  # 时间复杂度为O(n)，空间复杂度为O(1)。不过，我们可以非常确定的是，n的最大值仅仅为256。

    # 使用位运算，结合Python中数字的特殊实现，我们仅需要一个数字来替代record即可实现上面的算法
    def is_unique_char2(self, string):
        if len(string) > 256:
            return True

        record = 0L

        for ch in string:
            # print record
            ch_val = ord(ch)

            if (record & (1 << ch_val)) > 0:
                return False
            record |= (1 << ch_val)

        return True

# Python的某些数据结构，则我们可以通过collections里的工具来实现：
is_unique_char3 = lambda s: True if len(s) > 256 else not bool(filter(lambda n: n > 1, Counter(s).values()))

# 对字符串进行排序后，然后遍历每一个元素并与周围元素比较(请自行尝试)


if __name__ == '__main__':
    string_tmp = 'zfleehfdfhefnlaflhiefldfhilllddez'
    ucis = UniqueCharsInString()
    print "is_unique_char: ", ucis.is_unique_char(string_tmp)
    print "is_unique_char1: ", ucis.is_unique_char1(string_tmp)
    print "is_unique_char2: ", ucis.is_unique_char2(string_tmp)
    print "is_unique_char3: ", is_unique_char3(string_tmp)
