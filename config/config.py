import time
from datetime import *

GLOBAL_LOG = "D:\\Logs\\Debug\\"
RUNTIMETAG = "robot-runlog-%date:~0,4%%date:~5,2%%date:~8,2%"
ROBOTLOGPATH = "D:\\PS_auto_project\\Logs\\robotf-runlog\\robotf-runlog-" + datetime.now().strftime('%Y%m%d%H')