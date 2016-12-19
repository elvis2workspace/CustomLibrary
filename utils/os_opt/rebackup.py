#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Created on 2015年4月1日

@author: zhangxiuhai
"""
import os


def get_dir(file_path):
    print file_path, '\n'
    return os.listdir(file_path)


def bak_file(file_path, file_path_bak):
    file_path_list = os.listdir(file_path)
    for l in file_path_list:
        file_path = os.path.join(file_path, l)
        file_path_bak = os.path.join(path_bak, l)  
        print '[debug]: '+file_path
        print '[debug1]: '+file_path_bak

        # 如果文件路径为目录
        if os.path.isdir(file_path):
            print 'The source path is %s' % file_path

            # 如果在备份目录中文件夹不存在则创建
            if not os.path.isdir(path_bak):  
                create_com = '''mkdir -p '%s' ''' % file_path_bak
                if os.system(create_com) == 0:  
                    print create_com   
                else:  
                    print 'create folder failure!'  
                    os._exit(0) 
                bak_file(file_path, file_path_bak)  
        else:  
            # 如果文件已经存在，则比较文件修改时间
            if os.path.isfile(file_path_bak):  
                stat_bak = os.stat(file_path_bak)  
                stat_source = os.stat(file_path) 
                     
                # 判断文件修改时间
                if stat_source.st_mtime <= stat_bak.st_mtime:  
                    continue  
                    cp_com = '''cp '%s' '%s' ''' % (file_path, file_path_bak)  
                    if os.system(cp_com) == 0:   
                        print cp_com  
                    else:   
                        print 'create folder failure!'  
                        os._exit(0)   
                
if __name__ == '__main__':
    # 要备份的文件目录
    path = 'D:\\robotframework\\test_1' 
    # 备份文件目录
    path_bak = 'F:\\documents\\backup' 
    # 开始备份
    bak_file(path, path_bak) 