#!/usr/bin/python
#_*_coding: utf-8_*_
import adodbapi
conStr="Provider=SQLOLEDB.1;Password=sa123;Persist Security Info=True;User ID=sa;Initial Catalog=run;Data Source=10.10.0.101,1633"
conn=adodbapi.connect(conStr)
cur=conn.cursor()
sqlcmd ="select * from senderinfo"
try:
    cur.execute(sqlcmd)
except:
    print "执行sql失败"
else:
    data=cur.fetchall()
    
    print data
for record in data:
    print record
print "下面是查询出来的结果"
n=len(data)
k=len(data[0])
print n
print k
for i in range(n) :
    for j in range(k):
        print data[i][j]
conn.close
