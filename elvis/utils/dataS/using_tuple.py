#coding=utf-8
'''
Created on 2015年3月5日

@author: Elvis
'''

#!/usr/bin/python
#Filename:using_tuple.py
#print_tuple

def using_continue():
    '''using_continue: a example for continue '''
    while True:
        s = raw_input('Enter something:')
        if s == 'quit':
            break
        if len(s) < 3:
            continue
        print 'Input is of sufficient length'

print using_continue.__doc__
print using_continue()
      
def powersum(power, *args):
    '''Powersum:Return the sum of each argurments raised to specified power.'''
    total = 0
    for i in args:
        total += pow(i, power)
    return total

print powersum.__doc__
print powersum(2, 3, 4)

def printMax(x, y):
    '''printMax:Print the maximum of two numbers.
    The two values must be integers.'''
    
    x = int(x)
    y = int(y)
    
    if x > y:
        print x, 'is maximum'
    else:
        print y, 'is maximum'
        
print printMax.__doc__
printMax(3,5)

def using_tuple():
    '''Using_tuple:a example for using tuple.'''
    age = 22
    name = 'Swaroop'
    
    print '%s is %d years old' % (name, age)
    
    zoo=('wolf', 'elephant', 'penguin')
    print 'Number of animals in the zoo is',len(zoo)
    
    new_zoo = ('monkey', 'dolphin', zoo)
    
    print 'Number of animals in the new zoo is', len(zoo)
    print 'All animals in new zoo are', new_zoo
    print 'Animals brought from old zoo are', new_zoo[2]
    print 'Last animal brought from old zoo is',new_zoo[2][2]

print using_tuple.__doc__
using_tuple()