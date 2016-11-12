#! /usr/bin/python
# -*- coding: utf-8 -*-


def make_repeater(n):
    return lambda s: s*n

twice = make_repeater(2)

print twice('word')
print twice(5)

# lambda 返回函数对象，本质上，lambda需要一个参数，后面仅跟单个表达式作为函数体，
# 而表达式的值被这个新建的函数返回。注意，
# 即便是print语句也不能用在lambda形式中，只能使用表达式。

# exec and eval statement
# exec语句用来执行储存在字符串或文件中的Python语句。
# 例如，我们可以在运行时生成一个包含Python代码的字符串，然后使用exec语句执行这些语句,如下：
exec 'print "Hello World!"'

# eval语句用来计算存储在字符串中的有效Python表达式，如下：

print eval('2*3')
