# -*- coding: utf-8 -*-

# 安装与使用
# 1.导入Python SQLITE数据库模块
# Python2.5之后，内置了SQLite3，成为了内置模块，这给我们省了安装的功夫，只需导入即可
import sqlite3

# 2. 创建/打开数据库
#      在调用connect函数的时候，指定库名称，如果指定的数据库存在就直接打开这个数据库，如果不存在就新创建一个再打开
cx = sqlite3.connect("E:/test.db")
# 也可以创建数据库在内存中
# con = sqlite3.connect(":memory:")

# 3.数据库连接对象
#     打开数据库时返回的对象cx就是一个数据库连接对象，它可以有以下操作：
# commit()--事务提交
# rollback()--事务回滚
# close()--关闭一个数据库连接
# cursor()--创建一个游标
# 关于commit()，如果isolation_level隔离级别默认，那么每次对数据库的操作，都需要使用该命令，你也可以设置isolation_level=None，
# 这样就变为自动提交模式。

# 4.使用游标查询数据库
#     我们需要使用游标对象SQL语句查询数据库，获得查询对象。 通过以下方法来定义一个游标。
cu=cx.cursor()
#      游标对象有以下的操作：
# execute()--执行sql语句
# executemany--执行多条sql语句
# close()--关闭游标
# fetchone()--从结果中取一条记录，并将游标指向下一条记录
# fetchmany()--从结果中取多条记录
# fetchall()--从结果中取出所有记录
# scroll()--游标滚动
# 1. 建表
cu.execute("create table catalog (id integer primary key,pid integer,name varchar(10) UNIQUE,nickname text NULL)")
# 上面语句创建了一个叫catalog的表，它有一个主键id，一个pid，和一个name，name是不可以重复的，以及一个nickname默认为NULL

# 2. 插入数据
# 请注意避免以下写法：
# # Never do this -- insecure 会导致注入攻击
# pid=200
# c.execute("... where pid = '%s'"% pid)
# 正确的做法如下，如果t只是单个数值，也要采用t=(n,)的形式，因为元组是不可变的。
for t in[(0,10,'abc','Yu'),(1,20,'cba','Xu')]:
    cx.execute("insert into catalog values (?,?,?,?)", t)
# 简单的插入两行数据,不过需要提醒的是,只有提交了之后,才能生效.我们使用数据库连接对象cx来进行提交commit和回滚rollback操作.
cx.commit()

# 3.查询
cu.execute("select * from catalog")
# 要提取查询到的数据,使用游标的fetch函数,如:
cu.fetchall()

# 如果我们使用cu.fetchone(),则首先返回列表中的第一项,再次使用,则返回第二项,依次下去.

cu.execute("update catalog set name='Boy' where id = 0")
cx.commit()


# 5.删除
cu.execute("delete from catalog where id = 1")
cx.commit()

# 6.使用中文
# 请先确定你的IDE或者系统默认编码是utf-8,并且在中文前加上u
x=u'鱼'
cu.execute("update catalog set name=? where id = 0",x)
cu.execute("select * from catalog")
cu.fetchall()
# [(0, 10, u'\u9c7c', u'Yu'), (1, 20, u'cba', u'Xu')]
# 如果要显示出中文字体，那需要依次打印出每个字符串

for item in cu.fetchall():
    for element in item:
        print element,
        print
# 0 10 鱼 Yu
# 120 cba Xu

# 7.Row类型
# Row提供了基于索引和基于名字大小写敏感的方式来访问列而几乎没有内存开销。 原文如下：
# sqlite3.Row
# provides both index-based and case-insensitive name-based access to columns with almost no memory overhead. It will probably be better than your own custom dictionary-based approach or even a db_row based solution.
# Row对象的详细介绍
# class  sqlite3.Row
# A Row instance serves as a highly optimized row_factory for Connection objects. It tries to mimic a tuple in most of its features.
# It supports mapping access by column name and index, iteration, representation, equality testing and len().
# If two Row objects have exactly the same columns and their members are equal, they compare equal.
# Changed in version 2.6: Added iteration and equality (hashability).
# keys()
# This method returns a tuple of column names. Immediately after a query, it is the first member of each tuple in Cursor.description.
# New in version 2.6
cx.row_factory = sqlite3.Row

c = cx.cursor()

c.execute('select * from catalog')


r = c.fetchone()

type(r)

print r
# (0, 10, u'\u9c7c', u'Yu')

len(r)
r[2]            #使用索引查询


r.keys()
# ['id', 'pid', 'name', 'nickname']

for e in r:
   print e,


 # 使用列的关键词查询
r['id']
r['name']
