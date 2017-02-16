#!/usr/bin/env python 
import os
import telnetlib, sys

def telnetdo(HOST=None, USER=None, PASS=None, COMMAND=None): #define a function 
    if not HOST: 
        try:
            HOST = sys.argv[1] 
            USER = sys.argv[2] 
            PASS = sys.argv[3] 
            COMMAND = sys.argv[4] 
        except: 
            print "Usage: remote.py host user pass command"
            return 
    tn = telnetlib.Telnet() # 
    try: 
        tn.open(HOST) 
    except: 
        print "Cannot open host"
        return 
    tn.read_until("login:") 
    tn.write(USER + '\n') 
    if PASS: 
        tn.read_until("Password:") 
        tn.write(PASS + '\n') 
        tn.write(COMMAND + '\n') 
        tn.write("exit\n") 
        tmp = tn.read_all() 
        tn.close() 
        del tn 
        return tmp 
        
if __name__ == '__main__': 
    print telnetdo()