#!/usr/bin/python 
import logging.config
import thread

logging.config.fileConfig('logging.conf') 
# create logger 
logger = logging.getLogger('simpleExample') 
# Define a function for the thread 
def print_time( threadName, delay): 
    logger.debug('thread 1 call print_time function body') 
    count = 0 
    logger.debug('count:%s',count)
    
if __name__ == '__main__':
    thread1 = thread.get_ident()
    print print_time(thread1, 10)