#!/usr/bin/env python
# -*- coding:UTF-8 -*-   

import os  
import subprocess  
  
class ExeMgr(object):  
    '''  
Python管理应用程序  
    '''  
  
    def __init__(self,appPath):  
        '''  
        appPath:要启动的应用程序的路径  
        pid:启动的进程id  
        '''  
        self.appPath = appPath  
        self.pid = None  
          
    def start(self):  
        '''  
            启动应用程序  
        '''  
        #判断应用程序路径是否存在  
        if(os.path.exists(self.appPath)):  
            p = subprocess.Popen(self.appPath)  
            self.pid = p.pid  
            if self.pid is None:  
                return False  
            return True  
        else:  
            print '应用程序路径'+self.appPath+'不存在'  
              
              
if __name__ == '__main__':  
      
    exeMgr = ExeMgr(r"C:\Program Files\Tencent\QQ\Bin\QQ.exe")  
    exeMgr.start()  
    print '程序已成功启动'  