#! /usr/bin/python

import sys
import os
import string

filename = sys.argv[1]
fd = open (filename, 'r')
for i in fd:
    # if line = nl(newline), i.strip()[0] -> index out of range.
    if i == '\n' or i.strip()[0] == '#': 
        pass
    else:
        print i
fd.close()











