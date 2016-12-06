import cookielib
import sys
import urllib
import urllib2

reload(sys)
sys.setdefaultencoding("utf-8")

# open facebook
jar = cookielib.LWPCookieJar()
cookieprocessor = urllib2.HTTPCookieProcessor(jar)
opener = urllib2.build_opener(cookieprocessor,urllib2.HTTPHandler)
urllib2.install_opener(opener)

host_url = "https://www.facebook.com"

headers = {"":"",}
headers = {"Accept-Language":"en-us",
                  "Content-Type":"application/x-www-form-urlencoded",
                  "Referer":"https://www.facebook.com/#!/?sk=nf",
                  "User-Agent":"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.3; .NET4.0C; .NET4.0E)",
           }

request = urllib2.Request(host_url, headers=headers)
response = urllib2.urlopen(request)
content = response.read()


# login facebook
login_data = {
    "email": "Luannaiwh811@gmail.com",
    "pass": "iwhzjbhq59",
           }

data = urllib.urlencode(login_data)
login_url = "https://www.facebook.com/login.php?login_attempt=1"
request = urllib2.Request(login_url, data, headers=headers)
response = urllib2.urlopen(request)
content = response.read()


# post content

post_url = "https://www.facebook.com/ajax/updatestatus.php?av=100008228327077"

post_data = {"xhpc_message":"1234567890",
             "xhpc_publish_type": "1",
             "xhpc_message_text": "1234567890",
             "xhpc_targetid": "100008228327077",
             "xhpc_ismeta": "1",
             "xhpc_context": "home",
             "xhpc_timeline": "1",
             "xhpc_composerid": "u_0_1a",
             "privacyx": "300645083384735",
             }

data = urllib.urlencode(post_data)
request = urllib2.Request(post_url, data, headers=headers)
response = urllib2.urlopen(request)
content = response.read()
