#!/usr/bin/env python

from socket import *
from time import ctime

HOST = ''
PORT = 21567
BUFSIZ = 1024
ADDR = (HOST, PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)

while True:
    print 'waiting for connection...'
    tcpCliSock, addr = tcpSerSock.accept()
    print '...connected from:', addr

    while True:
        data = tcpCliSock.recv(BUFSIZ)
        if not data:
            break
<<<<<<< HEAD
        print "Receive data is %s" %  data
            
#   tcpCliSock.send('Your command is %s' % data)
#   tcp send() is buffer?
#        tcpCliSock.send('[%s] %s' % (
#            ctime(), data))
        tcpCliSock.send('%s-1\n' %  data)
        pass
        pass
        pass
        pass
        pass
        #Oh fuck! delay time is needed by multi send()
        tcpCliSock.send('%s-2\n' %  data)
        print data 
=======
        tcpCliSock.send('[%s] %s' % (
            ctime(), data))
>>>>>>> a35a9dcf7cc447b2d5116160b82eda0bc2cfa178

    tcpCliSock.close()
tcpSerSock.close()
