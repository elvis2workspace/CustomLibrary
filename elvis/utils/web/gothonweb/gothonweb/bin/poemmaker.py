#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月17日

@author: zhang.xiuhai
'''
import os.path

import tornado.options
from tornado.options import define, options

define("port", default=8000, help="run on the given port.", type=int)

# PATH = lambda p: os.path.abspath(
#     os.path.join(os.path.dirname(__file__), p)
# )
#  
# path = PATH(
#             '../templates'
#         )
# 
# print path

class IndexHandler(tornado.web.RequestHandler):
    def get(self):
        self.render('log.html')
        
class PoemPageHandler(tornado.web.RequestHandler):
    def post(self):
        noun1 = self.get_arguments('noun1')
        noun2 = self.get_arguments('noun2')
        verb = self.get_arguments('verb')
        noun3 = self.get_arguments('noun3')
        self.render('poem.html', roads=noun1, wood=noun2, made=verb, 
            difference=noun3)
        
if __name__ == '__main__':
    tornado.options.parse_command_line()
    app = tornado.web.Application(
        handlers=[(r'/', IndexHandler), (r'/poem', PoemPageHandler)], 
        template_path=os.path.join(os.path.dirname(__file__), "../templates"))
        
                      
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()