#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月8日

@author: zhang.xiuhai
'''
from nose.tools import *
from bin.app import app
from tests.tools import assert_response

def test_index():
    # test_selenium2library our first GET request to /hello
    resp = app.request("/")
    assert_response(resp, status="404")
    
    # test_selenium2library our first GET request to /hello
    resp = app.request("/hello")
    assert_response(resp)
    
    # make sure default values work for the form
    resp = app.request("/hello", method="POST")
    assert_response(resp, contains="Nobody")
    
    # test_selenium2library that we get expected values
    data = {'name': 'Zed', 'greet': 'Hola'}
    resp = app.request("/hello", method="POST", data=data)
    assert_response(resp, contains="Zed")
    
if __name__ == '__main__':
    pass