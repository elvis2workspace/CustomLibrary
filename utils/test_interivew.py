# -*- coding: utf-8 -*-

# delete the same element in a list

l = ['b', 'c', 'd', 'b', 'c', 'a', 'a']

# use set
list(set(l))

# use dictionary
l2 = {}.fromkeys(l).keys()
print l2

# 用字典并保持顺序
l1 = ['b', 'c', 'd', 'b', 'c', 'a', 'a']
l2 = list(set(l1))
l2.sort(key=l1.index)
print l2


# 列表推导式
l1 = ['b', 'c', 'd', 'b', 'c', 'a', 'a']
l2 = []
[l2.append(i) for i in l1 if i not in l2]
print l2

# 6 链表成对调换
# 1->2->3->4转换成2->1->4->3.


class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None


class Solution:
    # @param a ListNode
    # @return a ListNode

    def __init__(self):
        pass

    def swap_pairs(self, head):
        if head is not None and head.next is not None:
            next_e = head.next
            head.next = self.swap_pairs(next_e.next)
            next_e.next = head
            return next_e
        return head

l_tmp = [1, 2, 3, 4]

for i in l_tmp:
    ln_tmp = ListNode(i)
    solution = Solution()
    re_list = solution.swap_pairs(ln_tmp)
    print re_list.val,

# 7 创建字典的方法
# 1 直接创建
dict_exam = {'name': 'earth', 'port': '80'}

# 2 工厂方法
items = [('name', 'earth'), ('port', '80')]
# dict2 = dict_exam(items)
# dict1 = dict_exam((['name', 'earth'], ['port', '80']))

# 3 fromkeys()方法
dict1 = {}.fromkeys(('x', 'y'), -1)
dict_exam = {'x': -1, 'y': -1}
dict2 = {}.fromkeys(('x', 'y'))
dict2 = {'x': None, 'y': None}

# 8 合并两个有序列表
# 知乎远程面试要求编程
#
# 尾递归


def _recursion_merge_sort2(list_1, list_2, tmp):
    if len(list_1) == 0 or len(list_2) == 0:
        tmp.extend(list_1)
        tmp.extend(list_2)
        return tmp
    else:
        if list_1[0] < list_2[0]:
            tmp.append(list_1[0])
            del list_1[0]
        else:
            tmp.append(list_2[0])
            del list_2[0]
        return _recursion_merge_sort2(list_1, list_2, tmp)


def recursion_merge_sort2(list_1, list_2):
    return _recursion_merge_sort2(list_1, list_2, [])


# 循环算法
def loop_merge_sort(list_1, list_2):
    tmp = []
    while len(l1) > 0 and len(l2) > 0:
        if l1[0] < l2[0]:
            tmp.append(l1[0])
            del l1[0]
        else:
            tmp.append(l2[0])
            del l2[0]
    tmp.extend(l1)
    tmp.extend(l2)
    return tmp
