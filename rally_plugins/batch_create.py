#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, commands, os, MySQLdb
import Queue, threading, time
from fabric.colors import *
from optparse import OptionParser


class vmListAnaly():
    def __init__(self, vmlist_file):
        self.f_handle = open(vmlist_file)
        self.vmlist = {}
        while True:
            self.file_tmp = self.f_handle.readline()
            if self.file_tmp:
                if self.vmlist.has_key(self.file_tmp.split()[0]):
                    self.vmlist[self.file_tmp.split()[0]].append((self.file_tmp.split()[1], self.file_tmp.split()[2]))
                else:
                    self.vmlist[self.file_tmp.split()[0]] = []
                    self.vmlist[self.file_tmp.split()[0]].append((self.file_tmp.split()[1], self.file_tmp.split()[2]))
            else:
                break

    def getAllIps(self):
        ips = []
        for __host in self.vmlist:
            for i in self.vmlist[__host]:
                ips.append(i[0])
        return ips

    def getFirstIps(self):
        ips = []
        for __host in self.vmlist:
            ips.append(self.vmlist[__host][0][0])
        return ips

    def getSecondIps(self):
        ips = []
        for __host in self.vmlist:
            del self.vmlist[__host][0]
        for __host in self.vmlist:
            for i in self.vmlist[__host]:
                ips.append(i[0])
        return ips

    def getAllCreInfo(self):
        return self.vmlist

    def getFirstCreInfo(self):
        results = []
        for __host in self.vmlist:
            self.vmlist[__host] = [self.vmlist[__host][0]]
        return self.vmlist

    def getSecondCreInfo(self):
        for __host in self.vmlist:
            del self.vmlist[__host][0]
        return self.vmlist

    def __del__(self):
        self.f_handle.close()


class getCmd():
    def __init__(self, project, vminfo, flavor, mysql_host, mysql_user, mysql_passwd):
        self.mysql_user = mysql_user
        self.mysql_host = mysql_host
        self.mysql_passwd = mysql_passwd
        self.project = project
        self.vminfo = vminfo
        self.flavor = flavor
        self.create_cmd = []

    def get_cmd(self):
        quantum_obj = mysqlEng(host=self.mysql_host, user=self.mysql_user, passwd=self.mysql_passwd)
        for __host in self.vminfo:
            if self.vminfo[__host]:
                for i in self.vminfo[__host]:
                    __ip = i[0]
                    __flavor = i[1]
                    __network_id = quantum_obj.getNetworkID(__ip)
                    __tmp = "nova --os-username %s --os-password %s --os-tenant-name %s boot --security-groups %s " \
                            "--flavor %s --image %s --nic net-id=%s,v4-fixed-ip=%s --meta lan=%s --availability-zone " \
                            "nova:%s %s" % (
                        self.project['OS_USERNAME'], self.project['OS_PASSWORD'], self.project['OS_TENANT_NAME'],
                        self.project['SECURITY_GROUPS'], flavor[__flavor], self.project['IMAGE_ID'], __network_id, __ip,
                        __ip, __host, __ip)
                    self.create_cmd.append(__tmp)
        return self.create_cmd


class mysqlEng():
    def __init__(self, host='127.0.0.1', user='root', passwd='', port='3306'):
        self.conn = MySQLdb.connect(host=host, user=user, passwd=passwd, port=3306)
        self.cur = self.conn.cursor()

    def getNetworkID(self, ip):
        self.conn.select_db('quantum')
        net = "%s.%s.%s.0/24" % (ip.split('.')[0], ip.split('.')[1], ip.split('.')[2])
        __sql = "select network_id from subnets where name='%s'" % net
        self.cur.execute(__sql)
        ret = self.cur.fetchall()
        return ret[0][0]

    def getVmstate(self, ip):
        self.conn.select_db('nova')
        __sql = "select instances.uuid,instance_metadata.value,instances.host,instances.vm_state from instance_metadata,instances where instances.uuid = instance_metadata.instance_uuid and instance_metadata.value = %s and instances.vm_state not in ('error','deleted');"
        self.cur.execute(__sql, ip)
        ret = self.cur.fetchall()
        return ret[0][3]

    def getActNum(self, ips):
        self.conn.select_db('nova')
        __sql = "select count(*) from instance_metadata,instances where instances.uuid = instance_metadata.instance_uuid and instance_metadata.value in ('%s') and instances.vm_state = 'active';" % "','".join(
            ips)
        self.cur.execute(__sql)
        ret = self.cur.fetchall()
        return ret[0][0]

    def updateInstanceFlavor(self, ip, flavor):
        self.conn.select_db('nova')
        __get_flavor_info_sql = "select id,memory_mb,vcpus from instance_types where flavorid = '%s' and deleted = 0;" % flavor
        self.cur.execute(__get_flavor_info_sql)
        __flavor_info = self.cur.fetchall()
        __flavor_id = __flavor_info[0][0]
        __mem = __flavor_info[0][1]
        __vcpus = __flavor_info[0][2]
        __update_flavor_info_sql = "update instances set instance_type_id='%d',vcpus='%d',memory_mb='%d' where hostname='%s' AND vm_state != 'deleted' AND vm_state != 'error';" % (
            __flavor_id, __vcpus, __mem, ip)
        self.cur.execute(__update_flavor_info_sql)
        self.conn.commit()

    def getInstanceFlavor(self, ip):
        self.conn.select_db('nova')
        __get_instance_flavor_sql = "select vcpus,memory_mb from instances where hostname='%s' AND vm_state != 'deleted' AND vm_state != 'error';" % ip
        self.cur.execute(__get_instance_flavor_sql)
        ret = self.cur.fetchall()
        results = (ip, ret[0][0], ret[0][1])
        return results

    def getInstanceInfo(self, ips):
        self.conn.select_db('nova')
        __get_instance_info_sql = "select instances.uuid,instance_metadata.value,instances.host,instances.vm_state,memory_mb from instance_metadata,instances where instances.uuid = instance_metadata.instance_uuid and instance_metadata.value in ('%s') and instances.vm_state !='error' and instances.vm_state != 'deleted';" % "','".join(
            ips)
        self.cur.execute(__get_instance_info_sql)
        ret = self.cur.fetchall()
        return ret

    def getNodeInfo(self, nodes):
        self.conn.select_db('nova')
        __get_node_info_sql = "select instances.uuid,instance_metadata.value,instances.host,instances.vm_state,memory_mb from instance_metadata,instances where instances.uuid = instance_metadata.instance_uuid and instances.host in ('%s') and instances.vm_state !='error' and instances.vm_state != 'deleted';" % "','".join(
            nodes)
        self.cur.execute(__get_node_info_sql)
        ret = self.cur.fetchall()
        return ret

    def __del__(self):
        self.cur.close()
        self.conn.close()


class checkIps():
    def __init__(self):
        self.ping_queue = Queue.Queue()
        self.up_ip = []
        self.down_ip = []

    def __check(self, q):
        while q.qsize():
            __ip = q.get()
            __cmd = "ping -c 2 %s" % __ip
            if commands.getstatusoutput(__cmd)[0] == 0:
                self.up_ip.append(__ip)
            else:
                self.down_ip.append(__ip)
            q.task_done()

    def put2q(self, ips):
        for __ip in ips:
            self.ping_queue.put(__ip)

    def checking(self):
        print  yellow('Checking ips ...')
        for i in xrange(100):
            __pingd = threading.Thread(target=self.__check, args=(self.ping_queue,))
            __pingd.setDaemon
            __pingd.start()
        self.ping_queue.join()
        return {'up': self.up_ip, 'down': self.down_ip}


class createVm():
    def __init__(self, mysql_host, mysql_user, mysql_passwd):
        self.create_queue = Queue.Queue()
        self.mysql_user = mysql_user
        self.mysql_host = mysql_host
        self.mysql_passwd = mysql_passwd

    def __put2q(self, cmd_list, q):
        while len(cmd_list) > 0:
            while q.qsize() <= 3:
                q.put(cmd_list.pop())
                time.sleep(1)
                if not cmd_list:  break
        time.sleep(1)

    def __create(self, q):
        time.sleep(5)
        while q.qsize() > 0:
            __cmd = q.get()
            __ip = __cmd.split()[-1]
            print  "Building %s" % __ip
            ret = commands.getstatusoutput(__cmd)
            if ret[0] == 0:
                time.sleep(2)
                __vm_state = 'building'
                while __vm_state != 'active':
                    time.sleep(5)
                    __vm_state = mysqlEng(host=self.mysql_host, user=self.mysql_user,
                                          passwd=self.mysql_passwd).getVmstate(__ip)
                    if __vm_state == 'error':
                        self.error_vms += 1
                        print  red("虚拟机创建出错,IP:%s") % __ip
                        break
                self.ok_vms += 1
                print  green("%s 创建成功,此次创建虚拟机总数:%s , 成功的虚拟机:%s ,出错的虚拟机:%s") % (
                    __ip, self.all_vms, self.ok_vms, self.error_vms)
            else:
                self.error_vms += 1
                print  red("执行命令出错,IP:%s,出错信息: %s \n") % (__ip, ret[1])

    def creating(self, cmds, ths):
        self.all_vms = len(cmds)
        self.ok_vms = 0
        self.error_vms = 0
        __put_thread = threading.Thread(target=self.__put2q, args=(cmds, self.create_queue,))
        __put_thread.setDaemon
        __put_thread.start()
        for i in xrange(ths):
            __created = threading.Thread(target=self.__create, args=(self.create_queue,))
            __created.setDaemon
            __created.start()
            time.sleep(10)


class createAllvm():
    def __init__(self, vmlist, flavor, mysql_host, mysql_user, mysql_passwd):
        self.vmlist = vmlist
        self.mysql_host = mysql_host
        self.mysql_user = mysql_user
        self.mysql_passwd = mysql_passwd
        self.flavor = flavor

    def start(self):
        create_obj = createVm(mysql_host=self.mysql_host, mysql_user=self.mysql_user, mysql_passwd=self.mysql_passwd)

        first_create_cmd = getCmd(project_info, vmListAnaly(self.vmlist).getFirstCreInfo(), self.flavor,
                                  mysql_host=self.mysql_host, mysql_user=self.mysql_user,
                                  mysql_passwd=self.mysql_passwd).get_cmd()
        create_obj.creating(first_create_cmd, 2)
        first_ips = vmListAnaly('vmhost').getFirstIps()
        while True:
            time.sleep(10)
            if mysqlEng(host=self.mysql_host, user=self.mysql_user, passwd=self.mysql_passwd).getActNum(
                    first_ips) == len(first_ips):  break
        print  '第一波创建完毕'
        second_create_cmd = getCmd(project_info, vmListAnaly(self.vmlist).getSecondCreInfo(), self.flavor,
                                   mysql_host=self.mysql_host, mysql_user=self.mysql_user,
                                   mysql_passwd=self.mysql_passwd).get_cmd()
        if not second_create_cmd:
            print  green('只有一波了..创建完毕')
            sys.exit()
        create_obj.creating(second_create_cmd, 3)
        second_ips = vmListAnaly('vmhost').getSecondIps()
        while True:
            time.sleep(10)
            if mysqlEng(host=self.mysql_host, user=self.mysql_user, passwd=self.mysql_passwd).getActNum(
                    second_ips) == len(second_ips):  break
        print  '第二波创建完毕'


if __name__ == '__main__':
    # project_name = sys.argv[1]
    # vmlist = sys.argv[2]
    mysql_host = '127.0.0.1'
    mysql_user = 'root'
    mysql_passwd = '123456'
    parser = OptionParser()
    parser.add_option("-c", "--create", dest="create", action="store_true", default=False, help="create vm in filename")
    parser.add_option("-u", "--update", dest="update", help="update ip to a flavor")
    parser.add_option("-p", "--ping", dest="ping", action="store_true", default=False, help="multil threading to ping")
    parser.add_option("-i", "--instances", dest="instances", help="query a ip's info")
    parser.add_option("-n", "--nodes", dest="nodes", help="query a host's info")
    parser.add_option("-g", "--getcmd", dest="getcmd", action="store_true", default=False,
                      help="get build vm commands from a file")
    parser.add_option("-d", "--delete", dest="delete", help="delete a instance")
    parser.add_option("-f", "--filename", dest="filename", help="Specify a file")
    parser.add_option("-F", "--flavor", dest="flavor", help="Specify a flavor")
    parser.add_option("-t", "--tenant", dest="tenant", help="Specify a tenant")
    (options, args) = parser.parse_args()
    project_conf = {
        'test': {
            'OS_USERNAME': 'test',
            'OS_PASSWORD': 'test',
            'OS_TENANT_NAME': 'test',
            'SECURITY_GROUPS': 'default',
            'IMAGE_ID': '25b349f2-a105-4d2d-aabe-89253e8b4eb1',
        },
        'test2': {
            'OS_USERNAME': 'test2',
            'OS_PASSWORD': 'test2',
            'OS_TENANT_NAME': 'test2',
            'SECURITY_GROUPS': 'default',
            'IMAGE_ID': 'a6955dde-7794-45cc-a468-65ce7d2473c1',
        },
    }
    flavor = {
        '1': '1',
        '2': '2',
        '4': '4',
        '6': '6',
        '8': '8',
        '12': '12',
        '16': '16',
    }
    the_mysql_obj = mysqlEng(host=mysql_host, user=mysql_user, passwd=mysql_passwd)
    if options.create:
        project_info = project_conf[options.tenant]
        vmfile = options.filename
        the_create_obj = createAllvm(vmlist=vmfile, flavor=flavor, mysql_host=mysql_host, mysql_user=mysql_user,
                                     mysql_passwd=mysql_passwd)
        the_create_obj.start()
    if options.update:
        ip = options.update
        flavor = options.flavor
        the_mysql_obj.updateInstanceFlavor(ip, flavor)
        print  "update ok,now the vm %s is %s cups and %sMB memory" % the_mysql_obj.getInstanceFlavor(ip)
        instance_id = the_mysql_obj.getInstanceInfo([ip])[0][0]
        print  red('please command == nova reboot --hard %s == to effect' % instance_id)
    if options.ping:
        filename = options.filename
        allip = vmListAnaly(filename).getAllIps()
        check_obj = checkIps()
        check_obj.put2q(allip)
        check_ret = check_obj.checking()
        if check_ret['up']:
            for ip in check_ret['up']:
                print  red("%s is up,please check...") % ip
            sys.exit(9)
        else:
            print  green('All ip is check ok...,you can create these vms')
    if options.instances:
        args.append(options.instances)
        ips = args
        instances_info = the_mysql_obj.getInstanceInfo(ips)
        print  "id\t\t\t\t\tip\t\tnode\t\t\tstate\tmemory"
        for i in instances_info:
            print  "%s\t%s\t%s\t%s\t%s" % i
    if options.nodes:
        args.append(options.nodes)
        nodes = args
        nodes_info = the_mysql_obj.getNodeInfo(nodes)
        print  "id\t\t\t\t\tip\t\tnode\t\t\tstate\tmemory"
        for i in nodes_info:
            print  "%s\t%s\t%s\t%s\t%s" % i
    if options.getcmd:
        filename = options.filename
        tenant = options.tenant
        project_info = project_conf[tenant]
        result = getCmd(project_info, vmListAnaly(filename).getAllCreInfo(), flavor, mysql_host=mysql_host,
                        mysql_user=mysql_user, mysql_passwd=mysql_passwd).get_cmd()
        for i in result:
            print  i
    if options.delete:
        args.append(options.delete)
        ips = args
        instances_ids = []
        instances_info = the_mysql_obj.getInstanceInfo(ips)
        for i in instances_info:
            instances_ids.append(i[0])
        if not instances_ids:
            print  red("no instance found")
            sys.exit(8)
        delete_cmd = "nova delete " + ' '.join(instances_ids)
        result = commands.getstatusoutput(delete_cmd)
        if result[0] == 0:
            print  green('OK')
        else:
            print  red("error info: %s" % result[1])
