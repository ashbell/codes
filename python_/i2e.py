#! /usr/bin/python

import string
# 0 ~ 999,999,999,999
# Integer base
base = {'one' : 1, 'ten' : 10, 'hundred' : 100,
        'thousand' : 1000, 'million' : 1000000,
        'billion' : 1000000000
       }

# n1 billions and n2 millions and n3 thousands and n4 hundreds and n5 tentys-n6 ones

partn = []
def PartitionN1(n):
    billion = n / long(base['billion'])
    partn.append(billion)
    t = n % long(base['billion'])
    million = t / long(base['million'])
    partn.append(million)
    t = n % long(base['million'])
    thousand = t / long(base['thousand'])
    partn.append(thousand) 
    t = n % long(base['thousand'])
    hundred = t / long(base['hundred'])
    partn.append(hundred) 
# the lastest two num
    t = n % long(base['hundred'])
#    ten = t / long(base['ten'])
#    partn.append(ten)
#    one = n % long(base['ten'])
#    partn.append(one)
    partn.append(t)
    if billion > 999:
        print 'it\'s to large!'
        exit()
    

in20 = { 0:' ',         1:'one',       2:'two',         3:'three',      4:'four', 
         5:'five',      6:'six',       7:'seven',       8:'eight',      9:'nine', 
        10:'ten',      11:'eleven',   12:'twelve',     13:'thirteen',  14:'fourteen', 
        15:'fifteen',  16:'sixteen',  17:'seventeen',  18:'eighteen',  19:'nineteen' 
       }
in100 = { 2:'twenty', 3:'thirty',  4:'forty',  5:'fifty', 
          6:'sixty',  7:'seventy', 8:'eighty', 9:'ninety'
        }

output = [] 
def Int2Eng(n):
    # fuck up. if not do like this will be crash! 
    # local variable 'p1' referenced before assignment
    p1 = p2 = p3 = 0 
    
    if  n > 0 and n < 1000:
        p1 = (n / 100)       #[0 1 2 3 4 5 6 7 8 9 ]
        p2 = ((n % 100) / 10 )
        p3 = (n % 10)
    if p1 == 1:
        hunsufix = 'hundred'
    elif p1 > 1 and p1 < 10:
        hunsufix = 'hundreds'
    else:
        hunsufix = ' '
    if p1 == 0 or p2 == 0 and p3 == 0:
        link = ' '
    else:
        link = 'and'
    # Less than 20
    if p2 == 0 or p2 == 1:
        p2 = p3 + p2 * 10
        p3 = 0
        tenty = in20  # if less than 20, search in in20
    else:
        tenty = in100 # if more than 20, search in in100
    
    output.append( (str( in20[p1]+'-'+hunsufix+'-'+link+'-'+tenty[p2]+'-'+in20[p3])).strip()
                )
loop = True

strbase = {  0:'billions', 1:'millions', 2:'thousands',
             3:'thunders', 4:'',   5:''
                           # tentys and ones
          }
strbasenull = { 0:'', 1:'', 2:'', 3:'', 4:'', 5:''}

while loop:
    n = long(raw_input('Enter n:'))
    PartitionN1(n)
    #print partn
    #[1,2,3,4,5]
    for i in partn:
        Int2Eng(int(i))

    for i in range(0, len(output)):
        insqua = str(output[i].replace('-',' ') ).strip() 
        if insqua == '':
            lsqbra = ''
            rsqbra =''
            unitsufix = strbasenull # avoid to []millions status!
                                    # if is empty, millions also be null
        else:
            lsqbra = '['
            rsqbra = ']'
            unitsufix = strbase     # if is not empty, using normal strbase.
        print  lsqbra + insqua + rsqbra \
              +' '+unitsufix[i],
    output = []
    partn = []
    print 




