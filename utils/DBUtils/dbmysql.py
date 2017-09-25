# coding=utf-8
# !/usr/bin/env python
"""
Created on 2015年3月31日

@author: Administrator

"""

import MySQLdb

# connect
conn = MySQLdb.connect(host="localhost", user="root", passwd="000000", db="test", charset="utf8")
cursor = conn.cursor()

# add
sql = "insert into user1(name,age) values(%s,%s)"
param = ("tom", str(20))
n = cursor.execute(sql, param)
print n

# 更新
sql = "update user set name=%s where Id=9001"
param = ("ken")
n = cursor.execute(sql, param)
print n

# 查询
n = cursor.execute("select * from user")
for row in cursor.fetchall():
    for r in row:
        print r,
print ""

# 删除
sql = "delete from user where name=%s"
param = ("ted")
n = cursor.execute(sql, param)
print n
cursor.close()

# 关闭
conn.close()
