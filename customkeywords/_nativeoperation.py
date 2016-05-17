# -*- coding: utf-8 -*-

'''
Created on 2015年5月8日

@author: zhang.xiuhai
'''
import random
import string

class _NativeOperationKeywords(object):
    '''
    pn2: 生成易记的伪随机密码:
    （1）对于每一个随机字符，我们都按照以下流程处理一遍
    
    （2）随机找到中间点，然后分成两段，翻转合并
    
    （3）用最后的结果的倒数maxmem个字符来做locate标记
    
    （4）每次找到这个locate处，若找不到，locate缩减为locate[1:]，继续找，直到找到为止
    
    （5）我们要的本次的随机字符就是找到的locate处的后一个字符self.data[where+len(locate)+1]，如果locate为空，那么ch就是（2）后的第一个字符，也是随机的
    '''

#     def __init__(self, params):
#         '''
#         Constructor
#         '''
    def __init__(self,filename):  
        self.data = open(filename).read().lower() 
         
    def renew(self,n = 8,maxmem = 3):  
        chars = []  
        for i in range(n):  
            rmdIndex = random.randrange(len(self.data))   
            self.data = self.data[rmdIndex:]+self.data[:rmdIndex]  
            where = i-1  
            locate = chars[-maxmem:]  
            while where < 0 and locate:  
                where = self.data.find(str(locate))  
                locate = locate[1:]  
            ch = self.data[where+len(locate)+1]  
            if not ch.islower():  
                ch = random.choice(string.lowercase)  
            chars.append(ch)  
        return ''.join(chars)        