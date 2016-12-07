#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月21日

@author: zhangxiuhai
'''

class Room(object):
    '''
    classdocs
    '''


    def __init__(self, name, description):
        '''
        Constructor
        '''
        self.name = name
        self.descrition = description
        self.paths = {}
        
    def go(self, direction):
        return self.paths.get(direction, None)
    
    def add_paths(self, paths):
        self.paths.update(paths)