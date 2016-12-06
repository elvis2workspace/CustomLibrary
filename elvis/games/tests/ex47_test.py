#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月21日

@author: zhangxiuhai
'''
import unittest

from ex47.game import Room


class Ex47Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass

    def test_room(self):
        gold = Room("GoldRoom", 
        """This room has gold in it you can grab. There's a
        door to the north.""")
        self.assert_equal(gold.name, "GoldRoom")
        self.assert_equal(gold.paths, {})

    def test_room_paths(self):
        center = Room("Center", "Test room in the center.")
        north = Room("North", "Test room in the north.")
        south = Room("South", "Test room in the south.")
    
        center.add_paths({'north': north, 'south': south})
        self.assert_equal(center.go('north'), north)
        self.assert_equal(center.go('south'), south)
        
    def test_map(self):
        start = Room("Start", "You can go west and down a hole.")
        west = Room("Trees", "There are trees here, you can go east.")
        down = Room("Dungeon", "It's dark down here, you can go up.")
    
        start.add_paths({'west': west, 'down': down})
        west.add_paths({'east': start})
        down.add_paths({'up': start})
    
        self.assert_equal(start.go('west'), west)
        self.assert_equal(start.go('west').go('east'), start)
        self.assert_equal(start.go('down').go('up'), start)

    # 构造测试集  
    def suites(self):  
        suite = unittest.TestSuite()  
        suite.addTest(Ex47Test("test_room"))
        suite.addTest(Ex47Test("test_room_paths"))
        suite.addTest(Ex47Test("test_map"))
        return suite 
    
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main(defaultTest = 'suites')