#! /usr/bin/python

# get filename
fname = raw_input("Enter your filename:")

# attempt to ope file for reading
try:
    fobj = open(fname, 'r')
except IOError, e:
    print "file '%s' reading error!" % fname
else:
    for eachline in fobj:
        print eachline,
    fobj.close()

