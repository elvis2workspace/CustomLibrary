#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 
@author: elvis.zhang
@license: Apache Licence
@file: selenium-test.py
@time: 25/09/2017 10:02

"""

# from splinter import Browser
from selenium import webdriver
from selenium.webdriver.common.keys import Keys

def func():
    pass


class Main(object):
    def __init__(self):
        pass


if __name__ == '__main__':
    url = 'https://github.com/mark-adams/docker-chromium-xvfb/blob/master/samples/python3/test_google.py'
    driver = webdriver('chrome')
    cont = driver.get(url)
    print cont
    element = driver.find_by_xpath('/html/body/div[1]/header/div/div/div/div/form/label/input[1]')
    element.type('aaaa')