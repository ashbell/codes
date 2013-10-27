#! /usr/bin/python

# Find substring in mainstring.
# substr   = 'abc'
# mainstr  = 'habcking', return index 1, return -1 otherwise.
# fucking the string like:
#           hh -> hhhhh lenght of mainstr % 2 != 0
#           ini -> iiniiniin   
#           in -> inininininininini
#           ini -> iiniiniin
#           in  -> inini

# Oh my god! who can help me?

#index = []
def findchar(string, char):
    index = []
    lenstring = len (string)
    lenchar = len (char)
    i = j = k = c = 0
    while i < lenstring:
        j = 0
        if string[i] == char[j]:
            k = i
        # print str(i), str(k), str(j)
        # while (j < lenchar) and (string[k] == char[j])
        # while (j < lenchar) and (k < lenstring):  # k must be less than lenstring,
                                                  # when 'in' match 'inini' k = 5, fatal error
        while (j < lenchar) and (k < lenstring) and (string[k] == char[j]):
            k = k + 1
            j = j + 1
            # print str(k), str(j)
            if j > 0 and (j > lenchar - 1):
                index.append (i)
                c = c + 1
                i = i + lenchar - 1 # i step
                break
        i = i + 1
#print str(c)
    if c == 0:
        print 'nothing find'
    if c > 0:
        print 'find at:', ''.join(str(index))
	return index

if __name__ == '__main__':
	string = str(raw_input('Enter mainstr:'))
	char   = str(raw_input('Enter  substr:'))
	findchar (string, char)

# Mon Jan 14 22:24:23 CST 2013

# k get value from i ...for we use 'in' match 'inini'
# i < 5, but when i == 4, the lastest letter 'i' match the first letter of substr
# so k = k + 1 == 5.  God damn it ! k must less than 5.



