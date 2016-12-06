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
        
central_corridor = Room("Central Corridor",
                            )
    
laser_weapon_armory = Room("Laser Weapon Armory",
                               )
    
the_bridge = Room("The Bridge",
                      )
the_bridge = Room("The Bridge",
                      )
the_end_winner = Room("The End",
                          )
    
the_end_loser = Room("The End",
                         )
    
    
escape_pod.add_paths({
                      '2': the_end_winner,
                      '*': the_end_loser
                      })

generic_death = Room("death", "You died.")

the_bridge.add_paths({
                      'throw the bomb': generic_death,
                      'slowly place the bomb': escape_pod
                      })

laser_weapon_armory.add_paths({
                               '0132': the_bridge,
                               '*': generic_death
                               })

central_corridor.add_paths({
                            'shoot!': generic_death,
                            'dodge!': generic_death,
                            'tell a joke': laser_weapon_armory
                            })

START = central_corridor
