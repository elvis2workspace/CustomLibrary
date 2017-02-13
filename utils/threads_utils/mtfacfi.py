#coding=utf-8
'''
Created on 2015年2月7日

@author: Elvis
'''
import threading
from time import sleep, ctime

loops = (4, 2)

class Mythread(threading.Thread):
    '''
    classdocs
    '''


    def __init__(self, func, args, name=''):
        '''
        Constructor
        '''
        threading.Thread.__init__(self)
        self.name=name
        self.func=func
        self.args=args
    def run(self):
        apply(self.func, self.args)
        
        
def loop(nloop, nsec):
        print 'start loop' , nloop, 'at:', ctime()
        sleep(nsec)
        print 'loop', nloop, 'done at:', ctime()
   
def main():
    print 'starting at:', ctime()
    threads=[] 
    nloops = range(len(loops))       