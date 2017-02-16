#!/usr/bin/env python
# -*- encoding: utf-8 -*-
"""
Created on 2015年3月14日

@author: Elvis
"""

import os
import re
import sys
import datetime
import paramiko
from os import walk
from os.path import join


class read_json():
    def __init__(self):
        pass

    def read(self, obj, key):
        collect = list()
        for k in obj:
            v = obj[k]

            if isinstance(v, str) or isinstance(v, unicode):
                if key == ' ':
                    collect.append({k: v})
                else:
                    collect.append({str(key) + "." + k: v})
            elif isinstance(v, int):
                if key == ' ':
                    collect.append({k: v})
                else:
                    collect.append({str(key) + "." + k: v})
            elif isinstance(v, bool):
                if key == ' ':
                    collect.append({k: v})
                else:
                    collect.append({str(key) + "." + k: v})
            elif isinstance(v, dict):
                collect.extend(self.read(v, k))
            elif isinstance(v, list):
                collect.extend(self.read_list(v, key))
        return collect

    def read_list(self, obj, key):
        collect = list()
        for index, item in enumerate(obj):
            for k in item:
                v = item[k]
                if isinstance(v, str) or isinstance(v, unicode):
                    collect.append({key + "[" + str(index) + "]" + "." + k: v})
                elif isinstance(v, int):
                    collect.append({key + "[" + str(index) + "]" + "." + k: v})
                elif isinstance(v, bool):
                    collect.append({key + "[" + str(index) + "]" + "." + k: v})
                elif isinstance(v, dict):
                    collect.extend(self.read(v, key + "[" + str(index) + "]"))
                elif isinstance(v, list):
                    collect.extend(self.read_list(v, key + "[" + str(index) + "]"))
        return collect

        # ojt=test_data1
        # print read(ojt,' ')


class FileUtils():
    # 打印当前根目录下所查找文件的全路径
    def __init__(self):
        pass

    def search_file_under_dir(self):
        name = sys.stdin.readline().rstrip()
        for root, dirs, files in walk('/'):
            if name in dirs or name in files:
                print join(root, name)

    """
    复制文件到另一个文件
    """

    def copy_from_file2file(self):
        fin = file('test.txt', 'r')
        fout = file('test.txt.bak', 'w')
        while True:
            line = fin.readline()
            if len(line) == 0:
                break

            fout.write(line)

        fin.close()
        fout.close()

    def search_file_likes(self, file_path, name):
        rtn_path = os.path.normpath(os.path.expanduser(file_path.replace('/', os.sep)))
        path_n, file_n = os.path.split(rtn_path)
        # print path_n, "\n" + file_n

        for root, dirs, files in walk(path_n):
            # print root, "\n"
            # print dirs
            # print files  # print all of files under the dirs
            if name in files:
                print name
            else:
                return -1

    def fuzzy_finder(self, user_input, collection):
        suggestions = []
        pattern = '.*?'.join(user_input)  # Converts 'djm' to 'd.*?j.*?m'
        regex = re.compile(pattern)  # Compiles a regex.
        for item in collection:
            match = regex.search(item)  # Checks if the current item matches the regex.
            if match:
                suggestions.append((len(match.group()), match.start(), item))
        return [x for _, _, x in sorted(suggestions)]

    def remove_space(self):
        for root, dirs, files in os.walk(".", topdown=False):
            for fn in files:
                if fn.find(" ") > 0:
                    n = fn.replace(" ", "_")
                    fn = os.path.join(root, fn)
                    n = os.path.join(root, n)
                    os.rename(fn, n)
            for d in dirs:  # remove space in directory names
                if " " in d:
                    nd = d.replace(" ", "_")
                    d = os.path.join(root, d)
                    nd = os.path.join(root, nd)
                    os.rename(d, nd)

    def add_name(self):
        try:
            fileNames = os.listdir(dir)
        except BaseException, e:
            print '#### ERROR: Your input is not a file path! ####'
            sys.exit()
            ex = raw_input('input the ex:\t')
        for fileName in fileNames:
            newName = dir + '\\' + ex + fileName
            print newName
            os.rename(dir + '\\' + fileName, newName)

    def re_name(self):
        try:
            fileNames = os.listdir(dir)
        except BaseException, e:
            print '#### ERROR: Your input is not a file path! ####'
            sys.exit()
        while True:
            ex = raw_input('input the ex:\t')
            x = len(ex)
            if x <= 3:
                print '\"ex is too short\"'
                continue

            for fileName in fileNames:

                if ex in fileName:
                    newName = dir + '\\' + fileName[x:]
                    print newName
                    os.rename(dir + '\\' + fileName, newName)
                else:
                    print 'This \"%s\" is not exist!' % ex

            choice = raw_input('type \"y\" or \"Y\" for retry or more files rename ?\t')
            if choice in ('y', 'Y'):
                pass
            else:
                print '*' * 30 + '\nBye!'
                break

    def do_cmd(cmd, hostname, port, username, password):
        paramiko.util.log_to_file('paramiko.log')
        s = paramiko.SSHClient()
        s.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        s.connect(hostname=hostname, port=port, username=username, password=password)
        stdin, stdout, stderr = s.exec_command(cmd)
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
    # search_file_under_dir()
    tmp_path = __file__
    fu = FileUtils()
    fu.search_file_likes(tmp_path, 'fileLib.py')
    # print rtn_file, "\n" + rtn_path
    fu.copy_from_file2file()

    """模糊查询"""
    collections = ['django_migrations.py', 'django_admin_log.py', 'main_generator.py', 'migrations.py', 'api_user.doc',
                   'user_group.doc', 'accounts.txt',
                   ]
    print "fuzzy_finder results: ", fu.fuzzy_finder('mig', collections)

    """verify the rename and add name function."""
    choice = int(raw_input('what do you want? addName press \'1\',rename press \'2\': \t'))
    dir_files = raw_input('The path of your files: \t')
    if choice == 1:
        fu.add_name()
    elif choice == 2:
        fu.re_name()
    else:
        print 'choose a right choice!!!'

    """verify upload_file function."""
    hostname = '192.168.4.108'
    username = 'root'
    password = 'Python@1'
    port = 22
    cmd = 'free; df -h'

    local_dir = 'F:\\'
    remote_dir = '/tmp/test/'
    fu.do_cmd(cmd, hostname, port, username, password)
    fu.upload_files_pc(local_dir, remote_dir, hostname, username, password, port)
