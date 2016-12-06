#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月15日

@author: zhang.xiuhai
'''

import tornado.web


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")

application = tornado.web.Application([
    (r"/", MainHandler),
])

if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.current().start()
        