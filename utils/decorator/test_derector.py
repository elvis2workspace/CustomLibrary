# -*- coding: utf-8 -*-


class Foo(object):

    def __init__(self, name, professional):
        self.__name = name
        self.professional = professional

    @property
    def get_name(self):
        return self.__name

    @get_name.setter
    def get_name(self, another_name):
        self.__name = another_name


name = '柴静'
professional = '新闻人'
foo = Foo(name, professional)
print "foo.get_name: ", foo.get_name

print "foo._Foo__name: ", foo._Foo__name

foo.get_name = '雾霾女神'

print "foo.get_name: ", foo.get_name

print "Foo.get_name: ", Foo.get_name
