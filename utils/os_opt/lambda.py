#! /usr/bin/env python
# -*- coding: utf-8 -*-


# Filename:lambda.py

def make_repeater(n):
    return lambda s: s * n

twice = make_repeater(2)

print twice('word')
print twice(5)

exec 'print "Elvis, Hello World!"'

print eval('2*3')
