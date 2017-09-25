# coding=utf-8
"""
Created on 2015年3月25日

@author: Administrator
"""
# !/bin/usr/env python

# 导入模块 urllib2
import urllib2
import BeautifulSoup
import re
import MySQLdb


def getURL():
    """getURL:"""
    # 随便查询一篇文章，比如On random graph。对每一个查询google
    # scholar都有一个url，这个url形成的规则是要自己分析的。
    query = 'On+random+graph'
    url = 'http://scholar.google.com/scholar?hl=en&q=' + query + '&btnG=&as_sdt=1%2C5&as_sdtp='
    # 设置头文件。抓取有些的网页不需要专门设置头文件，但是这里如果不设置的话，. from: 1point3acres.com/bbs
    # google会认为是机器人不允许访问。另外访问有些网站还有设置Cookie，这个会相对复杂一些，
    # 这里暂时不提。关于怎么知道头文件该怎么写，一些插件可以看到你用的浏览器和网站交互的
    # 头文件（这种工具很多浏览器是自带的），我用的是firefox的firebug插件。
    header = {'Host': 'scholar.google.com',
              'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; rv:26.0) Gecko/20100101 Firefox/26.0',
              'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
              'Accept-Encoding': 'gzip, deflate',
              'Connection': 'keep-alive'}
    # 建立连接请求，这时google的服务器返回页面信息给con这个变量，con是一个对象
    req = urllib2.Request(url, headers=header)
    con = urllib2.urlopen(req)
    # 对con这个对象调用read()方法，返回的是html页面，也就是有html标签的纯文本. visit 1point3acres.com for more.
    doc = con.read()
    # 关闭连接。就像读完文件要关闭文件一样，如果不关闭有时可以、但有时会有问题，
    # 所以作为一个守法的好公民，还是关闭连接好了。
    con.close()


def parseURL():
    # 导入BeautifulSoup模块和re模块，re是python中正则表达式的模块

    # 生成一个soup对象，doc就是步骤二中提到的

    soup = BeautifulSoup.BeautifulSoup(getURL())
    # 抓取论文标题，作者，简短描述，引用次数，版本数，引用它的文章列表的超链接
    # 这里还用了一些正则表达式，不熟悉的先无知它好了。至于'class' : 'gs_rt'中
    # 'gs_rt'是怎么来的，这个是分析html文件肉眼看出来的。上面提到的firebug插件
    # 让这个变的很简单，只要一点网页，就可以知道对应的html 标签的位置和属性，. 鍥磋鎴戜滑@1point 3 acres
    # 相当好用。. more info on 1point3acres.com
    paper_name = soup.html.body.find('h3', {'class': 'gs_rt'}).text
    paper_name = re.sub(r'\[.*\]', '', paper_name)  # eliminate '[]' tags like '[PDF]'
    paper_author = soup.html.body.find('div', {'class': 'gs_a'}).text
    paper_desc = soup.html.body.find('div', {'class': 'gs_rs'}).text
    temp_str = soup.html.body.find('div', {'class': 'gs_fl'}).text
    temp_re = re.match(r'[A-Za-z\s]+(\d*)[A-Za-z\s]+(\d*)', temp_str)
    citeTimes = temp_re.group(1)
    versionNum = temp_re.group(2)
    if citeTimes == '':
        citeTimes = '0'
    if versionNum == '':
        versionNum = '0'
    citedPaper_href = soup.html.body.find('div', {'class': 'gs_fl'}).a.attrs[0][1]

    # 打开文件webdata.txt，生成对象file,这个文件可以是不存在的，参数a表示往里面添加。. From 1point 3acres bbs
    # 还有别的参数，比如'r'只能读但不能写入，'w'可以写入但是会删除原来的记录等等
    file = open('webdata.txt', 'a')
    line = paper_name + '#' + paper_author + '#' + paper_desc + '#' + citeTimes + '\n'
    # 对象file的write方法将字符串line写入file中-google 1point3acres
    file.write(line)
    # 再一次的，做个随手关闭文件的好青年
    file.close()


def intoMySql():
    # 和服务器建立链接,host是服务器ip，我的MySQL数据库搭建在本机，默认的是127.0.0.1，
    # 用户、密码、数据库名称对应着照输就行了，默认的端口号是3306，charset是编码方式，
    # 默认的是utf8(也有可能是gbk，看安装的版本)。
    conn = MySQLdb.connect(host='127.0.0.1', user='root', passwd='000000', db='dbname', port=3306, charset='utf8')

    # 建立cursor
    cur = conn.cursor()

    # 通过对象cur的execute()方法执行SQL语句
    cur.execute("select * from citeRelation where paperName = 'On Random Graph'")

    # fetchall()方法获得查询结果，返回的是一个list，可以直接这样查询：list[i][j]，
    # i表示查询结果中的第i+1条record，j表示这条记录的第j+1个attribute(别忘了python从0开始计数)
    list = cur.fetchall()

    # 也可以进行delete,drop,insert,update等操作，比如： sql = "update studentCourseRecord set fail = 1 where studentID = '%s' and
    #  semesterID = '%s' and courseID = '%s'" % (studentID, course[0], course[1]) cur.execute(sql)

    # 与查询不同的是，执行完delete,insert,update这些语句后必须执行下面的命令才能成功更新数据库
    #     conn.commit()

    # 一如既往的，用完了之后记得关闭cursor，然后关闭链接
    cur.close()
    conn.close()


if __name__ == '__main__':
    print 'this is first test\n'
    getURL()
    print getURL().__doc__
    parseURL()
    intoMySql()
