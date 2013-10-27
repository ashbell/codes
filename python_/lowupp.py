#! /usr/bin/python

st = str(raw_input('Enter string:'))

out = []
for i in st:
    if i > 'a' and i < 'z':
        out.append(i.upper())
    elif i > 'A' and i < 'Z':
        out.append(i.lower())
    else:
        out.append(i)
print ''.join(out) # Very usefull.


