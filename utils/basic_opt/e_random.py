# -*- coding: utf-8 -*-
import random


# get长度为count的随机数列表
def get_random_list(count):

    """

    :param count:
    :return:
    """
    list_tmp = [random.randrange(1, 100000, i+1) for i in range(0, count)]
    return list_tmp


if __name__ == '__main__':
    list_t = get_random_list(100000)
    print len(list_t)
    print list_t
