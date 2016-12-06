#coding:utf-8
import requests
import re

url = r'http://www.renren.com/ajaxLogin'

user = {'email':'15828692608','password':'haixiuzhang1988'}
s = requests.Session()
r = s.post(url,data = user)

html = r.text
print html
visit = []
first = re.compile(r'</span><span class="time-tip first-tip"><span class="tip-content">(.*?)</span>')
second = re.compile(r'</span><span class="time-tip"><span class="tip-content">(.*?)</span>')
third = re.compile(r'</span><span class="time-tip last-second-tip"><span class="tip-content">(.*?)</span>')
last = re.compile(r'</span><span class="time-tip last-tip"><span class="tip-content">(.*?)</span>')
visit.extend(first.findall(html))
visit.extend(second.findall(html))
visit.extend(third.findall(html))
visit.extend(last.findall(html))
for i in visit:
    print i

print '以下是更多的最近来访'
vm = s.get('http://www.renren.com/myfoot.do')
fm = re.compile(r'"name":"(.*?)"')
visitmore = fm.findall(vm.text)
for i in visitmore:
    print i