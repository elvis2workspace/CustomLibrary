#!/usr/bin/env python
#coding:gbk
import os
import logging
import pybcs

# ������־����
pybcs.init_logging(logging.INFO)

# ���޸�����
AK ='' # ���Ϊ���AK
SK ='' # ���Ϊ���SK

BUCKET=''

# ��Ϊ���bucket
bcs = pybcs.BCS('http://bcs.duapp.com/', AK, SK, pybcs.HttplibHTTPC) #���������ʽѡ��ʹ�õ�HttpClient, ������:
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

dir = os.getcwd()+'1' ##ȡһ���͵�ǰ�ű�ͬĿ¼�µ�Ŀ¼��
filelist = listFile(dir)

for i in filelist:
    ic = i.replace(os.getcwd(),'') # ȥ��os.getcwd()ǰ׺
    upi = ic.replace('','/') # ���滻Ϊ/
    o = b.object(upi)
    o.put_file(i)