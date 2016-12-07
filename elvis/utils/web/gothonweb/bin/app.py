#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月7日

@author: zhang.xiuhai
'''
import os

import web

urls = (
        '/hello', 'Log'
)

app = web.application(urls, globals())

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p)
)

path = PATH(
            '../templates'
        )
# render = web.template.render('D:\workspace\myPython\gothonweb1\templates/')
# root = os.path.dirname(__file__)
# path = os.path.join(root, '..', 'templates/')
print path

# render = web.template.render(r'D:\workspace\myPython\gothonweb1\templates', base="layout")

render = web.template.render(r'D:\workspace\myPython\gothonweb\templates')

print render

class Index():
    def GET(self):
        greeting = "Hello World!"
        return render.index(greeting = greeting)

# class home():
#     def GET(self):
#         form = web.input(name="Nobody", greet=None)
#         if form.greet:
#             greeting = "%s,Welcome to Auto Test report and record. %s" % (form.name, form.greet)    
#             return greeting
#         else:
# #             return "ERROR: greet is required."
#             return render.hello_form
#     
#     def POST(self):
#         form = web.input(name="Nobody", greet="Hello")
#         greeting = "%s, %s" % (form.greet, form.name)
#         return render.index(greeting = greeting)
    
class Hello():
    def GET(self):
        return render.hello_form()
     
     
    def POST(self):
        form = web.input(name="Nobody", greet="Hello")
        greeting = "%s, %s" % (form.greet, form.name)
        return render.index(greeting = greeting)

class Log():
    def GET(self):
        return render.log()
    
    def POST(self):
        pass
    
if __name__ == '__main__':
    app.run()