#!/usr/bin/env python

# -*- coding: utf-8 -*-

import subprocess
from Queue import Queue
from threading import Thread

num_threads = 3
ips = ['127.0.0.1', '192.168.20.247']
q = Queue()


def pingme(i, queue):
    while True:
        ip = queue.get()
        print "Thread %s is pinging %s." % (i, ip)
        ret = subprocess.Popen('ping -c 1 %s' % ip, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if ret == 0:
            print '%s is alive!' % ip
        elif ret == 1:
            print '%s is down...' % ip
        queue.task_done()

# start num_threads threads
for i in range(num_threads):
    t = Thread(target=pingme, args=(i, q))
    t.setDaemon(True)
    t.start()

for ip in ips:
    q.put(ip)
    print 'main thread waiting...'
    q.join()
    print 'Done'
