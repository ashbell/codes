#!/usr/bin/python

from socket import *

HOST = 'localhost'
PORT = 21567
BUFSIZ = 1024
ADDR = (HOST, PORT)

tcpCliSock = socket(AF_INET, SOCK_STREAM)
tcpCliSock.connect(ADDR)

while True:
    data = raw_input('> ')
    if not data:
        break
    tcpCliSock.send(data)
    srvdata = tcpCliSock.recv(BUFSIZ) 
    if not srvdata:
        break
    print srvdata

tcpCliSock.close()
