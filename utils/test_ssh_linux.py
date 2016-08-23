# -*- coding: utf-8 -*-

import paramiko
import threading


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
                print '%s\tOK\n' % ip
                ssh.close()
    except:
        print '%s\tError\n' % ip

if __name__ == '__main__':
    cmd = ['cal', 'echo hello']  # 即将执行的命令列表
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
