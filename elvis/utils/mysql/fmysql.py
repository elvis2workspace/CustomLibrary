#coding=utf-8
'''
Created on 2015年3月31日

@author: Administrator
'''
import MySQLdb
import time

#连接    
conn=MySQLdb.connect(host="localhost",user="root",passwd="000000",db="test_selenium2library",charset="utf8")
cursor = conn.cursor()    

#删除表
sql = "drop table if exists user"
cursor.execute(sql)

#创建
sql = "create table if not exists user(name varchar(128) primary key, created int(10))"
cursor.execute(sql)

#写入    
sql = "insert into user(name,created) values(%s,%s)"   
param = ("aaa",int(time.time()))    
n = cursor.execute(sql,param)    
print 'insert',n    
   
#写入多行    
sql = "insert into user(name,created) values(%s,%s)"   
param = (("bbb",int(time.time())), ("ccc",33), ("ddd",44) )
n = cursor.executemany(sql,param)    
print 'insertmany',n    

#更新    
sql = "update user set name=%s where name='aaa'"   
param = ("zzz")    
n = cursor.execute(sql,param)    
print 'update',n    
   
#查询    
n = cursor.execute("select * from user")    
for row in cursor.fetchall():    
    print row
    for r in row:    
        print r    
   
#删除    
sql = "delete from user where name=%s"   
param =("bbb")    
n = cursor.execute(sql,param)    
print 'delete',n    

#查询    
n = cursor.execute("select * from user")    
print cursor.fetchall()    

def dosql1(dbname):
    # Open database connection
    db = MySQLdb.connect("localhost","root","000000",dbname,charset='utf8' )
    
    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    
    # Drop table if it already exist using execute() method.
    cursor.execute("DROP TABLE IF EXISTS EMPLOYEE")
    
    # Create table as per requirement
    sql = """CREATE TABLE EMPLOYEE (
             FIRST_NAME  CHAR(20) NOT NULL,
             LAST_NAME  CHAR(20),
             AGE INT,  
             SEX CHAR(1),
             INCOME FLOAT )"""
    
    cursor.execute(sql)

    # disconnect from server
    db.close()
    
if __name__ == '__main__':
    sqlname = 'test_selenium2library'
    dosql1(sqlname)
    cursor.close()     
    #提交    
    conn.commit()
    #关闭    
    conn.close()   
