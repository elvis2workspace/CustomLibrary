# -*- coding: utf-8 -*-

import os
import shutil
from CustomLibrary.lib.FileOpt.fileLib import *

dst_path1 = "/Users/Elvis/pycharm_projects/CustomLibrary/test_dir/src3"
src_path1 = "/Users/Elvis/pycharm_projects/CustomLibrary/test_dir/src"

# 复制路径下的所有文件到指定目录
# def exec_dir(src_path, dst_path):
#     if os.path.isfile(src_path):
#         if os.path.exists(dst_path) is not True:
#             os.mkdir(dst_path)
#         shutil.copy(src_path, dst_path)
#     elif os.path.isdir(src_path):
#         print src_path + "is directory."
#         for file_sub in os.listdir(src_path):
#             print src_path + "/" + file_sub
#             exec_dir(src_path + "/" + file_sub, dst_path)
#     else:
#         print "error."
#
file_obv = FileUtils()
file_obv.exec_dir(src_path1, dst_path1)
