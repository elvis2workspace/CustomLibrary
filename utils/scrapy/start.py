#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
from robobrowser import RoboBrowser

# b = RoboBrowser(history=True)
# b.open('http://itest.info/courses/2')
#
# title = b.select('.headline h2')
# print title[0].text
#
# infos = b.select('h4')
#
# for info in infos:
#     print info.text

# 访问python selenium自动化测试班的页面
url = 'http://itest.info/courses/2'
b = RoboBrowser(history=True)
b.open(url)

# 获取这个班的名称--python selenium自动化测试班
class_name = b.select('.headline h2')
if len(class_name) > 0:
    print "not None."
    print class_name[0].text

# 获取这个班的描述--独一无二的超低价培训-口碑之选
class_desc = b.select('.tag-box')
if len(class_desc) > 0:
    print class_desc[0].text

# 获取开班的时间--**第五期报名截止2015年1月17日，开课时间1月17日
class_time = b.select('h4')
if len(class_time) > 0:
    print class_time[0].text

# 获取授课老师信息--虫师
teacher = b.select('.thumbnail-style h3')
if len(teacher) > 0:
    print teacher[0].text

# 获取报名方式--**课程咨询请联系QQ：12079456
qq = b.find(text=re.compile('QQ'))
print qq

# 获取selenium进阶群的群号--**技术交流+selenium 进阶群：189116036
qq_group = b.find(text=re.compile('\+selenium'))
print qq_group


# find方法
# find方法是返回页面上符合条件的第1个元素。

def find_func():
    url = 'http://itest.info/courses/2'
    b = RoboBrowser(history=True)
    b.open(url)

    # 通过tag name抓取

    # <title>重定向科技</title>
    title = b.find('title')
    print title.text

    # 通过属性(attribute)抓取

    # <img id="logo-header"src="/assets/logo-0648b8fb283a9802457da74f0c157b12.png" />
    img = b.find(id='logo-header')
    if img is not None:
        print img['src']

    # <a href="/courses/4">android测试工具自制班</a>
    # print b.find(href='/courses/4').text

    # <li class="active">python selenium自动化测试班</li>
    result_s = b.find(class_='active', text=re.compile('python'))
    if result_s is not None:
        print result_s.text
    else:
        print "Return String is Null."


# find_all方法
# find_all方法的用法跟find基本相同，但是find_all会返回所有符合条件的tag的集合(ResultSet)。
def find_all_func():
    url = 'http://itest.info/courses/2'
    b = RoboBrowser(history=True)
    b.open(url)
    # 页面上所有的a
    all_links = b.find_all('a')
    for link in all_links:
        print link.text
    # 页面上所有class是container的div
    divs = b.find_all(class_='container')
    print divs
    # limit 参数控制返回的元素个数# 页面上前2个p
    first_two_p = b.find_all('p', limit=2)
    print first_two_p
    # 如果第1个参数是列表则返回相匹配的集合# 页面上所有的meta和titleprint b.find_all(['meta', 'img'])


# select方法
# select方法是我最喜欢的方法，该方法支持css选择器(可惜不是全部)，返回的是list。

def select_func():
    url = 'http://itest.info/courses/2'
    b = RoboBrowser(history=True)
    b.open(url)

    # 页面上所有的a
    all_links = b.select('a')
    for link in all_links:
        print link.text

    # 页面上所有class是container的div
    divs = b.select('.container')
    print len(divs)
    # 其他技巧
    #     - 找到页面上所有具有id属性的元素 b.find_all(id=True)
    #     - 不递归查找元素。也就是说只在的直接子后代中查找 b.find('p', recursive=False)


if __name__ == "__main__":
    find_all_func()
    find_func()
    select_func()