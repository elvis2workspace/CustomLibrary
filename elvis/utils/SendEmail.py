#-*-coding:utf-8 -*-
__author__ = 'DongJie'

import MySQLdb
import time
import smtplib
from email.mime.text import MIMEText

#检测用户使用情况，如果用户超过两天未登录，测发送邮件


class EmailConfig(object):
    #邮件配置
    HOST = ""
    USERNAME = ""
    DOMAIN = ""
    PASSWORD = ""


#执行数据库操作
def exec_mysql(host, user, password, database, sql, port = 3306):
    try:
        conn = MySQLdb.connect(host = host,
                               user = user,
                               passwd = password,
                               db = database,
                               port = port,
                               charset="utf8")
        cur = conn.cursor()
        if cur:
            cur.execute(sql)
            result = cur.fetchall()
            return result
    except MySQLdb.Error, e:
        print "Mysql Error %d: %s" % (e.args[0], e.args[1])


class SendEmail(EmailConfig):
    def __init__(self):
        EmailConfig.__init__(self)

    def ToMail(self, mailto, subject, body, format='plain'):
        if isinstance(body, unicode):
            body = str(body)
        me= u"系统管理员" + "<"+"xx@xx.com"+">"
        msg = MIMEText(body, format, 'utf-8')
        if not isinstance(subject, unicode):
            subject = str(subject)
        if isinstance(mailto, list):
            msg['To'] = ";".join(mailto)
        else:
            msg['To'] = mailto
        msg['Subject'] = subject
        msg['From'] = me
        msg["Accept-Language"]="zh-CN"
        msg["Accept-Charset"]="ISO-8859-1,utf-8"
        try:
            s = smtplib.SMTP()
            s.connect(self.HOST)
            s.login(self.USERNAME,self.PASSWORD)
            s.sendmail(me, mailto, msg.as_string())
            s.close()
            return True
        except Exception, e:
            print str(e)
            return False




