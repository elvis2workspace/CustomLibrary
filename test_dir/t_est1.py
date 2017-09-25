#!/usr/bin/evn python

var_arr=[0]*100

var_arr[0]=1
for i in xrange(1, 100):
    var_arr[i] = var_arr[i-1] + i

print var_arr[99]
print var_arr[0]