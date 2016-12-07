#!/usr/bin/env python

def fib():
    '''
     Fibonacci series:
     the sum of two elements defines the next
    '''
    
    a, b = 0, 1
    while b < 100:
        print b,
        a, b = b, a+b
        
def primeLists():
    for n in range(2, 10):
        for x in range(2, n):
            if n % x == 0:
                print n, 'equals', x, '*',n/x
                break
        else:
            #loop fell through without finding a factor
            print n,'is a prime number.'
            
if __name__ == '__main__':
    fib()
    print '\n'
    primeLists()