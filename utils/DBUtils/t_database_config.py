#!/usr/bin/python
# -*- coding: UTF-8 -*-
"""
Python 标准数据库接口为 Python DB-API，Python DB-API为开发人员提供了数据库应用编程接口。
Python 数据库接口支持非常多的数据库，你可以选择适合你项目的数据库：
GadFly
mSQL
MySQL
PostgreSQL
Microsoft SQL Server 2000
Informix
Interbase
Oracle
Sybase
你可以访问Python数据库接口及API查看详细的支持数据库列表。
不同的数据库你需要下载不同的DB API模块，例如你需要访问Oracle数据库和Mysql数据，你需要下载Oracle和MySQL数据库模块。
DB-API 是一个规范. 它定义了一系列必须的对象和数据库存取方式, 以便为各种各样的底层数据库系统和多种多样的数据库接口程序提供一致的访问接口 。
Python的DB-API，为大多数的数据库实现了接口，使用它连接各数据库后，就可以用相同的方式操作各数据库。
Python DB-API使用流程：
引入 API 模块。
获取与数据库的连接。
执行SQL语句和存储过程。
关闭数据库连接。

########################################

事务机制可以确保数据一致性。
事务应该具有4个属性：原子性、一致性、隔离性、持久性。这四个属性通常称为ACID特性。
原子性（atomicity）。一个事务是一个不可分割的工作单位，事务中包括的诸操作要么都做，要么都不做。
一致性（consistency）。事务必须是使数据库从一个一致性状态变到另一个一致性状态。一致性与原子性是密切相关的。
隔离性（isolation）。一个事务的执行不能被其他事务干扰。即一个事务内部的操作及使用的数据对并发的其他事务是隔离的，并发执行的各个事务之间不能互相干扰。
持久性（durability）。持续性也称永久性（permanence），指一个事务一旦提交，它对数据库中数据的改变就应该是永久性的。接下来的其他操作或故障不应该对其有任何影响。
Python DB API 2.0 的事务提供了两个方法 commit 或 rollback。
"""
import MySQLdb
remote_databse = "172.16.12.79"
user_id = "root"
user_password = "123456"

database = "autotest"

# 打开数据库连接
db = MySQLdb.connect(remote_databse, user_id, user_password, database)

# 使用cursor()方法获取操作游标
cursor = db.cursor()

# 使用execute方法执行SQL语句
cursor.execute("SELECT VERSION()")

# 使用 fetchone() 方法获取一条数据库。
data = cursor.fetchone()

print "Database version : %s " % data

# 删除表
cursor.execute("DROP TABLE IF EXISTS EMPLOYEE")
cursor.execute("DROP TABLE IF EXISTS user")

# 创建数据表SQL语句
sql = """CREATE TABLE EMPLOYEE (
         FIRST_NAME CHAR(20) NOT NULL,
         LAST_NAME CHAR(20),
         AGE INT,
         SEX CHAR(1),
         INCOME FLOAT )"""

sql_create = """CREATE TABLE `user` (
                `Id` int(11) NOT NULL AUTO_INCREMENT,
                `name` varchar(255) DEFAULT NULL,
                `age` varchar(255) DEFAULT NULL,
                PRIMARY KEY (`Id`)) ENGINE = InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;"""
cursor.execute(sql)
cursor.execute(sql_create)

# cursor.execute("DROP TABLE IF EXISTS EMPLOYEE")

# SQL 插入语句
sql_insert = """INSERT INTO EMPLOYEE(FIRST_NAME,
                LAST_NAME, AGE, SEX, INCOME)
                VALUES ('Mac', 'Mohan', 20, 'M', 2000)"""

sql_insert1 = """INSERT INTO EMPLOYEE(FIRST_NAME,
                LAST_NAME, AGE, SEX, INCOME)
                VALUES ('Mac1', 'Mohan1', 18, 'M', 20003)"""

sql_insert_pra = "INSERT INTO EMPLOYEE(FIRST_NAME, \
                  LAST_NAME, AGE, SEX, INCOME) \
                  VALUE ('%s', '%s', '%d', '%c', '%d') " % \
                  ('Elvis', 'Zhang', 29, 'M', 2002)
# SQL 查询语句
sql_query = "SELECT * FROM EMPLOYEE  WHERE INCOME > '%d'" % 1000

# SQL 更新语句
sql_update = "UPDATE EMPLOYEE SET AGE = AGE + 1 WHERE SEX = '%c'" % 'M'

# SQL 删除语句
sql_delete = "DELETE FROM EMPLOYEE WHERE AGE > '%d' " % 20

try:
    # 执行sql语句
    cursor.execute(sql_insert)
    cursor.execute(sql_insert1)
    cursor.execute(sql_insert_pra)
    cursor.execute(sql_update)
    cursor.execute(sql_delete)
    cursor.execute(sql_query)

    # 提交到数据库执行
    db.commit()

    # 获取所有记录列表
    results = cursor.fetchall()
    for row in results:
        fname = row[0]
        lname = row[1]
        age = row[2]
        sex = row[3]
        income = row[4]
        # 打印结果
        print "fname = %s, lname = %s, age = %d, sex = %s, income = %d" % (fname, lname, age, sex, income)

except:
    # rollback in case there is any error
    db.rollback()

# 关闭数据库连接
db.close()


