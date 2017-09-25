#!/bin/bash

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# 备份服务器的IP
server="192.168.1.179"

# 备份服务器的SSH用户账号
user="root"

# 备份服务器的SSH服务端口号
port=22

SSH="SSH -p $port -l $user -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no"

# 避免同时启动同一个脚本多次的锁文件
lock_file="/opt/rsync/lock"

# 需备份的源目录
sync_dir="/opt/source/sync"


function rsync_data()
{
local i
local n
local ret

n=`ls $sync_dir | wc -l`

if [ $n -ge 1 ]; then
echo "Info: $n files need rsync"

rsync -az -e "$SSH" $sync_dir $server:~/ 2>/dev/null
ret=$?

if [ $ret -eq 0 ]; then
for i in `ls $sync_dir`
do
$SSH $server "mv ~/sync/$i ~/backup/data" 2>/dev/null

if [ $? -eq 0 ]; then
rm -f "$sync_dir/$i"
fi
done
else
echo "Error: rsync return $ret"
ret=1
fi
else
echo "Info: no file need rsync"
ret=0
fi

return $ret
}

if [ -f $lock_file ]; then
pid=`cat $lock_file`
if ps -p $pid > /dev/null ; then
exit 0
fi
fi

echo $$ > $lock_file

# 同步数据
rsync_data 

# 删除锁文件
rm -f $lock_file

exit 0
