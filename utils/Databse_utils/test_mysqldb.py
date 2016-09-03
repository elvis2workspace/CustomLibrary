# -*- coding:utf-8 -*-
# test_mysqldb.py
# just used for a mysql test

"""
Created on 2016-9-2
@author: elvis
"""

# mysqldb
import MySQLdb
import sys


# connect
conn = MySQLdb.connect(host="192.168.20.247", user="root", passwd="root", db='autotest', charset="utf8")
cursor = conn.cursor()

# 创建表 sql 语法
sql_create = """CREATE TABLE `users` (
                `Id` int(11) NOT NULL AUTO_INCREMENT,
                `name` varchar(255) DEFAULT NULL,
                `age` varchar(255) DEFAULT NULL,
                PRIMARY KEY (`Id`)) ENGINE = InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;"""

cursor.execute(sql_create)

# add
sql = "insert into users(name,age) values(%s,%s)"
param = ("tom", str(20))
n = cursor.execute(sql, param)
print n

# 更新
sql = "update user set name=%s where Id=9001"
param = "ken"
n = cursor.execute(sql, param)
print n

# 查询
n = cursor.execute("select * from user")
for row in cursor.fetchall():
    for r in row:
        print r,
    print ""

# 删除
sql = "delete from user where name = %s"
param = "ted"
n = cursor.execute(sql, param)
print n
cursor.close()

# 关闭
conn.close()
