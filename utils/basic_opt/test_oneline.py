# -*- coding: utf-8 -*-

# 震惊小伙伴的单行代码（Python篇）

# 1、让列表中的每个元素都乘以2

print map(lambda x: x * 2, range(1, 11))

# 2、求列表中的所有元素之和

print sum(range(1, 1001))

# 3、判断一个字符串中是否存在某些词

wordlist = ["scala", "akka", "play framework", "sbt", "typesafe"]
tweet = "This is an example tweet talking about scala and sbt."

print map(lambda x: x in tweet.split(), wordlist)

# 4、读取文件
print open("test_oneline.py").readlines()

# 5、祝你生日快乐！
print map(lambda x: "Happy Birthday to " + ("you" if x != 2 else "dear Name"), range(4))

# 6. 过滤列表中的数值
print reduce(lambda(a, b), c: (a+[c], b) if c > 60 else (a, b + [c]), [49, 58, 76, 82, 88, 90], ([], []))

# 7. 获取XML web service数据并分析

from xml.dom.minidom import parse, parseString
import urllib2

# 注意，我将它转换成XML格式化并打印出来
print parse(urllib2.urlopen("http://search.twitter.com/search.atom?&q=python")).toprettyxml(encoding="utf-8")

# 8. 找到列表中最小或最大的一个数字

print min([14, 35, -7, 46, 98])
print max([14, 35, -7, 46, 98])

# 9. 并行处理

import multiprocessing
import math

print list(multiprocessing.Pool(processes=4).map(math.exp, range(1, 11)))

# 10. “Sieve of Eratosthenes”算法

# Python里没有Sieve of Eratosthenes操作符，但这对于Python来说并不是难事。

n = 50 # We want to find prime numbers between 2 and 50

print sorted(set(range(2, n+1)).difference(set((p * f) for p in range(2, int(n**0.5) + 2) for f in range(2, (n/p)+1))))

