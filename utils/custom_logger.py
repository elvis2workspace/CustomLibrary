#  -*- coding: utf-8 -*-
"""
more resource about logging , access to python url:
http://docs.python.org/2/library/logging.html
"""
import logging
import os
import ctypes

FOREGROUND_WHITE = 0X007
FOREGROUND_BLUE = 0x01  # text color contains blue.
FOREGROUND_GREEN = 0x02  # text color contains green.
FOREGROUND_RED = 0x04  # text color contains red.
FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN

STD_OUTPUT_HANDLE = -11
std_out_handle = ctypes.windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)


def set_color(color, handle=std_out_handle):
    bool_tmp = ctypes.windll.kernel32.SetConsoleTextAttribute(handle, color)
    return bool_tmp


class Logger:
    def __init__(self, path, console_level=logging.DEBUG, file_level=logging.DEBUG):
        self.logger = logging.getLogger(path)
        # Logging中有NOTSET < DEBUG < INFO < WARNING < ERROR < CRITICAL这几种级别，日志会记录设置级别以上的日志
        self.logger.setLevel(logging.DEBUG)
        # 设置日志格式
        """
        %(name)s Logger的名字
        %(levelname)s 文本形式的日志级别
        %(message)s 用户输出的消息
        %(asctime)s 字符串形式的当前时间。默认格式是 “2003-07-08 16:49:45,896”。逗号后面的是毫秒
        %(levelno)s 数字形式的日志级别
        %(pathname)s 调用日志输出函数的模块的完整路径名，可能没有
        %(filename)s 调用日志输出函数的模块的文件名
        %(module)s  调用日志输出函数的模块名
        %(funcName)s 调用日志输出函数的函数名
        %(lineno)d 调用日志输出函数的语句所在的代码行
        %(created)f 当前时间，用UNIX标准的表示时间的浮 点数表示
        %(relativeCreated)d 输出日志信息时的，自Logger创建以 来的毫秒数
        %(thread)d 线程ID。可能没有
        %(threadName)s 线程名。可能没有
        %(process)d 进程ID。可能没有
        """
        fmt = logging.Formatter('[%(name)s Logger] [%(filename)s] [%(asctime)s] [%(levelname)s] %(message)s', '%Y-%m-%d %H:%M:%S')

        # 设置CMD日志
        sh = logging.StreamHandler()
        # 添加handler到logger对象中
        sh.setFormatter(fmt)
        sh.setLevel(console_level)

        # 设置文件日志
        fh = logging.FileHandler(path)
        fh.setFormatter(fmt)
        fh.setLevel(file_level)
        self.logger.addHandler(sh)
        self.logger.addHandler(fh)

    def debug(self, message):
        self.logger.debug(message)

    def info(self, message):
        self.logger.info(message)

    def warn(self, message, color=FOREGROUND_YELLOW):
        set_color(color)
        self.logger.warn(message)
        set_color(FOREGROUND_WHITE)

    def error(self, message, color=FOREGROUND_RED):
        set_color(color)
        self.logger.error(message)
        set_color(FOREGROUND_WHITE)

    def critical(self, message):
        self.logger.critical(message)


if __name__ == '__main__':
    log_tmp = Logger('cus.log', logging.WARNING, logging.DEBUG)
    log_tmp.debug("just a debug information.")
    log_tmp.info("just a info level information.")
    log_tmp.warn("just a warn information.")
    log_tmp.error("just a error information")
    log_tmp.critical("just a critical information.")


