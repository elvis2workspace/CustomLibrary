#!/usr/bin/env python

import os
import re
import subprocess

localIpConfig = os.popen('ipconfig').read()
# print localIpConfig
# localIpConfig = "IPv4 adress is 192.168.4.126. The gateway is 192.168.4.1."
localAddress = re.search(r'192.168.4.*', localIpConfig)

if localAddress:
    print localAddress.group(0)
else:
    print "NO match!"

# os.system('appium -a 192.168.4.153 -p 4723 --no-reset')

launchCMD = "appium -a " + localAddress.group(0) + " -p 4723 --no-reset"
print launchCMD
# appiumRestls = os.popen(launchCMD).read()
os.system(launchCMD)

