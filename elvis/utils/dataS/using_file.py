#coding=utf-8
'''
Created on 2015年3月6日

@author: Elvis
'''
#!/usr/bin/python

poem = '''\
Programming is fun
When the work is done 
if you wanna make your work also fun:
  use Python!
'''

f = file('poem.txt', 'w')
f.write(poem)
f.close()

f = file('poem.txt')
while True:
    line = f.readline()
    if len(line) == 0:
        break
    print len(line), line,
    
f.close()