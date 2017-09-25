#!/bin/bash
sysbench  \
--test=/home/ubuntu/sysbench-0.5/sysbench/tests/db/oltp.lua \
--max-time=$1 \
--oltp-dist-type=uniform \
--mysql-host=10.0.100.$2 \
--mysql-port=3306 \
--mysql-user=elvis \
--mysql-password=Auto@123 \
--mysql-db=auto \
--mysql-table-type=innodb \
--oltp-test-mode=complex \
--oltp-table-size=15000000 \
--max-requests=0 \
--num-threads=$3 \
--report-interval=10 \
--report-checkpoints=10 \
$4 \
