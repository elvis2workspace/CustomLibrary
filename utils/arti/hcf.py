#coding=utf-8
'''
Created on 2015年9月22日

@author: Elvis
'''

import heapq

def hcf(x, y):
    for i in range(1, min(x,y)+1):
        if ((x % i == 0) and (y % i == 0)):
            hcf = i
            
    return hcf

def mgb(x, y):
    hcfe = hcf(x, y)
    return x*y/hcfe

def lcm(x, y):
    greater = max(x,y)
    while(True):
        if((greater%x==0) and (greater%y==0)):
            lcm = greater
            break
        greater += 1
    return lcm

def recur_fibo(n):
    
    if n <= 1:
        return n
    
    else:
        return (recur_fibo(n-1) + recur_fibo(n-2))

def sstring_oper():
    params = {"server":"mpilgrim", "database":"master", "uid":"sa", "pwd":"secret"}    
    for k, v in params.items():
        print "%s=%s" % (k, v) 
    for k, x in params.items():   
        print ";".join("%s=%s" % (k, v))

def buble(lis):
    for i in range(len(lis), 0, -1):
        for j in range(len(lis)-1):
            if lis[j] > lis[j+1]:
                tmp = lis[j]
                lis[j] = lis[j+1]
                lis[j+1] = tmp
    return str(lis)
    

#插入排序算法 
def insertion_sort(sort_list):
    iter_len = len(sort_list)
    if iter_len <= 2:
        return sort_list
    
    for i in range(1, iter_len):
        key = sort_list[i]
        j = i-1
        while j>=0 and sort_list[j]>key:
            sort_list[j+1] = sort_list[j]
            j -= 1
        sort_list[j+1] = key
    return sort_list
    
#选择排序算法
def selection_sort(sort_list):
    iter_len = len(sort_list)
    if iter_len <= 2:
        return sort_list
    
    for i in range(iter_len-1):
        smaller = sort_list[i]
        location = i
        
        for j in range(i, iter_len):
            if sort_list[j] < smaller:
                smaller = sort_list[j]
                location = j
        if i != location:
            sort_list[i], sort_list[location]=sort_list[location], sort_list[i]
            '''交换两个变量的值,在python中你就可以这么写：a, b = b, a，
                              因为赋值符号的左右两边都是元组，在python中，元组其实是由逗号“,”来界定的，
                              而不是括号
            '''
    return sort_list

def quick_sort_2(sort_list):
    if len(sort_list)<=1:
        return sort_list
    return quick_sort_2([lt for lt in sort_list[1:] if lt<sort_list[0]]) + \
           sort_list[0:1] + \
           quick_sort_2([ge for ge in sort_list[1:] if ge>=sort_list[0]])
           

def HeapSort(list):
    heapq.heapify(list)    
    heap = []
    while list:
        heap.append(heapq.heappop(list))
    list[:]=heap
    return list
        
if __name__ == '__main__':
    sstring_oper()
    li = [21,44,2,45,33,4,3,67]
    print "Heap Sort: ",HeapSort(li)
    print "Buble Sort: ",buble(li)
    print "Insert Sort: ",insertion_sort(li)
    print "Selection Sort: ",selection_sort(li)
    
    num1 = int(raw_input("first: "))
    num2 = int(raw_input("second: "))
    num3 = int(raw_input("third: "))
    
    print hcf(num1, num2)
    print mgb(num1, num2) 
    print lcm(num1, num2)
    
    for i in range(num3):
        print recur_fibo(i)
    
    