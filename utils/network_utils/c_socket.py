# coding=utf-8
"""
Created on 2015年1月24日

@author: Elvis
"""

import asyncore
import socket


class HTTPClient(asyncore.dispatcher):
    """
    classdocs
    """

    def __init__(self, host, path):
        """
        Constructor
        """
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect((host, 80))
        self.buffer = 'GET %s HTTP/1.0\r\n\r\n' % path

    def handle_connect(self):
        pass

    def handle_close(self):
        self.close()

    def handle_read(self):
        print self.recv(8192)

    def writable(self):
        return len(self.buffer) > 0

    def handle_write(self):
        sent = self.send(self.buffer)
        self.buffer = self.buffer[sent:]


client = HTTPClient('localhost', '/')
asyncore.loop()
