#!/usr/bin/env python
# -*- coding=utf-8 -*-

import multiprocessing
import random
import time
import os


# apply_async(func[, args[, kwds[, callback]]]) 它是非阻塞，
# apply(func[, args[, kwds]])是阻塞的（理解区别，看例1例2结果区别）
# close()    关闭pool，使其不在接受新的任务。
# terminate()    结束工作进程，不在处理未完成的任务。
# join()    主进程阻塞，等待子进程的退出， join方法要在close或terminate之后使用。


def func(msg):
    print "msg: ", msg
    time.sleep(3)
    print 'end'


"""多个进程池"""


def Lee():
    print "\nRun task Lee-%s" % (os.getpid())  # os.getpid()获取当前的进程的ID
    start = time.time()
    time.sleep(random.random() * 10)  # random.random()随机生成0-1之间的小数
    end = time.time()
    print 'Task Lee, runs %0.2f seconds.' % (end - start)


def Marlon():
    print "\nRun task Marlon-%s" % (os.getpid())
    start = time.time()
    time.sleep(random.random() * 40)
    end = time.time()
    print 'Task Marlon runs %0.2f seconds.' % (end - start)


def Allen():
    print "\nRun task Allen-%s" % (os.getpid())
    start = time.time()
    time.sleep(random.random() * 30)
    end = time.time()
    print 'Task Allen runs %0.2f seconds.' % (end - start)


def Frank():
    print "\nRun task Frank-%s" % (os.getpid())
    start = time.time()
    time.sleep(random.random() * 20)
    end = time.time()
    print 'Task Frank runs %0.2f seconds.' % (end - start)


if __name__ == "__main__":
    """异步，非阻塞"""
    pool = multiprocessing.Pool(processes=3)
    for i in xrange(5):
        msg = "hello %d" % i
        pool.apply_async(func, (msg,))  # 维持执行的进程总数为processes，当一个进程执行完毕后会添加新的进程进去

    print "##########非阻塞#############"
    pool.close()
    pool.join()  # 调用join之前，先调用close函数，否则会出错。执行完close后不会有新的进程加入到pool,join函数等待所有子进程结束
    print "Sub-process(es) done."

    print "##########阻塞###############"

    pool_block = multiprocessing.Pool(processes=3)
    for i in xrange(5):
        msg = "hello %d" % i
        pool_block.apply(func, (msg,))

    print "Mark block pool##############"
    pool_block.close()
    pool_block.join()
    print "block sub-process(es) done."

    """多个线程池"""

    function_list = [Lee, Marlon, Allen, Frank]
    print "parent process %s" % os.getpgid(os.getpid())

    pool_multi_process = multiprocessing.Pool(4)
    for func in function_list:
        pool_multi_process.apply_async(func)

    print "waiting for all subprocess done..."
    pool_multi_process.close()
    pool_multi_process.join()
    print "All subprocesses done."
