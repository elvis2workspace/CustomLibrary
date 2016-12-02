# -*- coding: utf-8 -*-

"""
Created on 2015年5月8日
@author: zhang.xiuhai
"""

import ctypes

STD_INPUT_HANDLE = -10
STD_OUTPUT_HANDLE= -11
STD_ERROR_HANDLE = -12

FOREGROUND_WHITE = 0x0007
FOREGROUND_BLUE = 0x01 # text color contains blue.
FOREGROUND_GREEN= 0x02 # text color contains green.
FOREGROUND_RED  = 0x04 # text color contains red.
FOREGROUND_INTENSITY = 0x08 # text color is intensified.
FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN
BACKGROUND_BLUE = 0x10 # background color contains blue.
BACKGROUND_GREEN= 0x20 # background color contains green.
BACKGROUND_RED  = 0x40 # background color contains red.
BACKGROUND_INTENSITY = 0x80 # background color is intensified.

std_out_handle = ctypes.windll.kernel64.GetStdHandle(STD_OUTPUT_HANDLE)


def set_color(color, handle=std_out_handle):
    """(color) -> BOOL
    Example: set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
    """
    cus_bool = ctypes.windll.kernel64.SetConsoleTextAttribute(handle, color)
    return cus_bool