# -*- coding: utf-8 -*-


from __future__ import division

import itertools
import os
import sets
import string
import struct

# 字符和字符值之间的转换
# 关键函数：ord,chr,unichr,利用好map()。
# ascii
print ord('a')
print chr(97)
# unicode
print ord(u'\u2020')
print repr(unichr(8224))
# 把字符串变成各个字符的值的列表
print map(ord,'Hello Wolrd')
# 反过来
print''.join(map(chr,range(97,100)))


# 字符串对齐
# python处理文本对齐：ljust，rjust，center
# 共生成20个字符
print '|', 'hej'.ljust(20), '|', 'hej'.rjust(20), '|', 'hej'.center(20), '|'
# 还可以指定分隔字符
print '|', 'hej'.ljust(20, '#'), '|', 'hej'.rjust(20, '#'), '|', 'hej'.center(20, '#'), '|'

# 去除字符串两端的空格
# 关键函数：lstrip(),rstrip(),strip() ，他们还能指定要去掉的字符（字符串）
x = '                hej                 '
print '|', x.lstrip(), '|', x.rstrip(), '|', x.strip(),'|'
y = 'ababab xx bababa'
print '|', y.lstrip('ab'), '|', y.rstrip('ab'), '|', y.strip('ab'), '|'


# 合并字符串
# 以后再用+或者+=就剁手吧！因为字符串是不可改变对象，所以在用+或+=的时候，会产生大量的中间对象，它们不断被创建和丢弃，效率不高。
# reduce其实和+或+=一样。
# 原则是：避免中间对象!用join！！
l = ['a','b','c','d','e','f','g']
largeString = ''.join(l)
print largeString

# 将字符串逐字符或逐词反转
# 对于逐字符：很喜欢[::-1]这种靠步长来整的。
# 对于逐词汇：reverse 和 [::-1] 也有用！
# 最后的正则：(\s+) 匹配不是空白字符的字符串。
# re.split(r('\s+'),s) 能够保留原来的空格。
import re
s = 'Hello  World'
# 步长为-1，表示从尾开始复制
print s[::-1]
# 逐词
revwords = s.split()
revwords.reverse()
print' '.join(revwords)

# 逐词简约版
revwords = ' '.join(s.split()[::-1])
print  revwords

# 正则版本，保留空格
revwords = re.split(r'(\s+)',s)
revwords.reverse()
revwords = ''.join(revwords)
print revwords

# 正则简约版
revwords = ''.join(re.split(r'(\s+)',s)[::-1])
print revwords

# reversed 代替 [::-1]
revwords = ' '.join(reversed(s.split( )))
print revwords
revwords = ''.join(reversed(re.split(r'(\s+)',s)))
print revwords


# 检查字符串中是否包含某字符集合中的字符 maketrans 和 translate。
# maketrans 和 translate的用法，简单说就是：
s = 'abcdefg-1234567' # 原始字符
table = string.maketrans('abc', 'ABC') # 把字符串中的abc字符集合映射成ABC字符集合
deltable = 'bc123' # 待删除字符集合
s.translate(table,deltable) # s删除掉deltable的字符后，对着table映射


# 在判断str1是否包含str2某些元素的时候：
notrans = string.maketrans('','') # 意思是不做映射


def containsAny(str1,str2):
    # str1删除str2中元素后，若len值发生改变，返回True，
    # 被删除的是共同元素
    return len(str1) != len(str1.translate(notrans,str2))

# 在判断str1是否完全包含str2的时候：
notrans = string.maketrans('', '')


def containsAll(str1,str2):
    return not str2.translate(notrans,str1)
# 如果str1完全包含str2，str2.translate(notrans,str1)会是一个空集合，not {} 就是True
# 如果str1不能全含str2，str2.translate(notrans,str1)会是一个非空集合{},，非空为True，not 非空就是False


'''aset 包含seq任意字符即可'''


# method 1 这是最朴素简单实用的短路法。
def containsAny(seq, aset):
    for c in seq:
        if c in aset:
            return True
    return False


# method 2 装逼写法，依旧短路法
def containsAny2(seq, aset):
    for item in itertools.ifilter(aset.__contains__,seq):
        return True
    return False

# method 3 装的艺术 - maketrans和translate
notrans = string.maketrans('', '')


def containsAny3(astr,strset):
    return len(astr) != len(astr.translate(notrans,strset))

'''seq包含aset全部字符'''


# method 4 这也是最朴素简单实用的短路法
def containsAll(seq,aset):
    for c in aset:
        if c not in seq:
            return False
    return True


# method 5 set集合 difference 不是短路法，效率没3高
def containsAll2(seq, aset):
    return not set(aset).difference(seq)

# method 6 装的艺术 - maketrans和translate
notrans = string.maketrans('', '')


def containsAll3(astr,strset):
    return not strset.translate(notrans,astr)


str1 = 'abcdefg'
str2 = 'ghijklm'

str3 = 'abcdef'
str4 = 'abcd'
L1 = [1, 2, 3, 3]
L2 = [1, 2, 3, 4]

print containsAny(str1,str2)       # str1 contains any of str2 True
print containsAny2(str1,str2)      # str1 contains any of str2 True
print containsAll(str3,str4)       # str3 contains all of str4 True
print containsAll(str4,str3)       # str4 contains all of str3 False
print containsAll2(L1,L2)           # L1 contains aLL of L2 False
print containsAll2(L2,L1)           # L2 contains all of L1 True
print containsAny3(str1,str2)      # str1 contains any of str2 True
print containsAll3(str3,str4)      # str3 contains all of str4 True
print containsAll3(str4,str3)      # str4 contains all of str3 False


# 过滤字符串中不属于指定集合的字符
# 关键：我又找到一个给字符去重和排序的方法了。
# 这个只要translate取反keep即可
allchars = string.maketrans('', '')


def makefilter(keep):
    delchars = allchars.translate(allchars,keep)

    def thefilter(s):
        return s.translate(allchars, delchars)
    return thefilter

# 准备阶段
keep = makefilter('abc')
# 执行阶段
print keep('abcdefgaaa') # abcaaa

# 规整字符串，无重，排好序
def canonicform(s):
    return makefilter(s)(allchars)

s = 'aaaaaaaaaasldkfjgh'
print canonicform(s) # adfghjkls

# 等同于
aa = makefilter(s)
print aa(allchars) # adfghjkls

# Unicode的translate
# unicode translate 反而更加简单，就一个table，映射即可
tb = dict([ (ord(ch), None) for ch in u" aeiou"])
s = u'four score and seven years ago'
print s.translate(tb)  # frscrndsvnyrsg

# 本任务中考虑到str要被keep，而不是被del
# 要有一个巨大的dict来标记想要的和不想要的！
# Fred L.Drake,Jr提供了一个特不错的方法(大神就是大神)：


class Keeper(object):
    def __init__(self,keep): # 构造函数
        self.keep = sets.Set(map(ord,keep))

    def __getitem__(self,n):  # 重载[]
        if n not in self.keep:
            return None
        return unichr(n)

    def __call__(self,s): # 被调用触发
        return unicode(s).translate(self)

makefilter = Keeper # 重命名下

if __name__ == '__main__':
    just_vowels = makefilter('aeiouy')
    print just_vowels(u'four score and seven years ago')


# 检查一个字符串是文本还是二进制
# 我这个直接拓展成，检查一个文件是文本文件还是二进制文件！
# 在istextfile中，切记，用"rb"打开。
# Fred的想法好奇特：用启发式探索法来判断。最高位为1的概率，超过阈值%30就判断为二进制。
text_characters = "".join(map(chr,range(32,127))) + "\n\r\t\b"
_null_trans = string.maketrans("","")


def istext(s, text_characters=text_characters, threshold = 0.30):
    # threshold 阈值
    # s包含了空值，不是文本，空值是文本的结束标志
    if"\0"in s:
        return False
    # 逻辑判定：空字符串是文本
    if not s:
        return True
    print 'aa'
    t = s.translate(_null_trans,text_characters)
    print len(t), len(s), len(t) / len(s)
    return  len(t) / len(s) <= threshold


def istextfile(filename, blocksize=512):
    return istext(open(filename, "rb").read(blocksize))

print istext("123123123123")  # True
# print istextfile('1.bin')  # False
# print istextfile('1.txt')  # True


# 控制大小写
# upper()            THIS IS OUYANG
# lower()             this is ouyang
# capitalize()      This is ouyang
# title()                This Is Ouyang

# 检查字符串是不是符合 开头大写，其余小写的情况

notrans = string.maketrans('', '')


def containsAny(str,strset):
    return len(str) != len(str.translate(notrans,strset))


# 还记得鸭子判断法吗？叫声像(能capitalize)+长得像(containsAny)
def isCapitaliezd(s):
    return s == s.capitalize() and containsAny(s,string.letters)

s1 = 'This is ouyang...'
s2 = 'this is ouyang...'
print isCapitaliezd(s1) # True
print isCapitaliezd(s2) # False


# 访问子字符串
theline = 'abcdefghijklmnopqrstuvwxyz1234567890'

# struct.unpack()按照指定格式
baseformat = "5s 3x 8s 8s"
numremain = len(theline) - struct.calcsize(baseformat)
formater = "%s %ds" % (baseformat,numremain)
l,s1,s2,t = struct.unpack(formater,theline)
print l,s1,s2,t


# struct.unpack()这么优秀的东西 - 封装
def fields(baseformat, theline, lastfield = False):
    numremain = len(theline) - struct.calcsize(baseformat)
    # lastfield and 's' or 'x' == lastfield ? 's':'x'
    format = "%s %d%s" % (baseformat,numremain,lastfield and's'or'x')
    return struct.unpack(formater,theline)

print'lastField == True : ',fields(baseformat,theline,True)
print'lastField == False : ',fields(baseformat,theline,False)


# struct.unpack()这么优秀的东西 - 封装 - memoizing 版本 - 字典做缓存
def fields_memoizing(baseformat, theline, lastfield = False,_cache={}):
    key = baseformat,len(theline),lastfield
    format = _cache.get(key)
    if format == None:
        numremain = len(theline) - struct.calcsize(baseformat)
        _cache[key] = format = "%s %d%s" % (baseformat,numremain,lastfield and's'or'x')
    return struct.unpack(format,theline)

print'lastField == True : ',fields_memoizing(baseformat,theline,True)
print'lastField == False : ',fields_memoizing(baseformat,theline,False)

# 5字节一组,最后不足5字节也不会越界
fivers = [theline[k:k+5] for k in xrange(0,len(theline),5)]
print fivers


# 5字节一组,最后不足5字节也不会越界 - 封装
def split_by(theline, n, lastfield):
    pieces = [theline[k:k+n] for k in xrange(0,len(theline),5)]
    # 如果最后一段太短或不需要，丢弃
    if not lastfield and len(pieces[-1]) < n:
            pieces.pop()
    return pieces

print'split_by lastField == True : ',split_by(theline,5,True)
print'split_by lastField == False : ',split_by(theline,5,False)

# 指定长度的切片,zip()打包成元组
cuts = [8,14,20,26,30]
pieces = [theline[i:j] for i,j in zip([0]+cuts,cuts+[None])]
print pieces


# 指定长度的切片,zip()打包成元组 - 封装
def split_at(theline, cuts, lastfield):
    pieces = [theline[i:j] for i,j in zip([0]+cuts,cuts+[None])]
    # 若不要最后一段，丢弃
    if not lastfield :
            pieces.pop()
    return pieces

print'split_at lastField == True : ',split_at(theline,cuts,True)
print'split_at lastField == False : ',split_at(theline,cuts,False)


# 这已经不是一个fun()了，而是一个generator()，返回一个迭代器对象
def split_at_yeild(theline, cuts, lastfield = True):
    last = 0
    for cut in cuts:
        yield theline[last:cut]
        last = cut
    if  lastfield:
        yield theline[last:]

for i in split_at_yeild(theline,cuts,True):
    print i


# 改变多行文本字符串的缩进
# 关键：str.splitlines(False or True) ,为True的时候保留'\n'
def reinent(s, numSpaces):
    leading_space = numSpaces * ' '
    lines = [leading_space + line.strip() for line in s.splitlines()]
    return'\n'.join(lines)


def addSpaces(s, numAdd):
    white = " "*numAdd
    return white + white.join(s.splitlines(True))


def numSpaces(s):
    return [len(line) - len(line.lstrip()) for line in s.splitlines()]


def delSpaces(s, numDel):
    if numDel > min(numSpaces(s)):
        raise ValueError,"删的空格比最小空格数大"
    return'\n'.join(line[numDel:] for line in s.splitlines())

x = """    line one
    line two
    and line three """
print x
print reinent(x,4)
print addSpaces(x,4)
print delSpaces(x,3)


# 替换字符串中的子串
# 任务要求是：某字符串中被标记的字符串要被按照字典，替换掉。
# 比如说：str = 'just "a" test'   dict = {'a':'one'} ，那么结果是：'just one test'
# 关键词：偶数项就是要替换的子字符串。
# Ps：增加一个安全模式感觉很专业。
def expand(formater, d, marker='"', safe = False):
    if safe:
        # dict.get(key[, default])
        def lookup(w):return d.get(w,w.join(marker*2))
    else:
        def lookup(w):return d[w]
    parts = formater.split(marker)
    # 偶数项 就是要替换的子字符串
    parts[1::2] = map(lookup,parts[1::2])
    return''.join(parts)

formater = 'just  a "a" "b" test '
print expand(formater,{'a':'one','b':'two'})

# more professional
Tem = string.Template('$thing')
Tem.substitute({'thing':5})

# local() 内置函数，存储了本地变量
new_style = string.Template('this is $thing')

print new_style.substitute({'thing':5})
print new_style.substitute({'thing':'test'})
print new_style.substitute(thing = 5)
print new_style.substitute(thing = 'test')


# local() 本地变量
msg = string.Template('the square of $number is $square')
for number in range(10):
    square = number * number
    print msg.substitute(locals())


# 一次完成多个替换
# 考虑到以后很多情况下，dict是固定的，其实我们只要做一次准备工作就够了。而上面的版本是用一次，准备一次。
# 很自然的想到闭包工厂！（在前几节有提到这概念）
# 我们就准备一次transalte，然后就能用translate去无限匹配替换text了。


class make_xlat:
    def __init__(self, *args, **kwds):
        self.adict = dict(*args,**kwds)
        self.rx = self.make_rx()

    def make_rx(self):
        return re.compile('|'.join(map(re.escape, self.adict)))

    def one_xlat(self,match):
        return self.adict[match.group(0)]

    def __call__(self,text):
        return self.rx.sub(self.one_xlat,text)


# 重载
class make_xlat_by_whole_words(make_xlat):
    def make_rx(self):
        return re.compile(r'\b%s\b' % r'\b|\b'.join(map(re.escape,self.adict)))

adict = {"a":"1","b":"2","xyz":"456"}
translate = make_xlat(adict)
translate_by_whole_words = make_xlat_by_whole_words(adict)

print translate('aba xyz cacb')
print translate_by_whole_words('aba xyz cacb')


# 检查字符串中的结束标记
# 我需要先介绍下 itertools.imap（选自Python API）：
def imap(function, *iterables):
    # imap(pow, (2,3,10), (5,2,3)) --> 32 9 1000
    iterables = map(iter, iterables)
    while True:
            args = [next(it) for it in iterables]
            if function is None:
                yield tuple(args)
            else:
                yield function(*args)


# 1、先把参数都变成iter对象，iter是个funciton
# 2、循环调用func来处理iter对象
# 3、如果看不懂yield，请看拓展阅读第二篇
# 检查字符串中的结束标记：
def anyTrue(predicate,sequence):
    return True in itertools.imap(predicate,sequence)


def endswith(s,*endings):
    return anyTrue(s.endswith, endings)

for f in os.listdir('.'):
    if endswith(f,'.jpg','.png','.jepg'):
        print f


# 本节还引入了一个概念：被绑定办法（Bound Method）
L = ['a', 'b', 'c']
x = L.append
x('d')
print L


german_ae = unicode('\xc3\xa4', 'utf8')
print german_ae


