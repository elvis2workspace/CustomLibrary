#!/usr/bin/env python
#coding:gbk
import os
import logging
import pybcs

# 设置日志级别
pybcs.init_logging(logging.INFO)

# 请修改这里
AK ='' # 请改为你的AK
SK ='' # 请改为你的SK

BUCKET=''

# 改为你的bucket
bcs = pybcs.BCS('http://bcs.duapp.com/', AK, SK, pybcs.HttplibHTTPC) #这里可以显式选择使用的HttpClient, 可以是:
# #HttplibHTTPC
# #PyCurlHTTPC

def listFile(path, isDeep=True):
    _list = []
    if isDeep:
        try:
            for root, dirs, files in os.walk(path):
                for fl in files:
                    _list.append('%s%s'% (root, fl))
        except:
            pass
        else:
            for fn in glob.glob( path + os.sep +'*' ):
                if not os.path.isdir(fn):
                    _list.append('%s'% path + os.sep + fn[fn.rfind('')+1:])

    return _listb = bcs.bucket(BUCKET)

dir = os.getcwd()+'1' ##取一个和当前脚本同目录下的目录名
filelist = listFile(dir)

for i in filelist:
    ic = i.replace(os.getcwd(),'') # 去掉os.getcwd()前缀
    upi = ic.replace('','/') # 将替换为/
    o = b.object(upi)
    o.put_file(i)