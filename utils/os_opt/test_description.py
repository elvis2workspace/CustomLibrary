#!/usr/bin/env python
# -*- coding:utf-8 -*-


class Integer(object):
    def __init__(self, name):
        self.name = name

    def __get__(self,instance,owner):
        if instance is None:
            return self
        return instance.__dict__[self.name]

    def __set__(self,instance,value):
        if value < 0:
            raise ValueError('Negative value not allowed')
        instance.__dict__[self.name] = value


class Movie(object):
    score = Integer('score')
    ticket = Integer('ticket')

    def __init__(self, title, description, score, ticket):
        self.title = title
        self.description = description
        self.score = score
        self.ticket = ticket
