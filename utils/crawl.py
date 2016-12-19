import re
import requests
r=requests.get('http://stock.jrj.com.cn/share,600031,zcfzb.shtml')
data=r.text
result =re.findall(r"<tr><td\s*class=\u0022txl\u0022>([^<]+)</td><td>", data)
