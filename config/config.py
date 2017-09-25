import os
from datetime import *

GLOBAL_LOG = "D:\\Logs\\Debug\\"
RUNTIMETAG = "robot-runlog-%date:~0,4%%date:~5,2%%date:~8,2%"
ROBOTLOGPATH = "D:\\PS_auto_project\\Logs\\robotf-runlog\\robotf-runlog-" + datetime.now().strftime('%Y%m%d%H')

MAXVERSIONS = 100
APPIUM_RUNLOG = "D:\\PS_auto_project\\Logs\\appium-runlog\\appium-runlog-" + str(date.today()) + "-" + \
                str(datetime.now().hour) + "-" + str(datetime.now().minute) + "\\"

APPIUM_LOCALPATH = "E:\\Program Files\\nodejs\\node_global\\"

APPIUMPORT = 4723
LOCALADDRESS = "http://192.168.20.114:4723/wd/hub"
AAPTToolPATH = "E:\\Android\\android-sdk\\build-tools\\22.0.1\\"
APKLOCALPATH = "D:\\PS_auto_project\\apps\\"
CRASHLOGPATH = "D:\\Logs\\error_log"
ONEDRIVE = "/Users/Elvis/OneDrive"

PATH = lambda p: os.path.abspath(
    os.path.join(os.path.dirname(__file__), p))

print PATH
