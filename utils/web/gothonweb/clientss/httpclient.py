#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年10月22日

@author: zhang.xiuhai
'''
import requests

# Base URL being accessed
url = 'http://httpbin.org/get'

# Dictionary of query parameters (if any)
parms = {
   'name1' : 'value1',
   'name2' : 'value2'
}

# Encode the query string
querystring = parse.urlencode(parms)

# Make a GET request and read the response
u = requests.urlopen(url+'?' + querystring)
resp = u.read()
