#! /usr/bin/python

loop = True
while loop:
    start = int (raw_input ('Start:'))
    end   = int (raw_input ('End:'))
    if start > end or start == end:
        print 'fuck up,stupid and fool'
        exit()

    print '-' * 60
    print '%10s%15s%10s%10s%10s' % ('Dec', 'Bin', 'Oct', 'Hex', 'ASCII')
    for i in range(start, end):
        print '-' * 60
        if start > 256 or end > 256:
            print '%10d' % i, '%15s' % bin(i).upper(), '%#10o' % i, '%#10X' % i 
        else:
            print '%10d' % i, '%15s' % bin(i).upper(), '%#10o' % i, '%#10X' % i, '%10c' % chr(i)
        # %010X will add '0' before hex-string.
    print '-' * 60



