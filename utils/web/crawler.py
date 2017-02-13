#coding=utf-8
"""
Created on 2015年1月26日

@author: Elvis
"""
import re
import urllib
import urllib2


def getHtml(url):
    page = urllib2.urlopen(url)
    html = page.read()
    return html


def getImg(html):
    reg = r'src="(.+?\.jpg)" pic_ext'
    imgre = re.compile(reg, 0)
    # 以列表的形式列出字符串中所有的匹配项
    imglist = re.findall(imgre, html, 0)
    x = 0
    for imgurl in imglist:
        urllib.urlretrieve(imgurl, '%s.jpg' % x)
        x += 1
    return imglist

if __name__ == '__main__':
    html = getHtml("http://tieba.baidu.com/p/2460150866")
    print getImg(html)