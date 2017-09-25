# -*- coding: utf-8 -*-

import paramiko
import time
import re
import smtplib


def ssh_ljds(Host, UserName, PassWord, cmd):
    ssh = paramiko.SSHClient()
    ssh.load_system_host_keys()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    ssh.connect(Host, username=UserName, password=PassWord, timeout=5, compress=True)

    channel = ssh.invoke_shell()
    channel.send('enable'.encode() + b'\n')
    channel.send('ccie'.encode() + b'\n')
    channel.send('terminal length 0'.encode() + b'\n')
    channel.send(cmd.encode() + b'\n')
    time.sleep(1)

    SSHreply = channel.recv(32768).decode('utf-8')

    Check = re.findall('up', SSHreply)
    if Check != ['up', 'up']:
        ljds_email('elvis.zhang@cloudminds.com', 'Python@1', 'elvis.zhang@cloudminds.com', 'elvis.zhang@cloudminds.com')

    channel.close()
    ssh.close()


def ljds_email(UserName, Password, From, To):
    # 编写邮件主题、内容
    Subject = '接口状态异常通知邮件'
    Text = 'loopback 0 接口状态异常'
    Message = ('From: {}\nTo: {}\nSubject: {}\n\n{}'.format(From, To, Subject, Text))

    # 发送邮件
    email = smtplib.SMTP('smtp.cloudminds.com')
    email.login(UserName, Password)
    email.sendmail(From, To, Message)
    email.quit()


if __name__ == '__main__':
    ssh_ljds('10.12.32.93', 'root', 'Python@1', 'show ip interface brief loopback 0')
