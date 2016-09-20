# -*- coding: utf-8 -*-
import random


def _c_random_list(count):

    list_tmp = [random.randrange(1, 100000, i+1) for i in range(0, count)]
    return list_tmp


if __name__ == '__main__':
    list_t = _c_random_list(100000)
    print len(list_t)
    print list_t
