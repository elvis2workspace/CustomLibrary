#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年10月20日

@author: zhang.xiuhai
'''

import filecmp
import os
import random
import re
import shutil
import string
import winsound
from random import choice

MAXVERSIONS = 100 

'''
pn4: 检查你的Windows声音系统
''' 
try:  
    winsound.PlaySound("*",winsound.SND_ALIAS)  
except RuntimeError,e:  
    print 'Sound system has problems,',e  
else:  
    print 'Sound system is OK'
    
'''
pn1: 生成随机密码
'''  
def GenPasswd(length = 8,chars = string.letters+string.digits):  
    return ''.join([choice(chars) for i in range(length)])  

'''
pn2: 生成易记的伪随机密码:
（1）对于每一个随机字符，我们都按照以下流程处理一遍

（2）随机找到中间点，然后分成两段，翻转合并

（3）用最后的结果的倒数maxmem个字符来做locate标记

（4）每次找到这个locate处，若找不到，locate缩减为locate[1:]，继续找，直到找到为止

（5）我们要的本次的随机字符就是找到的locate处的后一个字符self.data[where+len(locate)+1]，如果locate为空，那么ch就是（2）后的第一个字符，也是随机的
'''
class password(object):  
    def __init__(self,filename):  
        self.data = open(filename).read().lower()  
    def renew(self,n = 8,maxmem = 3):  
        chars = []  
        for i in range(n):  
            rmdIndex = random.randrange(len(self.data))   
            self.data = self.data[rmdIndex:]+self.data[:rmdIndex]  
            where = -1  
            locate = chars[-maxmem:]  
            while where < 0 and locate:  
                where = self.data.find(str(locate))  
                locate = locate[1:]  
            ch = self.data[where+len(locate)+1]  
            if not ch.islower():  
                ch = random.choice(string.lowercase)  
            chars.append(ch)  
        return ''.join(chars)    

'''
pn3:统计Apache中每个IP的点击率
思想：
（1）按行读内容，正则匹配
（2）检查IP范围，min 和 max 的妙用
（3）存在+1，不存在置1：list[ip] = list.get(ip,0) + 1，这里的get中的0是指获取不到ip的时候的默认值
'''
def calcuteApacheIpHits(logfile_pathname):  
    ip_specs = r'\.'.join([r'\d{1,3}']*4)  
    re_ip = re.compile(ip_specs)  
  
    ipHitListing = {}  
    contents = open(logfile_pathname,"r")  
    for line in contents:  
        match = re_ip.match(line)  
        if match :  
            ip = match.group()  
            #检查正确性  
            try:  
                quad = map(int,ip.split('.'))  
            except ValueError:  
                pass  
            else:  
                #如果ip存在就+1，不存在就设置为1  
                if len(quad) == 4 and min(quad) >= 0 and max(quad) <= 255:   
                    ipHitListing[ip] = ipHitListing.get(ip,0) + 1  
    return ipHitListing

'''
备份文件
''' 
def backup(tree_top,bakdir_name = 'bakdir'):  
    for root,subdirs,files in os.walk(tree_top):  
        #join链接出每个root下的子目录bakdir  
        backup_dir = os.path.join(root,bakdir_name)  
        #确保每个root下都有个子目录叫bakdir  
        if not os.path.exists(backup_dir):  
            os.makedirs(backup_dir)  
        #bakdir下的不递归处理  
        subdirs[:] = [d for d in subdirs if d != bakdir_name]  
  
        for file in files:  
            filepath = os.path.join(root,file)  
            destpath = os.path.join(backup_dir,file)  
            #检查版本，共有MAXVERSIONS个版本  
            for index in xrange(MAXVERSIONS):  
                backup = "%s.%2.2d" % (destpath,index)  
                if not os.path.exists(backup):  
                    break  
            if index > 0:  
                old_backup = "%s.%2.2d" % (destpath,index-1)  
                #abspath = os.path.abspath(filepath)#filepath本来就是绝对路径  
                try:  
                    #如果备份和源文件一致，continue不处理  
                    if os.path.isfile(old_backup) and filecmp.cmp(filepath,old_backup,shallow = False):  
                        continue  
                except OSError:  
                        pass  
            try:  
                shutil.copy(filepath,backup)  
            except OSError:  
                pass
            
  
if __name__ == '__main__':
    p = password("F:\\Program Files (x86)\\Android\\android-sdk\\docs\\assets\\GPL-LICENSE.txt")  
    print 'pn2: 生成易记的伪随机密码', p.renew(12)  
    
    for i in range(6):  
        print 'pn1: 生成随机密码', GenPasswd(12)
        
    print 'pn3: 统计Apache中每个IP的点击率', calcuteApacheIpHits("log.txt")
    
    backup("c:\\test_selenium2library")