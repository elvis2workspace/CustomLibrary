# -*- coding:utf-8 -*-
import subprocess
import telnetlib, sys
import wmi


def python_call_powershell(ip):
    try:
        args = [r"C:\WINDOWS\system32\WindowsPowerShell\v1.0\powershell.exe", "-ExecutionPolicy", "Unrestricted",
                r"test_ping.ps1", ip]  # args参数里的ip是对应调用powershell里的动态参数args[0],
        # 类似python中的sys.argv[1]
        p = subprocess.Popen(args, stdout=subprocess.PIPE)
        dt = p.stdout.read()
        return dt
    except Exception, e:
        print e
        return False


# 安装python的wmi包
def call_remote_bat(ip_address, username, password, target_bat):
    try:
        #  use wmi to connect remote server
        conn = wmi.WMI(computer=ip_address, user=username, password=password)
        filename = target_bat  # remote file
        cmd_call_bat = r"cmd /c call %s " % filename
        conn.Win32_Process.Create(CommandLine=cmd_call_bat)  # execution bat file
        print "execution success."
    except Exception, e:
        log = open('logfile', 'a')
        log.write('%s, call bat Failed!\r\n' % ip_address)
        log.close()
        return False


def telnetdo(HOST=None, USER=None, PASS=None, COMMAND=None):  # define a function
    if not HOST:
        try:
            HOST = sys.argv[1]
            USER = sys.argv[2]
            PASS = sys.argv[3]
            COMMAND = sys.argv[4]
        except:
            print "Usage: telnet_remote.py host user pass command"
            return
    tn = telnetlib.Telnet()  #
    try:
        tn.open(HOST)
    except:
        print "Cannot open host"
        return
    tn.read_until("login:")
    tn.write(USER + '\n')
    if PASS:
        tn.read_until("Password:")
        tn.write(PASS + '\n')
        tn.write(COMMAND + '\n')
        tn.write("exit\n")
        tmp = tn.read_all()
        tn.close()
        del tn
        return tmp


if __name__ == "__main__":
    ip = ['192.168.4.108', '192.160.20.247']
    # print python_call_powershell(ip)
    call_remote_bat('192.168.20.216', 'zhang.xiuhai', 'Python@8', r"D:\temp\releasePack\back_all.bat")
