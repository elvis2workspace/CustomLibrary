#coding=utf-8
#!/usr/bin/python
import re
import urllib


def getHtml(url):
	page = urllib.urlopen(url)
	html = page.read()
	page.close()
	print html
	return html

def getWeather(html):
	reg = '<script type=.*?></script>'
	weatherList = re.compile(reg).findall(html)
	print weatherList
	return weatherList


htmlexg = getHtml('http://www.weather.com.cn/weather/101270101.shtml')

print getWeather(htmlexg)
