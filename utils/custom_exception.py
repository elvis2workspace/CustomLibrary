# -*- coding: utf-8 -*-

from robot.api import logger


class CustomException(Exception):
    """Root exception for all exceptions raised by this module."""
    def __init__(self, str_value):
        self.str = str_value

    def __str__(self):
        return self.str


class SocketTimeError(CustomException):
    pass


class SocketRefusedError(CustomException):
    pass


class OpenFileError(CustomException):
    pass


class WriteContentError(CustomException):
    pass


class DevicesNotFound(CustomException):
    """被测设备未连接"""


class ShortInputException(Exception):
    """ A useer-defined exception class."""
    def __init__(self, length, atleast):
        Exception.__init__(self)
        self.length = length
        self.atleast = atleast


def connect(self):
    pass


def write(content):
    if isinstance(content, basestring):
        f_handler = open("file.txt",'w')
        try:
            f_handler.write(content)
        except Exception:
                raise WriteContentError
        finally:
            f_handler.close()
    else:
        raise OpenFileError


def test_two_divide():
    try:
        x = input('Enter the first number: ')
        y = input('Enter the second number: ')
        print x/y
    except (ZeroDivisionError,TypeError,NameError):
        print "你的数字不对！"


def test_two_divide1():
    try:
        x = input('Enter the first number: ')
        y = input('Enter the second number: ')
        print x/y
    except ZeroDivisionError:
        print "输入的数字不能为0！"
    except TypeError:           # 对字符的异常处理
        print "请输入数字！"


def test_two_divide2():
    while True:
        try:
            x = input('Enter the first number: ')
            y = input('Enter the second number: ')
            value = x/y
            print 'x/y is', value
            break
        except:
            print '无效输入，再来一次！'


if __name__ == '__main__':
    try:
        connect()
    except SocketRefusedError as err:
        logger.error(err)
    except SocketTimeError as err:
        logger.error(err)
    except CustomException as err:
        logger.error("API Unexpected error: %s" % err)
    except Exception:
        logger.error("API bug cause exception.")

    try:
        write()
    except OpenFileError as err:
        logger.error(err)
    except WriteContentError as err:
        logger.error(err)
    except CustomException:
        logger.error("API Error.")
    except Exception:
        logger.error("API Bug")
    else:
        logger.info("ok")

    """将异常处理和和正常流程分开"""

    try:
        x = input('Enter the first number: ')
        y = input('Enter the second number: ')
        print x/y
    except ZeroDivisionError:
        print "输入的数字不能为0！"

    test_two_divide()

    try:
        s = raw_input('Enter something --> ')
        if len(s) > 3:
            raise ShortInputException(len(s), 3)
    except EOFError:
        print '\nWhy did you do an EOF on me?'
    except ShortInputException, x:
        print 'ShortInputException: The input was of length %d, \
              was expecting at least %d' % (x.length, x.atleast)
    else:
        print 'No exception was raised.'

