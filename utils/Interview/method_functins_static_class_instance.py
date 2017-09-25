#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 
@author: elvis.zhang
@license: Apache Licence
@file: method_functins_static_class_instance.py
@time: 18/09/2017 19:40

"""


def func():
    pass


class Main(object):
    def __init__(self):
        pass


    def instance_function(self, n):
        self.class_functions(n)
        self._static_functions(n)
        print n

    # 作为工厂方法创建实例对象
    @classmethod
    def class_functions(cls, n):
        print n

    # 方法中不需要访问任何实例方法和属性，只是传入参数返回值，可以定义为
    # 静态方法，节省实例化对象的开销成本，放在类中仅做为类服务
    @staticmethod
    def _static_functions(n):
        print n


if __name__ == '__main__':
    a = Main()
    a.instance_function(5)

    # 等价
    Main.instance_function(a, 5)

    try:
        Main.instance_function(5)
    except TypeError:
        print "TypeError , i know."

    # class_method
    Main.class_functions(7)

    a.class_functions(7)

    # static method
    a.static_functions(6)
