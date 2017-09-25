# -*- coding: utf-8 -*-
import os
import time

opt_dir = r"D:\\temp"
"""一、取得当前目录"""
s = os.getcwd()
# s 中保存的是当前目录(即文件夹)
# 比如运行abc.py，那么输入该命令就会返回abc所在的文件夹位置。
# 举个简单例子，我们将abc.py放入A文件夹。并且希望不管将A文件夹放在硬盘的哪个位置，都可以在A文件夹内生成一个新文件夹。且文件夹的名字根据时间自动生成。

folder = time.strftime(r"%Y-%m-%d_%H-%M-%S",time.localtime())
os.makedirs(r'%s/%s'%(os.getcwd(),folder))

"""二、更改当前目录"""
dir_name = "C:\\123"
try:
    os.chdir(dir_name)
except:
    print "%s is not existed." % dir_name
finally:
    print "change current dir."
# 将当前目录设为 "C:\123", 相当于DOC命令的 CD C:\123
# 说明：当指定的目录不存在时，引发异常。
# 异常类型：WindowsError

"""三、将一个路径名分解为目录名和文件名两部分"""
# fpath , fname = os.path.split( "你要分解的路径")
# 例如：
a, b = os.path.split( "c:\\123\\456\\test.txt" )
print a
print b
# 显示：
# c:\123\456
# test.txt

"""四、分解文件名的扩展名"""
# fpathandname , fext = os.path.splitext( "你要分解的路径")
# 例如：
a, b = os.path.splitext("c:\\123\\456\\test.txt" )
print a
print b
# 显示：
# c:\123\456\test
# .txt

"""五、判断一个路径（ 目录或文件）是否存在"""
b = os.path.exists(opt_dir)
# 返回值b： True 或 False

"""六、判断一个路径是否是文件"""
b = os.path.isfile(opt_dir)
# 返回值b： True 或 False

"""七、判断一个路径是否目录"""
b = os.path.isdir(opt_dir)
# 返回值b： True 或 False

"""八、获取某目录中的文件及子目录的列表"""
# 1、获取某指定目录下的所有子目录的列表


def getDirList(p):
        p = str( p )
        if p == "":
            return []
        p = p.replace("/", "\\")
        if p[-1] != "\\":
             p += "\\"
        a = os.listdir( p )
        b = [x for x in a if os.path.isdir(p + x)]
        return b
print getDirList("C:\\")


# 2、获取某指定目录下的所有文件的列表
def getFileList(p):
        p = str(p)
        if p == "":
            return []
        p = p.replace("/", "\\")
        if p[-1] != "\\":
             p += "\\"
        a = os.listdir(p)
        b = [x for x in a if os.path.isfile(p + x)]
        return b
print getFileList("C:\\")

"""九、创建子目录"""

os.makedirs("C:\\123\\456\\789")
# 调用有可能失败，可能的原因是：
# (1) path 已存在时(不管是文件还是文件夹)
# (2) 驱动器不存在
# (3) 磁盘已满
# (4)磁盘是只读的或没有写权限

"""十、删除子目录"""
os.rmdir()   # path: "要删除的子目录"
# 产生异常的可能原因:
# (1) path 不存在
# (2) path 子目录中有文件或下级子目录
# (3) 没有操作权限或只读
# 测试该函数时，请自已先建立子目录

"""十一、删除文件"""
os.remove("")   # filename: "要删除的文件名"
# 产生异常的可能原因:
# (1)   filename 不存在
# (2) 对filename文件，没有操作权限或只读。

"""十二、文件改名"""
os.name("oldfilename", "newfilename")
# 产生异常的原因：
# (1) oldfilename 旧文件名不存在
# (2) newFilename 新文件已经存在时，此时，您需要先删除 newFilename 文件。


