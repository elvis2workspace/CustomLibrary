# -*- coding: utf-8 -*-

# send get request with parameters, include key and value in a dictionary

import requests
import json

r = requests.get('http://httpbin.org/get')
print r.text
print r.status_code
print r.headers

# 发送带参数的get请求,将key与value放入一个字典中，通过params参数来传递,其作用相当于urllib.urlencode
p_load = {'q': '测试'}
r = requests.get('http://www.baidu.com', params=p_load)
# r.cookies['']

print r.url
print r.status_code

# send post request by data parameter
pay_load = {'a': '杨', 'b': 'hello'}
r = requests.post("http://httpbin.org/post", data=pay_load)

print r.text

# send post requests by data json
pay_load_json = {'a': '杨', 'b': 'hello'}

r = requests.post("http://httpbin.org/post", data=json.dumps(pay_load_json))

print r.text

# send post requests by data file types
url = 'http://httpbin.org/post'
files = {'file': open('/Users/Elvis/Pictures/architecture-big.png', 'rb')}
r = requests.post(url, files=files)
print r.text

# customize headers, communication by headers parameters

url1 = 'https://api.github.com/some/endpoint'
pay_load = {'some': 'data'}
headers = {'content-type': 'application/json'}
r = requests.post(url1, data=json.dumps(pay_load), headers=headers)

print r.text
print "r.headers: ", r.headers
