#!/usr/bin/python

from  socket import *

HOST = '127.0.0.1'
PORT = 25123
BUFFSZ = 1024
ADDR = (HOST, PORT)


while True:
	tcpcliSock = socket(AF_INET, SOCK_STREAM)
	tcpcliSock.connect(ADDR)
	data = raw_input('>>')
	if not data:
		break
	tcpcliSock.send('%s\r\n' % data)
	data = tcpcliSock.recv(BUFFSZ)
	if not data:
		break
	print 'server say:  ',data.strip()
	tcpcliSock.close()






