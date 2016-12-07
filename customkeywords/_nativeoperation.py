#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
Created on 2015年5月8日
@author: zhang.xiuhai
'''

import random
import string
from xml.dom import minidom



class _NativeOperationKeywords(object):
    """
    pn2: 生成易记的伪随机密码:
    （1）对于每一个随机字符，我们都按照以下流程处理一遍
    （2）随机找到中间点，然后分成两段，翻转合并
    （3）用最后的结果的倒数maxmem个字符来做locate标记
    （4）每次找到这个locate处，若找不到，locate缩减为locate[1:]，继续找，直到找到为止
    （5）我们要的本次的随机字符就是找到的locate处的后一个字符self.data[where+len(locate)+1]，如果locate为空，那么ch就是（2）后的第一个字符，也是随机的
    """


    def __init__(self, filename):
        self.data = open(filename).read().lower() 

    # public
    def renew(self, n=8, max_mem=3):
        chars = []  
        for i in range(n):  
            rmd_index = random.randrange(len(self.data))
            self.data = self.data[rmd_index:]+self.data[:rmd_index]
            where = i-1  
            locate = chars[-max_mem:]
            while where < 0 and locate:  
                where = self.data.find(str(locate))  
                locate = locate[1:]  
            ch = self.data[where+len(locate)+1]  
            if not ch.islower():  
                ch = random.choice(string.lowercase)  
            chars.append(ch)  
        return ''.join(chars)

    def setup_xml(self, test_name, user_pin="123456", so_pin="12345678", loop_times=None, data_len=None):
        """
        Setup the xml file:data.xml, config param include test_name, user_pin, so_pin, loop_times,
        data_len.
        example:
        | setup xml | test_name | user_pin | so_pin | loop_times | data_len |
        | setup xml | 101 | 123456 | 12345678 | 50 | 32 |
        """
        doc = minidom.Document()
        doc.standalone = True
        root_node = doc.createElement("root")
        doc.appendChild(root_node)
        test_node = doc.createElement("Test")
        # book_node.setAttribute("isbn", "34909023")
        root_node.appendChild(test_node)
        test_name_node = doc.createElement("Testname")
        user_pin_node = doc.createElement("Userpin")
        so_pin_node = doc.createElement('Sopin')
        loop_times_node = doc.createElement('Looptimes')
        data_len_node = doc.createElement('Datalen')
        test_node.appendChild(test_name_node)
        test_node.appendChild(user_pin_node)
        test_node.appendChild(so_pin_node)
        test_node.appendChild(loop_times_node)
        test_node.appendChild(data_len_node)
        test_name_text_node = doc.createTextNode(test_name)
        test_name_node.appendChild(test_name_text_node)
        user_pin_text_node = doc.createTextNode(user_pin)
        user_pin_node.appendChild(user_pin_text_node)
        so_pin_text_node = doc.createTextNode(so_pin)
        so_pin_node.appendChild(so_pin_text_node)
        loop_times_text_node = doc.createTextNode(loop_times)
        loop_times_node.appendChild(loop_times_text_node)
        data_len_text_node = doc.createTextNode(data_len)
        data_len_node.appendChild(data_len_text_node)
        # doc.writexml(f, "/t", "/t", "/n", "utf-8")
        f = open("data.xml", "w")
        test_node.toprettyxml(encoding="utf-8")
        doc.writexml(f, encoding="utf-8")
        f.close()


if __name__ == '__main__':
    native_opt_instance = _NativeOperationKeywords("./runbat.py")
    native_opt_instance.setup_xml(test_name="101", loop_times="50", data_len="32")
