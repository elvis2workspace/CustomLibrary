#!/usr/bin/env python
# coding=utf-8

# -*- coding: UTF-8 -*-
"""
Python内置对SMTP的支持，可以发送纯文本邮件、HTML邮件以及带附件的邮件。
Python对SMTP支持有smtplib和email两个模块，email负责构造邮件，smtplib负责发送邮件。
只要掌握了各种邮件类型的构造方法，正确设置好邮件头，就可以顺利发出。
构造一个邮件对象就是一个Messag对象，如果构造一个MIMEText对象，就表示一个文本邮件对象，如果构造一个MIMEImage对象，
就表示一个作为附件的图片，要把多个对象组合起来，就用MIMEMultipart对象，而MIMEBase可以表示任何对象。
"""
import os
import smtplib
from email.mime.base import MIMEBase
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.utils import parseaddr, formataddr


def _format_addr(s):
    name, addr = parseaddr(s)
    return formataddr((Header(name, 'utf-8').encode(), addr))

def send_email(receivers, *args, **kwargs):

    # 第三方 SMTP 服务
    mail_host = "smtp.exmail.qq.com"  # 设置服务器
    mail_user = "elvis.zhang@cloudminds.com"  # 用户名
    mail_pass = "Python@1"  # 密码
    sender = mail_user
    # receivers = ['xiuhai5052@gmail.com']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱
    attached = args[0]
    print attached

    # 邮件对象:
    message = MIMEMultipart()
    message['From'] = _format_addr('达闼测试 <%s>' % sender)
    message['To'] = _format_addr('收件人 <%s>' % receivers[0])
    message['Subject'] = Header('来自达闼测试人员', 'utf-8').encode()

    # 邮件正文是MIMEText:
    message.attach(MIMEText('' + '' + '', 'html', 'utf-8'))

    if os.path.exists(attached):
        # 添加附件就是加上一个MIMEBase，从本地读取一个图片:
        with open(attached, 'rb') as f:
            # 设置附件的MIME和文件名，这里是png类型:
            mime = MIMEBase('image', 'gif', filename='七层网络.gif')
            # 加上必要的头信息:
            mime.add_header('Content-Disposition', 'attachment', filename='七层网络.gif')
            mime.add_header('Content-ID', '<0>')
            mime.add_header('X-Attachment-Id', '0')
            # 把附件的内容读进来:
            mime.set_payload(f.read())
            # 用Base64编码:
            encoders.encode_base64(mime)
            # 添加到MIMEMultipart:
            message.attach(mime)
    try:
        smtpObj = smtplib.SMTP()
        smtpObj.connect(mail_host, 25)  # 25 为 SMTP 端口号
        smtpObj.set_debuglevel(1)  # 设置为1，可以查看运行日志
        smtpObj.login(mail_user, mail_pass)
        smtpObj.sendmail(sender, receivers, message.as_string())
        print "邮件发送成功"
    except smtplib.SMTPException:
        print "Error: 无法发送邮件"


if __name__ == "__main__":
    # 接收邮件，可设置为你的QQ邮箱或者其他邮箱
    receivers = ['itopzabbixreminder@gemdale.com']
    attached_path = '/Users/Elvis/Downloads/七层网络.gif'
    i = 0
    while i <= 100:
        send_email(receivers, attached_path)
        i = i+1


