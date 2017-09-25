#!/bin/bash
/usr/bin/sysbench  \
/usr/share/sysbench/oltp_read_write.lua \
--time=$1 \
--oltp-dist-type=uniform \
--mysql-host=10.0.100.$2 \
--mysql-port=3306 \
--mysql-user=elvis \
--mysql-password=Auto@123 \
--mysql-db=auto \
--mysql-table-type=innodb \
--oltp-test-mode=complex \
--oltp-table-size=15000000 \
--oltp-tables-count=1 \
--max-requests=0 \
--threads=$3 \
--report-interval=10 \
--report-checkpoints=10 \
--percentile=99 \
$4 | tee /home/ubuntu/log/sysbench-oltp-$(date +%F"-"%H-%M-%S).log \
