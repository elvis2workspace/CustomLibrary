# -*- coding: utf-8 -*-

"""
Created on 2015年5月8日
@author: elvis.zhang
"""

import filecmp
import os
import shutil

from CustomLibrary.config import config

PATH = lambda p: os.path.abspath(p)

class BackupKeywords(object):
    """
    备份文件
    """

    def __init__(self, params):
        """
        Constructor
        """
        self.dest_path = params

    # public
    # 备份目录下指定目录下所有文件
    def backup(self, tree_top):
        bakdir_name = self.dest_path
        for root, subdirs, files in os.walk(tree_top):
            # join链接出每个root下的子目录bakdir
            backup_dir = os.path.join(root, bakdir_name)

            # 确保每个root下都有个子目录叫bakdir
            if not os.path.exists(backup_dir):  
                os.makedirs(backup_dir)  

            # bakdir下的不递归处理
            subdirs[:] = [d for d in subdirs if d != bakdir_name]
      
            for file_o in files:
                filepath = os.path.join(root, file_o)
                destpath = os.path.join(backup_dir, file_o)

                # 检查版本，共有MAXVERSIONS个版本
                for index in xrange(config.MAXVERSIONS):
                    backup = "%s.%2.2d" % (destpath, index)
                    if not os.path.exists(backup):  
                        break  
                if index > 0:  
                    old_backup = "%s.%2.2d" % (destpath,index-1)  
                    # abspath = os.path.abspath(filepath)  # filepath本来就是绝对路径
                    try:  
                        # 如果备份和源文件一致，continue不处理
                        if os.path.isfile(old_backup) and filecmp.cmp(filepath,old_backup,shallow = False):  
                            continue  
                    except OSError:  
                            pass  
                try:  
                    shutil.copy(filepath, backup)
                except OSError:  
                    pass

    # adb backup命令可以备份，该脚本只用于备份设备上安装的第三方应用，将apk保存在当前目录下的backup_app文件夹中
    def get_apk_list(self):
        apps = []
        for apk in utils.shell("pm list packages -f -3").stdout.readlines():
            apps.append(apk.split(":")[-1].split("=")[0])

        return apps

    def backup_app(self):
        apps = self.get_apk_list()
        for apk in apps:
            utils.adb("pull %s backup_app" % apk).wait()
            print "pull %s succeed." % apk


if __name__ == '__main__':
    bk = BackupKeywords('bakdir')
    bk.backup("/Users/Elvis/Documents")

    path = PATH("%s/backup_app" % os.getcwd())
    if not os.path.isdir(path):
        os.mkdir(path)
    bk.backup_app()
    print "Completed."
