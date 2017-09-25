# -*- coding: utf-8 -*-

import csv, requests, re
from bs4 import BeautifulSoup
import os

url = 'https://www.v2ex.com/?tab=all'
html = requests.get(url).text
soup = BeautifulSoup(html, 'html.parser')
articles = []
for article in soup.find_all(class_='cell item'):
    title = article.find(class_='item_title').get_text()
    category = article.find(class_='node').get_text()
    author = re.findall(r'(?<=<a href="/member/).+(?="><img)', str(article))[0]
    u = article.select('.item_title > a')
    link = 'https://www.v2ex.com' + re.findall(r'(?<=href=").+(?=")', str(u))[0]
    articles.append([title, category, author, link])

if not os.path.exists('document'):
    os.makedirs('document')

with open(r'document/v2ex.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['文章标题', '分类', '作者', '文章地址'])
    for row in articles:
        print row
        writer.writerow(str(row))

# with open(filename, 'wb') as fd:
#     for chunk in r.iter_content(chunk_size):
#         fd.write(chunk)
