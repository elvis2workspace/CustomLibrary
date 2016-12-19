#coding=utf-8
'''
Created on 2015年3月5日

@author: Elvis
'''
#!/usr/bin/python
#Filename:using_dict.py

ab = { 'Swaroop' : 'swaroopch@byteofpythoninfo',
      'Larry' : 'larry@wall.org',
      'Matsumoto' : 'matz@ruby-lang.org',
      'Spammer' : 'spammer@hotmail.com'
    }

print "Swaroop's address is %s" % ab['Swaroop']

#Adding a key/value pair
ab['Guido']  = 'guido@python.org'

#Deleting a key/value pair
del ab['Spammer']

print '\nThere are %d contacts in the address-box\n' %len(ab)
for name, address in ab.items():
    print 'Contact %s at %s' % (name, address)
    
if 'Guido' in ab:
    print "\nGuido's address is %s" %ab['Guido']