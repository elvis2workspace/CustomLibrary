# -*- coding: utf-8 -*-

# from multiprocessing import Process
# 可以通过Process来构造一个子进程
# p = Process(target=fun,args=(args))
# 再通过p.start()来启动子进程
# 再通过p.join()方法来使得子进程运行结束后再执行父进程

from multiprocessing import Process
from multiprocessing import Pool
from multiprocessing import Queue
import multiprocessing
import os
import time
import random


# 子进程要执行的代码
def run_process(name):
    print'Run child process %s (%s)...' % (name, os.getpid())


def long_time_task(name):
    print "Run task %s (%s)..." % (name, os.getpid())
    start = time.time()
    time.sleep(3)
    end = time.time()
    print "Task %s runs %0.2f seconds." % (name, (end - start))


# write data function.
def write(q):
    for value in ['A', 'B', 'C']:
        print 'Put %s to queue...' % value
        q.put(value)
        time.sleep(random.random())


# 读数据进程执行的代码
def read(q):
    while True:
        if not q.empty():
            value = q.get(True)
            print 'Get %s from queue.' % value
            time.sleep(random.random())
        else:
            break


# 锁
# 写数据进程执行的代码:
def write_lock(q, lock_c):
    lock_c.acquire()  # 加上锁
    for value in ['A', 'B', 'C']:
        print'Put %s to queue...' % value
        q.put(value)
    lock_c.release()  # 释放锁


# 读数据进程执行的代码:
def read_lock(q):
    while True:
        if not q.empty():
            value = q.get(False)
            print'Get %s from queue.' % value
            time.sleep(random.random())
        else:
            break


if __name__ == '__main__':
    print'Parent process %s.' % os.getpid()
    p = Process(target=run_process, args=('test',))
    print'Process will start.'
    p.start()
    p.join()
    print 'Process end.'

    # pool创建子进程的方法与Process不同，是通过p1.apply_async(func,args=(args))实现，
    # 一个池子里能同时运行的任务是取决你电脑的cpu数量，
    # 如我的电脑现在是有4个cpu，那会子进程task0,task1,task2,task3可以同时启动，
    # task4则在之前的一个某个进程结束后才开始
    # 代码中的p.close()是关掉进程池子，是不再向里面添加进程了，对Pool对象调用join()方法会等待所有子进程执行完毕，
    # 调用join()之前必须先调用close()，调用close()之后就不能继续添加新的Process了。
    # 当时也可以是实例pool的时候给它定义一个进程的多少
    # 如果上面的代码中p=Pool(5)那么所有的子进程就可以同时进行

    p1 = Pool()
    for i in range(5):
        p1.apply_async(long_time_task, args=(i,))
    print "waiting for all subprocess done..."
    p1.close()
    p1.join()
    print "All subprocess done."

    # Communication between multiple sub processes
    print "Communication between multiple processes."

    # 父进程创建Queue，并传给各个子进程：
    q = Queue()
    pw = Process(target=write, args=(q,))
    pr = Process(target=read, args=(q,))
    # 启动子进程pw，写入:
    pw.start()
    # 等待pw结束:
    pw.join()
    # 启动子进程pr，读取:
    pr.start()
    pr.join()
    # pr进程里是死循环，无法等待其结束，只能强行终止:
    print '子进程间通信：所有数据都写入并且读完'

    # with pool to make communication between father and sub.

    manager = multiprocessing.Manager()
    # 父进程创建Queue，并传给各个子进程：
    qr = manager.Queue()
    p2 = Pool()
    pw = p2.apply_async(write, args=(qr,))
    time.sleep(0.5)
    pr = p2.apply_async(read, args=(qr,))
    p2.close()
    p2.join()

    print '父子进程间通信：所有数据都写入并且读完'

    manager_l = multiprocessing.Manager()
    # 父进程创建Queue，并传给各个子进程：
    ql = manager_l.Queue()
    lock = manager_l.Lock()  # 初始化一把锁
    pl = Pool()
    pwl = pl.apply_async(write, args=(ql, lock))
    prl = pl.apply_async(read, args=(ql,))
    pl.close()
    pl.join()

    print '使用锁控制对同一队列资源的访问，所有数据都写入并且读完'
