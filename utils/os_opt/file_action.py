# -*- coding: utf-8 -*-
import os

download_path = "C:\Users\hcb\Downloads"


def check_file(base_path, *file_t):
    if os.path.isdir(base_path):
        print "is dir."

    for f in file_t:
        f_path = os.path.join(base_path, f)

        rts = os.path.exists(f_path)
        if rts:
            print "The %s file have been existed." % f_path
        else:
            print "The %s file is not existed." % f_path


if __name__ == '__main__':
    check_file(download_path, u"车辆基础信息-20161027101240.xls", u"车辆基础信息-20161027105630.xls")
