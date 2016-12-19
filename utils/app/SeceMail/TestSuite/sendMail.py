#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年4月16日

@author: zhangxiuhai
'''
import smtplib
# from email.mime.text import MIMEText

def SendMail(mail_dest):
    mail_server = "smtp.qq.com"
    mail_port = "25"
    username = "2583773475@qq.com"
    password = "zhangxiuhai1988"
    
    if type(mail_dest) == str:
        addr_list = mail_dest.split(';')
    elif type(mail_dest) == list:
        addr_list = mail_dest
    else:
        print 'You input email address error!'
        
    try:
        handle = smtplib.SMTP(mail_server, mail_port)
        handle.connect(mail_server, mail_port)
        handle.login(username, password)
        #这里的msg其实就是一种固定的格式，From:To:Subject
        message = """From: From Person <2583773475@qq.com>
        To: To Person <secemaillog@163.com>
        MIME-Version: 1.0
        Content-type: text/html
        Subject: SMTP HTML e-mail test
        This is an e-mail message to be sent in HTML format
        <b>This is HTML message.</b>
        <h1>This is headline.</h1>
        """
        print message
        print addr_list
        handle.sendmail(username, addr_list, message)
        print "Send email successfully."
        handle.quit()
    except Exception,e:
        print "Send email failed because %s" % e
        
if __name__ == '__main__':
    SendMail('secemaillog@163.com')
    