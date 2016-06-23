# -*- coding: utf-8 -*-

import os
from customkeywords import *
from customversion import CUSTOM_LIB_VERSION

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
execfile(os.path.join(THIS_DIR, 'customversion.py'))

__version__ = CUSTOM_LIB_VERSION


class CustomLibrary(
        _CustomAndroidKeywords,
        # _NativeOperationKeywords,
        # _OutputResultKeywords,
        # _OutputRunnerKeywords,
        # _OutputPwdKeywords,
):

    """
    这里也可以装x 的写上我们创建的CustomLibrary 如何如何。
    """

    ROBOT_LIBRARY_SCOPE = 'GLOBAL'
    ROBOT_LIBRARY_VERSION = CUSTOM_LIB_VERSION
    
    def __init__(self):
        for base in CustomLibrary.__bases__:
            base.__init__(self)
