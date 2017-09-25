# -*- coding: utf-8 -*- 
import paramiko
import time
import os


def ssh_mysql_dump(ip, port, db_name, tbl_name, dump_name, t1, t2):
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(ip, port, 'root', 'Python@1', timeout=5)

        mysqldump_cmd = 'mysqldump -uroot -proot@mysql %s %s -w "id > %s and id < %s" > %s' % (
        db_name, tbl_name, t1, t2, dump_name)
        print mysqldump_cmd
        ok = ssh_exec_cmd_helper(ssh, mysqldump_cmd, ip)
        if not ok:
            return
        ssh.close()
    except Exception, e:
        print ip, 'ssh_mysql_dump Error', e

    username = 'root'
    privatekey = os.path.expanduser('~/.SSH/id_rsa')
    mykey = paramiko.RSAKey.from_private_key_file(privatekey, password='Python@1')
    transport = paramiko.Transport((ip, port))
    transport.connect(username=username, pkey=mykey)

    sftp = paramiko.SFTPClient.from_transport(transport)
    sftp.get('/root/%s' % dump_name, '/Users/Elvis/Downloads/mysqldump-to-csv/%s' % dump_name)
    sftp.close()
    transport.close()


def mysqldump_to_csv(dump_name, csv_name):
    try:
        mysqldump_to_csv_cmd = 'cd ~/Downloads/mysqldump-to-csv/ && python mysqldump_to_csv.py %s > %s' % (
        dump_name, csv_name)
        os.system(mysqldump_to_csv_cmd)
    except Exception, e:
        print 'mysqldump_to_csv Error', e


def ssh_exec_cmd_helper(ssh, cmd, ip):
    stdin, stdout, stderr = ssh.exec_command(cmd)
    errors = stderr.readlines()
    if errors:
        print ip, cmd, errors
        return False

    return True


if __name__ == "__main__":
    ip = '10.12.32.93'
    port = 22
    db_name = 'auto'
    tbl_name = 'auto_test'
    dump_name = 'dumpname.db'
    csv_name = 'csvname.csv'
    # t2 = int(time.time())
    # t1 = t2 - 86400 * 30
    t2 = 100
    t1 = 10
    print 'SSH mysqldump...'
    ssh_mysql_dump(ip, port, db_name, tbl_name, dump_name, t1, t2)
    print 'mysqldump to csv...'
    mysqldump_to_csv(dump_name, csv_name)
    print 'done!'
