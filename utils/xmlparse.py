#!/usr/bin/env python
#coding=utf-8

'''
Created on 2016年1月27日

@author: zhang.xiuhai
'''
import re
import urllib

local = 'D:\\testdir\\'
def getHtml(url):
    page = urllib.urlopen(url)#创建一个表示远程url的类文件对象，然后像本地文件一样操作这个类文件对象来获取远程数据。
    html = page.read()
    return html

def getImg(html):
    reg = r'src="(.+?\.jpg)"'
    imgre = re.compile(reg)
    imglist = re.findall(imgre, html)
    x = 0
    for imgurl in imglist:
        urllib.urlretrieve(imgurl, local+'%s.jpg' % x, callbackfunc)#将远程数据下载在本地当前目录，命名规则(回掉函数显示进度)
        x+=1
        
    #return imglist

def callbackfunc(blocknum, blocksize, totalsize):
    '''回调函数
    @blocknum:已经下载的数据块
    @blocksize:数据块的大小
    @totalsize:远程文件的大小
    '''
    percent = 100.0*blocknum*blocksize/totalsize
    if percent > 100:
        percent = 100
    print "%.2f%%"% percent
    
if __name__ == '__main__':
    html = getHtml("http://image.baidu.com/")
    print html
    
#     for item in getImg(html):
#         print item
        
    print getImg(html)