#coding=utf-8
'''
Created on 2015年2月4日

@author: Elvis
'''
import sys
from twisted.protocols.basic import LineReceiver
from twisted.python import log
from twisted.internet.protocol import ServerFactory
from twisted.internet import reactor

class CmdProtocol(LineReceiver):
    '''
    classdocs
    protocol类的方法都对应着一种事件处理
    '''
    delimiter = "\n"
    
    def __init__(self, params):
        '''
        Constructor
        '''
    def connectionMade(self):
        self.client_ip = self.transport.getPeer()[1]
        log.msg("Client connection from %s" % self.client_ip)
        if len(self.factory.clients) >= self.factory.clients_max:
            log.msg("Too many connections. bye !")
            self.client_ip = None
            self.transport.loseConnection()
        else:
            self.factory.clients.append(self.client_ip) 
            
        def lineReceived(self, line):
            log.msg('Cmd received from %s: %s' % (self.client_ip, line))
            
class MyFactory(ServerFactory):
        
        protocol = CmdProtocol
        
        def __init__(self, clients_max=10):
            self.clients_max = clients_max
            self.clients = []
            
log.startLogging(sys.stdout)
reactor.listenTCP(9999, MyFactory(2))
reactor.run()
            
                   