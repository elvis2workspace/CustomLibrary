import random
from CustomLibrary.utils.DataUtils.sort_functions import *


methods = (normal_find_same, quick_find_same, insertion_sort_r, bubble_sort_r, selection_sort_r)
alist = range(5000)
print random.shuffle(alist)

for m in methods:
    print "The method %s spends %s" % (m.__name__, record_time(m, alist))