#!/bin/bash
output=$(tput sgr0)

#定义监控的主机地址，可根据自己需要改变
nginx_server='http://127.0.0.1'

#封装Nginx服务监控的函数
Check_Nginx()
{

#使用curl获取状态码
Status_code=$(curl -m 5 -s -w %{http_code} ${nginx_server} -o /dev/null)

#判断状态码如果等于000或者大于500的我们就输出错误，否则我们输出内容
if [ $Status_code -eq 000 -o $Status_code -ge 500 ]; then
echo -e '\E[33m' "检查http服务错误，错误状态码是：" $output $Status_code
else
http_content=$(curl -s ${nginx_server})
echo -e '\E[32m' "http服务正常，状态码是：" $output $Status_code
fi
}

#调用一下

Check_Nginx