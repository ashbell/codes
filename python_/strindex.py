#! /usr/bin/python

# Wed Jan  9 22:43:57 CST 2013

import string
strindex = []
def StrGetIndex(s1, s2):
   begin = 0;
   end = len (s2)
   loop = True
   while loop:
      index = s2.find(s1, begin, end)
      if index < 0:
         loop = False
      else:
         strindex.append(index)
         begin = index + len (s1)
      #print 'Find %s at %s of  %d\n' % (s1, s2, index)

s1 = raw_input('Enter substring:')
s2 = raw_input('Enter mainxtring:')
StrGetIndex(s1, s2)
N = len(strindex)
if len(strindex) == 0:
    print 'nothing find'
    exit()
print '%s at\n%s index is' % (s1, s2),
for i in range(N):
    #print '%s at' % s1
    #'%s is %s' % (s1, s2, strindex[i]) Error: int str format convert.
    print strindex[i],






