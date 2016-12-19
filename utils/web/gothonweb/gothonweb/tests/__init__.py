#!/usr/bin/python

def getInput():
    name = raw_input("what's your name:")
    
    if name.endswith('tank'):
        print "hello tank."
    elif name.endswith('zhang'):
        print "hello zhang."
    else:
        print "hello xiuhai."
        

print getInput()