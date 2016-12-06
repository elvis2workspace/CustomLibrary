#coding=utf-8
'''
Created on 2015年3月14日

@author: Elvis
'''

fin = file('test.txt', 'r')
fout = file('test_1.txt', 'w')
while True:
    line = fin.readline()
    if len(line)==0:
        break
    
    fout.write(line)

fin.close()
fout.close()
