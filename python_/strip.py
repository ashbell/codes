#! /usr/bin/python

while True:
   out = []
   s =  raw_input('Enter strings:')
   left = 0
   right = len(s)
   while s[left] == ' ':
      left = left + 1
   while s[right -1] == ' ':  # Be carefull right -1.. its usefull!
      right = right - 1
   for i in range(left, right):
      out.append(s[i])
   print ''.join(out)   # Ahaha~, we can use ''.join(obj) to convert list to str.
   del out              # list[] -> str and tuple() -> str.
# Function : range(star, stop) create : star, star+1, star+2,.....stop-1
#
# Examples:       ~~~~This is my string~~~# 
#                 01234                  ^^
#                                        ||
#                                        ||
#                                        |len(string)
#                                        |--> len(string) -1
#
#
#
#
#
#
#
#Fri Jan 11 22:49:37 CST 2013









