#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年10月9日

@author: zhang.xiuhai
'''
from __future__ import division
        
class DivisionException(Exception):
    def __init__(self, x, y):
        Exception.__init__(self, x, y)
        self.x = x
        self.y = y
        
def main():
    try:
        fh = open("testfile", "w")
        fh.write("This is my test_selenium2library file for exception handling!!")
    except IOError:
        print "Error: can't find file or read data"
    else:
        print "Written content in the file successfully"
        fh.close()
    
    try:
        file("hello.txt", "r")
        print "读文件"
    except IOError:
        print "文件不存在"
    except:                                    #其它异常
        print "程序异常" 
    
    try:
        s = "hello"
        try:
            print s[0] + s[1]
            print s[0] - s[1]
        except TypeError:
            print "字符串不支持减法运算"
    except:
        print "异常"
    
    
    try:
        f = open("hello.txt", "r")
        try:
            print f.read(5)
        except:
            print "读文件异常"
        finally:
            print "释放资源"
            f.close()
    except IOError:
        print "文件不存在" 
        
    try:
        s = None
        if s is None:
            print "s 是空对象"
            raise NameError     #如果引发NameError异常，后面的代码将不能执行
        print len(s)
    except TypeError:
        print "空对象没有长度" 
              
if __name__ == '__main__':
#     main()
    newlist = ['dfad', 'rwe', '34']
    A, B, C = newlist
    print 'newlist element:', A, B, C
    print 'daoxu: ', newlist[::-1]
    try:
        x = 3
        y = 2
        if x % y > 0:            #如果大于0， 则不能被初始化，抛出异常
            print x/y
            raise DivisionException(x, y)
    except DivisionException, div:     #div 表示DivisionException的实例对象
        print "DivisionExcetion: x/y = %.2f" % (div.x/div.y)
      
      
      
      