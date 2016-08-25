# -*- coding: utf-8 -*-


def bubble_sort(args):
    for i in range(len(args) - 1, 0, -1):
        print i
        for j in range(0, i):
            print j,
            if args[j] > args[j+1]:
                args[j], args[j+1] = args[j+1], args[j]
        print '\t'

    print args


if __name__ == '__main__':
    array_tmp = [1, 2, 5, 3, 6, 8, 4]
    bubble_sort(array_tmp)
    array_tmp.sort()
    print array_tmp
