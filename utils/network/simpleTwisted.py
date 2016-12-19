#coding=utf-8
'''
Created on 2015年1月30日

@author: Elvis
'''
import sys
from twisted.internet import reactor
from twisted.internet.protocol import Protocol, Factory
print sys.modules['twisted.internet.reactor']

# 定义你Protocol类
class SimpleLogger(Protocol):

    def connectionMade(self):
        print 'Got connection from', self.transport.client
    def connectionLost(self, reason):
        print self.transport.client, 'disconnected'
    def dataReceived(self, data):
        print data


# 实例化Factory

factory = Factory()

# 设置factory的protocol属性以便它知道使用哪个protocol与客户端通信(这就是所谓的你的自定义
# protocol)

factory.protocol = SimpleLogger

# 监听指定的端口

reactor.listenTCP(1234, factory)

# 开始运行主程序
reactor.run()

