#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
from bs4 import BeautifulSoup
import os

'''
下载图片或者文件也可以使用urlretrieve模块
from urllib import request
request.urlretrieve('','1.jpg')
'''
# proxies = {
#     "http": "http://175.155.240.127:808",
#     "https": "http://114.239.149.110:808",
# }
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36',
    'Connection': 'keep-alive'}
url = 'http://www.wmpic.me/86253'
r = requests.get(url, headers=headers)
soup = BeautifulSoup(r.text, 'html.parser')
result = soup.select('#content > div.content-c > center > img')
links = []
for content in result:
    links.append(content.get('src'))

if not os.path.exists('pictures'):
    os.makedirs('pictures')

i = 0

pwd_str = os.getcwd()
for link in links:
    i += 1
    filename = 'pictures/' + str(i) + '.jpg'
    print filename

    ir = requests.get(link)
    with open(filename, 'wb') as fo:
        fo.write(ir.content)
