# -*- coding: utf-8 -*-


class TestLibrary:

    def __init__(self, mode='default'):
        """Creates new TestLibrary. 'mode' argument is used to determine mode."""
        self.mode = mode

    def some_keyword(self, args):
        """Does something based on given 'args'.

        What is done depends on the 'mode' specified when 'importing' the library.
        """
        if self.mode == 'secret':
            pass
