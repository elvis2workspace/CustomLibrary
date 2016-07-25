# -*- coding: utf-8 -*-


class CustomException(Exception):
    def __init__(self, str_value):
        self.str = str_value

    def __str__(self):
        return self.str
