#!/usr/bin/python

from socket import *
import sys
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
    	clidata = tcpCliSock.recv(BUFSIZ)
	if clidata:
		print ('[%s] %s' % (addr, clidata))
	else:
#srvdata = raw_input('>>')
		srvdata = sys.stdin.readline()
		tcpCliSock.send('<srv> %s\n' % srvdata)

tcpSerSock.close()
