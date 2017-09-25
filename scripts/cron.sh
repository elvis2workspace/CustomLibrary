#!/usr/bin/expect -f
spawn scp -r root@***.***.***.***:/bak/bakmysql/*.tar.gz /bak/bakmysql/
set timeout 3000
expect "root@***.***.***.***'s password:"
set timeout 3000
send "*******\r"
set timeout 3000
send "exit\r"
expect eof