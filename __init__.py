# -*- coding: utf-8 -*-

import os
from keywords import *
from version import VERSION

THIS_DIR = os.path.dirname(os.path.abspath(__file__))
execfile(os.path.join(THIS_DIR, 'version.py'))

__version__ = VERSION


class CustomLibrary(
        _CustomAndroidKeywords,
):

    """
    这里也可以装x 的写上我们创建的CustomLibrary 如何如何。
    """

    ROBOT_LIBRARY_SCOPE = 'GLOBAL'

    ROBOT_LIBRARY_VERSION = VERSION
    
    def __init__(self):
        print CustomLibrary.__bases__
        for base in CustomLibrary.__bases__:
            base.__init__(self)