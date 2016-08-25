# -*- coding: utf-8 -*-

import paramiko
import threading
import pexpect
# 需要安装模块，pycrypto-2.6.1，需要安装vcforpython.msi


def ssh1(ip, password, cmd):
    ret = -1
    ssh = pexpect.spawn('ssh root@%s "%s"' % (ip, cmd))
    try:
        i = ssh.expect(['password:', 'continue conneting (yes/no)?'], timeout=5)
        if i == 0:
            ssh.sendline(passwd)
        elif i == 1:
            ssh.sendline('yes\n')
            ssh.expect('password:')
            ssh.sendline(passwd)
            ssh.sendline(cmd)
            r = ssh.read()
            print r
            ret = 0
    except pexpect.EOF:
        print "EOF"
        ssh.close()
        ret = -1
    except pexpect.TIMEOUT:
        print "TIMEOUT"
        ssh.close()
        ret = -2
        return ret

"""
用root用户登录，安装软件时apt-get install xxx 最好加上“-y”参数
远端执行命令时如果有交互，可以这样用 stdin.write("Y")来完成交互，“Y”就是输入“Y”
"""


def ssh2(ip, username, passwd, cmd):
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(ip, 22, username, passwd, timeout=5)
        for m in cmd:
            stdin, stdout, stderr = ssh.exec_command(m)
            # stdin.write("Y") # 简单交互， 输入 'Y'
            out = stdout.readlines()
            # 屏幕输出

            for o in out:
                print o,
                ssh.close()
            print '%s\tOK\n' % ip
    except:
        print '%s\tError\n' % ip

if __name__ == '__main__':
    cmd = ['cal', 'echo hello']  # 即将执行的命令列表 所有的命令放到一个列表里面，遍历列表可以依次执行列表里面的命令
    username = 'root'
    passwd = 'Centos@r730'
    threads = []
    print "Begin..."
    # for i in range(1, 254):
    #     ip = '192.168.1.' + test_str(i)
    #     a = threading.Thread(target=ssh2, args=(ip, username, passwd, cmd))
    #     a.start()
    ip = '192.168.4.108 '
    a = threading.Thread(target=ssh2, args=(ip, username, passwd, cmd))
    a.start()

    ssh1(ip, passwd, 'cal')
