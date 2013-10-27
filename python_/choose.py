#! /usr/bin/python

# Print Function Menu.

print '------Function Menu Choose--------'
print '------1. Add Five Numbers---------'
print '------2. Average Five Numbers ----'
print '------3. Quit---------------------'

COUNTER = 5
list = [0, 0, 0, 0, 0]
def get5sum(list):
    asum = 0     #local var and global var's different!
    for i in range(5):
        asum = asum + list[i]
    return asum

def average5(list):
    sum = get5sum(list)
    return (float(sum) / 5)

def input():
    for i in range(COUNTER):
        num = raw_input('number :')
        list[i] = int(num)
    return list

loop = True
while (loop):
    ans = raw_input('Choose your function :')
    if ('1' == ans):
       print  "sum is :%d" %  get5sum(input())
    elif ('2' == ans):
       print  "average is :%f" % average5(input())
    elif ('3' == ans):
        loop = False
        print 'OK, we exit..!'
        exit()
    else:
        print 'fuck up!'

    
#Tue Jan  1 23:16:50 CST 2013
