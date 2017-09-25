#!/usr/bin/env python
# -*- coding:utf-8 -*-

# t_mysqldb.py
# just used for a mysql_utils test

"""
Created on 2016-9-2
@author: elvis
"""

# mysqldb
import MySQLdb

# connect
conn = MySQLdb.connect(host="10.12.32.93", user="root", passwd="root@mysql", db='auto', charset="utf8")
cursor = conn.cursor()


# 创建表 sql 语法
def _build_tables(table_name):
    sql_create = """CREATE TABLE """ + table_name + """ (
                `Id` int(11) NOT NULL AUTO_INCREMENT,
                `name` varchar(255) DEFAULT NULL,
                `class` varchar(255),
                `age` varchar(255) DEFAULT NULL,
                PRIMARY KEY (`Id`)) ENGINE = InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;"""
    cursor.execute(sql_create)


def is_existed(table_i):
    sql_show_tables = "show tables;"
    tables_example = cursor.execute(sql_show_tables)
    for table in cursor.fetchall():
        print table
        print table_i
        if table_i in table:
            return True
    return False


if __name__ == "__main__":
    new_table = "auto_test1"
    if is_existed(new_table) is False:
        print "To create new table....."
        _build_tables(new_table)
    else:
        pass

    # add
    sql = "insert into " + new_table + "(name, class, age) values(%s, %s, %s)"
    param = ("tom", "class two grade one", "7")
    cursor.execute(sql, param)

    # 一次插入多条数据
    cursor.executemany(sql, [
        ('Tom', '1 year 1 class', '6'),
        ('Jack', '2 year 1 class', '7'),
        ('Yaheng', '2 year 2 class', '7'),
    ])
    conn.commit()

    # 更新
    sql = "update " + new_table + " set class=%s where name='tom'"
    param = ("3 year 3 class",)
    cursor.execute(sql, param)
    conn.commit()

    # 查询
    cursor.execute("select * from " + new_table)
    for row in cursor.fetchall():
        print "Each row is: " + str(row)
        for r in row:
            print "Each elements is: " + str(r)

    # 删除
    sql = "delete from " + new_table + " where name = %s"
    param = ("tom",)
    n = cursor.execute(sql, param)
    print n
    conn.commit()

    # 关闭

    cursor.close()
    conn.close()
