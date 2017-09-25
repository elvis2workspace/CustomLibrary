#!/usr/bin/python

import pexpect


def login(port, user, passwd, ip, command):
    child = pexpect.spawn('SSH -p%s %s@%s "%s"' % (port, user, ip, command))
    o = ''
    try:
        i = child.expect(['[Pp]assword:', 'continue connecting (yes/no)?'])
        if i == 0:
            child.sendline(passwd)
        elif i == 1:
            child.sendline('yes')
        else:
            pass
    except pexpect.EOF:
        child.close()
    else:
        o = child.read()
        child.expect(pexpect.EOF)
        child.close()
    return o


if __name__ == '__main__':
    hosts = file('hosts.list', 'r')
    for line in hosts.readlines():
        host = line.strip("\n")
        if host:
            ip, port, user, passwd, commands = host.split(":")
            for command in commands.split(","):
                print "+++++++++++++++ %s run:%s ++++++++++++" % (ip, command),
                print login(port, user, passwd, ip, command)
    hosts.close()
