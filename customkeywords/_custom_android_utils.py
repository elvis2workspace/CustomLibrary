#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on 2015年7月21日

@author: zhang.xiuhai
'''

from  robot.api import logger
import os
import re
import subprocess
import time

MAXVERSIONS = 100 
RUNLOG = "D:\\Logs\\appium-runlog\\"
APPIUMPORT = 4723 
LOCALADDRESS = "http://192.168.20.114:4723/wd/hub"

class _CustomAndroidKeywords(object):
    '''
    classdocs
    '''

    def __init__(self):
        '''
        Constructor
        '''
    #public
    def get_local_address(self):    
        u'''获取本地的地址.例
        
        '''
        tmpconfig = os.popen('ipconfig').read()
        ip = re.search(r'192.168.*.*', tmpconfig)
        return ip.group(0)
    
    def launch_local_appium(self, ip="127.0.0.1", tport="4723", mode="no-reset"):
        u'''根据ip，port，mode启动本地appium，例           
        
        '''
        runlogfilestr = "appium-runlog-%date:~0,4%%date:~5,2%%date:~8,2%-"+str(int(time.mktime(time.strptime(time.ctime(), "%a %b %d %H:%M:%S %Y"))))[4:]+".txt"
        print runlogfilestr
        launchCMD = "appium -a " + str(ip) + " -p " + str(tport) + " " + "--" + mode \
        + " " + "--log-timestamp --local-timezone --session-override -g " +RUNLOG+runlogfilestr 
        
        tmppid = self.get_port_pid(APPIUMPORT)
        
        if None == tmppid:
            child = subprocess.Popen(launchCMD, shell=True)
            time.sleep(20)
            logger.info("Launch the local appium successfully.", also_console=True)
            return child.pid
        else:
            time.sleep(10)
            logger.info("Appium tool have launched, pid: "+tmppid, also_console=True)
            return tmppid
        
    def stop_tookit(self, toolniki):
        u'''停止测试工具,例如 本地Appium
        '''
        if "appium" == toolniki:
            pid = self.get_port_pid(APPIUMPORT)
            logger.info("going to stop local appium, pid: ."+str(pid), also_console=True) 
            stopAppiumCMD = "tskill " +str(pid)
            child = subprocess.Popen(stopAppiumCMD, shell=True)
            child.wait()
            return child.pid
            
        else:
            logger.console("No other tools to use.")
            
    def get_port_pid(self, port):   
        u'''根据参数中的端口号查找对应使用该端口号的进程ID，并返回该进程的PID号。
        '''
        getPidCMD = "netstat -ano | findstr  LISTENING | findstr " + str(port)
        appiumPidStr = os.popen(getPidCMD).read()
        print appiumPidStr
        
        if appiumPidStr:
            appiumG = appiumPidStr.split(' ')
            return appiumG[-1]
            logger.console("process about port " + str(port) +" is "+appiumG[-1]+" .", True, 'stdout')
        else:
            logger.console("No process about port " + str(port) +"!", True, 'stdout')
        
    def set_androidlog_status(self, flag=False, mode=True):
        u'''设置android日志开关
        '''
        srchAdbCMD = "tasklist | findstr adb"
        logPid = self.get_CMD_pid('adb.exe')
        
        if mode == True:
            logCMD = "adb shell logcat -v time >D:\Logs\logcat_" + flag + ".log &1"
            subprocess.Popen(logCMD, shell=True)
        elif mode == False:
            for i in logPid:
                logoffCMD = "tskill " + i
                child = subprocess.Popen(logoffCMD, shell=True)
                child.wait()
        else:
            return -1
        
        logger.debug("Debug on.", html=True)
        
    def grap_androidLog_afterOper(self, flag, path):  
        u'''获取操作后日志
        
        '''
        os.system("adb logcat -v time -d > "+path+"log_" + flag + ".log &1")
    
    
    def get_CMD_pid(self, tcmd):
        u'''获取执行命令的进程ID
        
        '''
        srchAdbCMD = "tasklist | findstr adb.exe"
        rStr = os.popen(srchAdbCMD).read()
        rg = rStr.split(' ')
        pidList = []
        num = 0
        for i in rg:
            if re.search(tcmd, i):
                num = num + 1
            elif re.match(r'\d{5}|\d{4}', i):
                pidList.append(i)
            else:
                pass
            
        if num <= 1 :
            logger.console("The process about "+ tcmd +" is not exist.", True)
        else:
            return pidList[1:num]
    
    def kill_adb_process(self, pro_alias='ecm',remote_url=None ):
        u'''杀掉移动端指定包名的进程
        '''
        adbCmd = "adb shell ps | grep "+str(pro_alias)+" | grep -v ecmapplication:"
        
#         for key in kwargs:
#             print "key words: " % (key, kwargs[key])
            
#         desired_caps = kwargs
#         if None == remote_url:
        proDetails = os.popen(adbCmd).read()
#         else:
#             tmpapp = webdriver.Remote(str(remote_url), desired_caps)
#             tmpapp.execute(adbCmd)
            
        logger.info(proDetails, also_console=True)
        isNull = (len(proDetails)==0)
        
        if isNull:
            print pro_alias +" is not alive."
            return;
        else: 
            adbPid = proDetails.split(' ')
            retVal = os.system("adb shell kill "+ str(adbPid[4]))
            
            if retVal == 0:
                print pro_alias + " "+ str(adbPid[4]) + " killed successfully."
            else:
                print pro_alias + " "+ str(adbPid[4]) + " process fail to kill!"
        
    def Cswipe(self, xstart, ystart, xend, yend):
        u'''自定义滑动屏幕关键字
        '''  
        os.system("adb shell input swipe "+str(xstart)+" "+str(ystart)+" " +str(xend)+" "+str(yend))
        return 0  
    
    def run_all_bat(self,path):
        u'''接收一个目录的路径，并执行目录下的所有bat文件.例
         | run all bat | filepath| 
        '''
        for root,dirs,files in os.walk(path):
            for f in files:
                if os.path.splitext(f)[1] == '.bat':
                    os.chdir(root)
                    #print root,f
                    os.system(f)

    
    def decode(self,customerstr):
        return customerstr.decode('utf-8')
    
    
    #private
    def _execute_sql(self, path):
        logger.debug("Executing : %s" % path)
        print path
        
if __name__ == '__main__':
    tmpObject = _CustomAndroidKeywords()
#     tmppro = tmpObject.launch_local_appium("192.168.20.114" , "4723", "no-reset")
#     print "run the testcase."
#     tmpObject.get_port_pid("4723")
#     time.sleep(10)
#     tmpObject.get_local_address()
#     tmpObject.stop_tookit("appium")
#     tmpObject.kill_adb_process('ecm')
#     tmpObject.set_androidlog_status('test729343434322', True)
#     tmpIp = tmpObject.get_local_address()
    
