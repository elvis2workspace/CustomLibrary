# -*- coding: utf-8 -*-

from __future__ import division
import os
import sys, time

# 动态加载目录下的所有模块
for path in ["src1"]:
    for i in list(set([os.path.splitext(i)[0] for i in os.listdir("./" + path)])):
        if i != "__init__" and i != ".DS_Store":  # 排除不必要的文件
            import_string = "import path" + "." + i
            exec import_string  # 执行字符串中的内容

            # j = '#'
            # for i in range(1,61):
            #     j += '#'
            #     sys.stdout.write(str(int((i/60)*100))+'% ||'+j+'->'+"")
            # sys.stdout.flush()
            # time.sleep(0.1)
