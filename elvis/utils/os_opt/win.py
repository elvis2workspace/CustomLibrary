#coding=utf-8
#!/usr/bin/python

import time
import uuid


def get_mac_address(): 
    mac=uuid.UUID(int = uuid.getnode()).hex[-12:] 
    return ":".join([mac[e:e+2] for e in range(0,11,2)])

def get_sys_time():
    return time.strftime('%Y-%m-%d',time.localtime(time.time()))
#获取mac地址
if  __name__ == '__main__':
    print get_mac_address()
    
    ctime=get_sys_time()
    print "The system time is:"+ctime

