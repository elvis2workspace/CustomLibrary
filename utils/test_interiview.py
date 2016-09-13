# -*- coding: utf-8 -*-

# 1、从一个列表中筛选出满足一定条件的列表，并返回
# 解法：列表综合
# eg：
# 通过列表综合，可以从一个已有的列表导出一个新的列表
# 例如，你有一个数的列表，而你想要得到一个对应的列表，使其中所有大于2的数都是原来的2倍
# 对于这种应用，列表综合是最理想的方法


# !/usr/bin/python
# Filename: list_comprehension.py
import sys


def list_comprehension():

    list_one = [2, 3, 4, 1, 5, 7, 56, 34, 78, 33, 79]
    list_two = [2*i for i in list_one if i > 2]
    return list_two


# 函数接收元组或字典形式的参数，使用*和**前缀。这种方法在函数需要获取可变数量的参数
# 由于在args变量前有*前缀，所有多余的函数参数都会作为一个元组存储在args中
# 如果使用的是**前缀，多余的参数则会被认为是一个字典的键/值对

def power_sum(power, *args):
    """Return the sum of  each argument raised to specified power."""
    total = 0
    for i in args:
        total += pow(i, power)
    return total


def read_file(file_name):
    """Print a file to the standard output.

        this is just a example of docstring.
        :type file_name: object"""

    f = file(file_name)
    while True:
        line = f.readline()
        if len(line) == 0:
            break
        print line,  # notice comma

    f.close()

print read_file.__doc__

if __name__ == '__main__':
    print list_comprehension()
    print power_sum(2, 3, 5, 6)

    if len(sys.argv) < 2:
        print 'No action specified.'
        sys.exit()

    if sys.argv[1].startwith('--'):
        option = sys.argv[1][2:]
        # fetch sys.argv[1] but without the first two characters

        if option == 'version':
            print 'Version 1.2'
        elif option == 'help':
            print '''This program prints files to the standard output. Any number of files can be specified.
            Options include:
            --version : Prints the version number
            --help    : Display this help '''
        else:
            print 'Unknown option.'
        sys.exit()
    else:
        for filename in sys.argv[1:]:
            read_file(filename)

    print read_file.__doc__


