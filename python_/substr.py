#! /usr/bin/python

from  findchr import findchar

def substr(mainstr, origstr, newstr):
    subindex = []
    tmp = []
    out = ''
    lennew  = len (newstr)
    lenmain = len (mainstr)
    lenorig = len (origstr)
    
    subindex = findchar(mainstr, origstr)
    mainstr = list( mainstr)
    newstr  = list (newstr)
    start = 0
    end = 0
    # oH, fuck up, it's so easy!, waste my a week!
    for val in subindex:
        end = val
        tmp.append(mainstr[start:end])
        tmp.append(newstr)
        start = val + lenorig
    tmp.append(mainstr[start:lenmain])

    for i in tmp:
        out =  out + ''.join(i)
    return out

loop = True
while loop:
    mainstr = str(raw_input('Enter mainstr:'))
    origstr = str(raw_input('Enter origstr:'))
    newstr  = str(raw_input('Enter newstr:'))

    st = substr(mainstr, origstr, newstr)
    print st

# Sun Jan 20 20:26:37 CST 2013


