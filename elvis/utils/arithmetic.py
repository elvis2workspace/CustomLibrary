#coding=utf-8
#!/usr/bin/python

#定义求质数的函数
def getprim(n):
#我们从3开始，提升效率，呵呵，微乎其微啦
    p=3
    x=0
    while(x<n):
        result=True
        for i in range(2,p-1):
            if(p%i==0):
                result=False
        if result==True:
            x=x+1
            rst=p
#注意:这里加2是为了提升效率，因为能被双数肯定不是质数。
        p+=2
    print(rst)


#字符列表去重
def uniq(s_list):
    if s_list:
        s_list.sort()
        last = s_list[-1]
        for i in range(len(s_list)-2, -1, -1):
            if last==s_list[i]:
                del s_list[i]
            else:
                last=s_list[i]

    return s_list

#定义冒泡排序
def bubble_sort(seq):
    for i in range(len(seq)):
        for j in range(i,len(seq)):
            if seq[j] < seq[i]:
                tmp = seq[j]
                seq[j] = seq[i]
                seq[i] = tmp
#定义选择排序              
def selection_sort(seq):
    for i in range(len(seq)):
        position = i
        for j in range(i,len(seq)):
            if seq[position] > seq[j]:
                position = j
        if position != i:
                tmp = seq[position]
                seq[position] = seq[i]
                seq[i] = tmp
#定义插入排序
def insertion_sort(seq):
    if len(seq) > 1:
        for i in range(1,len(seq)):
            while i > 0 and seq[i] < seq[i-1]:
                tmp = seq[i]
                seq[i] = seq[i-1]
                seq[i-1] = tmp
                i = i - 1

#调试调用函数              
if  __name__ == '__main__':
    getprim(1000)
    List=uniq(['b','b','d','b','c','a','a'])
    print "after deleting the repeated element the list is : " , List
    print "--------bubble_sort-------------"
    seq = [22,1,33,4,7,6,8,9,11]
    bubble_sort(seq)
    print seq
    print "--------selection_sort-------------"
    seq = [88,44,33,4,7,6,8,9,11]
    selection_sort(seq)
    print seq
    print "--------insertion_sort-------------"
    seq = [777,44,33,4,7,6,1111,100,11]
    insertion_sort(seq)
    print seq