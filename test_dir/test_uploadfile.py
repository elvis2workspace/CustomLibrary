# -*- coding: utf-8 -*-

import datetime
import os

import paramiko


def do_cmd(cmd, hostname, port, username, password):
    paramiko.util.log_to_file('paramiko.log')
    s = paramiko.SSHClient()
    s.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    s.connect(hostname= hostname, port=port, username=username, password=password)
    stdin,stdout,stderr = s.exec_command(cmd)
    print stdout.read()
    s.close()


def upload_files_pc(local_dir, remote_dir, hostname, username, password, port):
    try:
        t = paramiko.Transport((hostname, port))
        t.connect(username=username, password=password)
        sftp = paramiko.SFTPClient.from_transport(t)
        files = sftp.listdir(remote_dir)
        print files
        for f in files:
            print ''
            print '##################################'
            print 'Beginning to download file from %s %s ' % (hostname, datetime.datetime.now())
            print 'Downloading file:', os.path.join(remote_dir, f)
            sftp.get(os.path.join(remote_dir, f), os.path.join(local_dir, f))  # download
            # sftp.put(os.path.join(local_dir,f), os.path.join(remote_dir,f))  # upload
            print 'Download file success %s ' % datetime.datetime.now()
            print ''
            print '##################################'
    except Exception, data:
        print "connect error!"
        print data
    finally:
        t.close()


if __name__ == '__main__':
    hostname = '192.168.4.108'
    username = 'root'
    password = 'Centos@r730'
    port = 22
    cmd = 'free; df -h'

    local_dir = 'F:\\'
    remote_dir = '/tmp/test/'
    do_cmd(cmd, hostname, port, username, password)
    upload_files_pc(local_dir, remote_dir, hostname, username, password, port)

