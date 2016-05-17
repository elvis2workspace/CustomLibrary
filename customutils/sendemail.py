#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年9月1日

@author: zhang.xiuhai
'''
import smtplib,sys,email

def sendEmail():
    result_name = []
    mail_from = ""
    mail_to = ['', '']
    msg = MIMEText(result_name[0],_subtype='plain',_charset='utf-8') 
    msg['Subject']='web自动化测试报告'
    smtp=smtplib.SMTP('smtp.office365.com',587) smtp.ehlo() smtp.starttls()     #用户名密码 
    smtp.login("发邮件用的邮箱地址","发邮件用的邮箱密码") smtp.sendmail(mail_from,mail_to,msg.as_string())
    smtp.sendmail(mail_from,mail_to,msg.as_string()) 
    smtp.quit()
if __name__ == '__main__':
    pass