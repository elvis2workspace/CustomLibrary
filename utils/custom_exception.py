# -*- coding: utf-8 -*-

from robot.api import logger


class CustomException(Exception):
    """Root exception for all exceptions raised by this module."""


class SocketTimeError(CustomException):
    pass


class SocketRefusedError(CustomException):
    pass

    def __init__(self, str_value):
        self.str = str_value

    def __str__(self):
        return self.str


class OpenFileError(CustomException):
    pass


class WriteContentError(CustomException):
    pass


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

