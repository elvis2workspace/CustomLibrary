#!/bin/bash

for((i=1;i<11;i++));do

"dd" if=test_20M.log of=test_"$i"_"$RANDOM"_10M.log bs=1024k count=10

done
