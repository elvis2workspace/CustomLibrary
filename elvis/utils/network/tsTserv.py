#coding=utf-8
'''
Created on 2015年1月29日

@author: Elvis
'''
from socket import *
from time import ctime

HOST=''
PORT=21567
BUFSIZ=1024
ADDR = (HOST, PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)

while True:
    print 'waiting for message...'
    data, addr = tcpSerSock.recvfrom(BUFSIZ)
    tcpSerSock.sendto('[%s] %s' % (ctime(), data), addr)
    print '....received from and return to:', addr

tcpSerSock.close() 