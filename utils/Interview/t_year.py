#!/usr/bin/env python

year = int(input("please input year:\n"))

if year % 4 == 0 and year % 100 != 0 or year % 400 == 0:
    print ("{0} year is run".format(year))
else:
    print ("{0} year is not run".format(year))

