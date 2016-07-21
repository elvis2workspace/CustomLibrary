# -*- coding: utf-8 -*-

"""
Created on 2015年5月8日
@author: zhang.xiuhai
"""

import random
import string
from random import choice
import re


class _OutputPwdKeywords(object):
    """classdocs"""

    def __init__(self, filename):
        """Constructor"""
        self.data = open(filename).read().lower()

    def renew(self, n=8, maxmem=3):
        chars = []  
        for i in range(n):  
            rmd_index = random.randrange(len(self.data))
            self.data = self.data[rmd_index:]+self.data[:rmd_index]
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

    def gen_passwd(self, length=8, chars=string.letters+string.digits):
        """pn1: 生成随机密码"""
        return ''.join([choice(chars) for i in range(length)])

    def calcute_apache_ip_hits(self, logfile_pathname):
        """
        pn3:统计Apache中每个IP的点击率
        思想：
        （1）按行读内容，正则匹配
        （2）检查IP范围，min 和 max 的妙用
        （3）存在+1，不存在置1：list[ip] = list.get(ip,0) + 1，这里的get中的0是指获取不到ip的时候的默认值
        """

        ip_specs = r'\.'.join([r'\d{1,3}']*4)
        re_ip = re.compile(ip_specs)  
      
        ip_hit_listing = {}
        contents = open(logfile_pathname,"r")  
        for line in contents:  
            match = re_ip.match(line)  
            if match :  
                ip = match.group()  
                # 检查正确性
                try:  
                    quad = map(int,ip.split('.'))  
                except ValueError:  
                    pass  
                else:  
                    # 如果ip存在就+1，不存在就设置为1
                    if len(quad) == 4 and min(quad) >= 0 and max(quad) <= 255:   
                        ip_hit_listing[ip] = ip_hit_listing.get(ip,0) + 1
        return ip_hit_listing
