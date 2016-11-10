# -*- coding: utf-8 -*-

import requests
import  re

url = r'http://www.renren.com/ajaxLogin'

user = {'email':'email', 'password':'pass'}

s = requests.session()
r = s.post(url, data=user)

html = r.text

visit =[]
first =re.compile(r'(.*?)')
second =re.compile(r'(.*?)')
third =re.compile(r'(.*?)')
last =re.compile(r'(.*?)')
visit.extend(first.findall(html))
visit.extend(second.findall(html))
visit.extend(third.findall(html))
visit.extend(last.findall(html))

for i in visit:
    print i

print "follow is visitor."
vm = s.get('http://www.renren.com/myfoot.do')
fm = re.compile(r'"name":"(.*?)"')

visitmore = fm.findall(vm.text)

for i in visitmore:
    print i
