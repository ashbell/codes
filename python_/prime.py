#! /usr/bin/python

num = int (raw_input('Enter number:'))
fac_list  = range(1, num + 1)

print 'Before:', fac_list

i = 0
while i < len(fac_list):
    if num % fac_list[i] == 0:
        print '%d %% %d = %d' % ( num, fac_list[i], num % fac_list[i])
        del fac_list[i]
        i = i - 1 # i should be back.
    i = i + 1
print 
print 'After:', fac_list,












