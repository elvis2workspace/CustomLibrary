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
conn = MySQLdb.connect(host="10.2.1.8", user="hcb_open", passwd="dka5tredyiuyocp", db='db_loc_manage', charset="utf8")
cursor = conn.cursor()

# # 创建表 sql 语法
# sql_create = """CREATE TABLE `users` (
#                 `Id` int(11) NOT NULL AUTO_INCREMENT,
#                 `name` varchar(255) DEFAULT NULL,
#                 `age` varchar(255) DEFAULT NULL,
#                 PRIMARY KEY (`Id`)) ENGINE = InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;"""
#
# cursor.execute(sql_create)
#
# # add
# sql = "insert into users(name,age) values(%s,%s)"
# param = ("tom", str(20))
# n = cursor.execute(sql, param)
# print n
#
# # 更新
# sql = "update user set name=%s where Id=9001"
# param = "ken"
# n = cursor.execute(sql, param)
# print n

# 查询
n = cursor.execute("select * from loc_transport_line")
for row in cursor.fetchall():
    for r in row:
        print r,
    print ""

# # 删除
# sql = "delete from user where name = %s"
# param = "ted"
# n = cursor.execute(sql, param)
# print n
cursor.close()

# 关闭
conn.close()
