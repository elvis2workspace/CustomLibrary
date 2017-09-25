#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import time
from bs4 import BeautifulSoup

# 导入sklearn中的KMeans进行聚类分析
from sklearn.cluster import KMeans

# 导入pandas库
import pandas as pd

# 导入图表库
# import matplotlib.pyplot as plt

# 导入数值计算库
import numpy as np

# 设置列表页URL的固定部分
url = 'http://bj.lianjia.com/ershoufang/'
# 设置页面页的可变部分
page = 'pg'
# 此外，还需要在很http请求中设置一个头部信息，否则很容易被封。头部信息网上有很多现成的，也可以使用httpwatch等工具来查看。具体细节按照具体情况进行调整。
# 设置请求头部信息
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 '
                  'Safari/537.11',
    'Accept': 'text/html;q=0.9,*/*;q=0.8',
    'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
    'Accept-Encoding': 'gzip',
    'Connection': 'close',
    'Referer': 'http://www.baidu.com/link?url=_andhfsjjjKRgEWkj7i9cFmYYGsisrnm2A-TN3XZDQXxvGsM9k9ZZSnikW2Yds4s&wd'
               '=&eqid=c3435a7d00006bd600000003582bfd1f '
}

# 循环抓取列表页信息
for i in range(1, 100):
    if i == 1:
        i = str(i)
        a = (url + page + i + '/')
        r = requests.get(url=a, headers=headers)
        html = r.content
    else:
        i = str(i)
        a = (url + page + i + '/')
        r = requests.get(url=a, headers=headers)
        html2 = r.content
        html = html + html2
    # 每次间隔0.5秒
    time.sleep(0.5)

# 解析抓取的页面内容
lj = BeautifulSoup(html, 'html.parser')

# 完成页面解析后就可以对页面中的关键信息进行提取了。下面我们分别对房源的总价，房源信息和关注度三部分进行提取。
# 把页面div标签中class=priceInfo的部分提取出来，并使用for循环将其中每个房源的总价数据存在tp中。
# 提取房源总价

price = lj.find_all('div', attrs={'class': 'priceInfo'})
tp = []
for a in price:
    totalPrice = a.span.string
    tp.append(totalPrice)

# 提取房源信息
houseInfo = lj.find_all('div', attrs={'class': 'houseInfo'})
hi = []
for b in houseInfo:
    house = b.get_text()
    hi.append(house)

# 提取房源关注度
followInfo = lj.find_all('div', attrs={'class': 'followInfo'})
fi = []
for c in followInfo:
    follow = c.get_text()
    fi.append(follow)

"""创建数据表"""
house = pd.DataFrame({'totalprice': tp, 'houseinfo': hi, 'followinfo': fi})
# 查看数据表的内容
house.head()

"""对房源信息进行分列"""
houseinfo_split = pd.DataFrame((x.split('|') for x in house.houseinfo), index=house.index,
                               columns=['xiaoqu', 'huxing', 'mianji', 'chaoxiang', 'zhuangxiu', 'dianti'])
# 这是完成分列后的新数据表，房源的各种信息以及成为单独的字段。
# 查看分列结果
houseinfo_split.head()

"""将分列结果拼接回原数据表"""
house = pd.merge(house, houseinfo_split, right_index=True, left_index=True)
# 完成拼接后的数据表中既包含了原有字段，也包含了分列后的新增字段。
# 查看拼接后的数据表
house.head()

"""对房源关注度进行分列"""
followinfo_split = pd.DataFrame((x.split('/') for x in house.followinfo), index=house.index,
                                columns=['guanzhu', 'daikan', 'fabu'])
# 将分列后的关注度信息拼接回原数据表
house = pd.merge(house, followinfo_split, right_index=True, left_index=True)

# 按房源户型类别进行汇总
huxing = house.groupby('huxing')['huxing'].agg(len)
# 查看户型汇总结果
print huxing

# 绘制房源户型分布条形图
# plt.rc('font', family='STXihei', size=11)
# a = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20])
# plt.barh([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20], huxing, color='#052B6C', alpha=0.8,
#          align='center', edgecolor='white')
# plt.ylabel('户型')
# plt.xlabel('数量')
# plt.xlim(0, 1300)
# plt.ylim(0, 20)
# plt.title('房源户型分布情况')
# plt.legend(['数量'], loc='upper right')
# plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.4)
# plt.yticks(a, (
#     '1室0厅', '1室1厅', '1室2厅', '2室0厅', '2室1厅', '2室2厅', '3室0厅', '3室1厅', '3室2厅', '3室3厅', '4室1厅', '4室2厅', '4室3厅', '5室2厅',
#     '5室3厅',
#     '6室1厅', '6室2厅', '7室2厅', '7室3厅'))
# plt.show()

# 对房源面积进行二次分列
mianji_num_split = pd.DataFrame((x.split('平') for x in house.mianji), index=house.index, columns=['mianji_num', 'mi'])

# 将分列后的房源面积拼接回原数据表
house = pd.merge(house, mianji_num_split, right_index=True, left_index=True)

# 分列后的数据在使用前还需要进行清洗，通常的操作包括去除空格和格式转换。下面我们先对房源面积的值去除两端的空格，然后更改数值的格式以方便后面的计算。
# 去除mianji_num字段两端的空格
house['mianji_num'] = house['mianji_num'].map(str.strip)

# 更改mianji_num字段格式为float
house['mianji_num'] = house['mianji_num'].astype(float)

# 清洗后的房源面积字段可以开始分析了。首先查看所有北京在售二手房的面积范围，下面是代码和结果。房源面积从18.85到332.63。
# 查看所有房源面积的范围值
house['mianji_num'].min(), house['mianji_num'].max()
(18.850000000000001, 332.63)

# 有了房源面积的范围后，就可以对面积进行分组了，我们以50为区间将房源面积分为7组。并统计所有房源在这7组中的分布情况。
# 对房源面积进行分组
bins = [0, 50, 100, 150, 200, 250, 300, 350]
group_mianji = ['小于50', '50-100', '100-150', '150-200', '200-250', '250-300', '300-350']
house['group_mianji'] = pd.cut(house['mianji_num'], bins, labels=group_mianji)

# 按房源面积分组对房源数量进行汇总
group_mianji = house.groupby('group_mianji')['group_mianji'].agg(len)

# 使用房源面积分组字段对房源数量进行分组并绘制条形图。
# # 绘制房源面积分布图
# plt.rc('font', family='STXihei', size=15)
# a = np.array([1, 2, 3, 4, 5, 6, 7])
# plt.barh([1, 2, 3, 4, 5, 6, 7], group_mianji, color='#052B6C', alpha=0.8, align='center', edgecolor='white')
# plt.ylabel('面积分组')
# plt.xlabel('数量')
# plt.title('房源面积分布')
# plt.legend(['数量'], loc='upper right')
# plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.4)
# plt.yticks(a, ('小于50', '50-100', '100-150', '150-200', '200-250', '250-300', '300-350'))
# plt.show()

# 对房源关注度进行二次分列
guanzhu_num_split = pd.DataFrame((x.split('人') for x in house.guanzhu), index=house.index,
                                 columns=['guanzhu_num', 'ren'])
# 将分列后的关注度数据拼接回原数据表
house = pd.merge(house, guanzhu_num_split, right_index=True, left_index=True)

# 去除房源关注度字段两端的空格
house['guanzhu_num'] = house['guanzhu_num'].map(str.strip)

# 更改房源关注度及总价字段的格式
house[['guanzhu_num', 'totalprice']] = house[['guanzhu_num', 'totalprice']].astype(float)

# 清洗完后查看所有房源关注度的区间，关注度从0到725。也就是说有些房子很热门，而有些房子没有人关注。这可能和房源上线和更新的情况有关，此外还要考虑房源的销售速度，热门房源可能很抢手，刚上线就成交了。因此我们对情况进行简化，暂时忽略掉这些复杂的情况。仅对关注度的分布情况进行统计。
# 查看房源关注度的区间
house['guanzhu_num'].min(), house['guanzhu_num'].max()
(0.0, 725.0)

# 将关注度以100为区间分为8组，并按关注度区间进行汇总统计房源数量。查看在售房源的关注度分布情况。
# 对房源关注度进行分组
bins = [0, 100, 200, 300, 400, 500, 600, 700, 800]
group_guanzhu = ['小于100', '100-200', '200-300', '300-400', '400-500', '500-600', '600-700', '700-800']
house['group_guanzhu'] = pd.cut(house['guanzhu_num'], bins, labels=group_guanzhu)
group_guanzhu = house.groupby('group_guanzhu')['group_guanzhu'].agg(len)

# 绘制房源关注度分布条形图。
# 绘制房源关注度分布图
# plt.rc('font', family='STXihei', size=15)
# a = np.array([1, 2, 3, 4, 5, 6, 7, 8])
# plt.barh([1, 2, 3, 4, 5, 6, 7, 8], group_guanzhu, color='#052B6C', alpha=0.8, align='center', edgecolor='white')
# plt.ylabel('关注度分组')
# plt.xlabel('数量')
# plt.xlim(0, 3000)
# plt.title('房源关注度分布')
# plt.legend(['数量'], loc='upper right')
# plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.4)
# plt.yticks(a, ('小于100', '100-200', '200-300', '300-400', '400-500', '500-600', '600-700', '700-800'))
# plt.show()

# 使用房源总价，面积和关注度三个字段进行聚类
house_type = np.array(house[['totalprice', 'mianji_num', 'guanzhu_num']])

# 设置质心数量为3
clf = KMeans(n_clusters=3)

# 计算聚类结果
clf = clf.fit(house_type)

# 通过计算我们将在售房源分为三个类别，下面是每个类别的中心点坐标。
# 查看分类结果的中心坐标
clf.cluster_centers_array([[772.97477064, 112.02389908, 58.96330275],
                           [434.51073861, 84.92950236, 61.20115244],
                           [1473.26719577, 170.65402116, 43.32275132]])
# 在原数据表中标注所属类别
house['label'] = clf.labels_
