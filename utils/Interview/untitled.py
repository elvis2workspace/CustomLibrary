# -*- coding: utf-8 -*-
import os
import uuid
import time
# 列表判断后变成小写字母
def choose_str(list_param):
    return [s.lower() for s in list_param if isinstance(s, str)]

# 返回当前目录下所有匹配suffix的文件
def search_suffix(suf):
    return [d for d in os.listdir('.') if d.endswith(suf)]


def get_mac_address():
    mac = uuid.UUID(int=uuid.getnode()).hex[-12:]
    return ":".join([mac[e:e + 2] for e in range(0, 11, 2)])


def get_sys_time():
    return time.strftime('%Y-%m-%d', time.localtime(time.time()))
