#!/usr/bin/env python

import time

from twisted.internet import reactor


def notThreadSafe(x):
    """do something that isn't thread-safe"""
    #....

def threadSafeScheduler():
    """Run in thread-safe manner."""
    reactor.callFromThread(notThreadSafe, 3)
    
def aSillyBlockingMethod(x):
    time.sleep(10)
    print x

reactor.callInThread(aSillyBlockingMethod, "10 seconds have passed")
reactor.run()
