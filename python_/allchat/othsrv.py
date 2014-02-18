#! /usr/bin/python

import socket
import os
import select
import sys

def prompt():
    sys.stdout.write('<You> ')
    sys.stdout.flush()

try:
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except:
    print 'Failed to create socket'
    sys.exit()

PORT = 9050
HOST = '127.0.0.1'
RECV_BUFFER = 4096

server_socket.bind((HOST, PORT))
server_socket.listen(10)

input = [server_socket, sys.stdin]

print 'Chat Program'
prompt()

while 1:

    inputready, outputready, exceptready = select.select(input,[],[])

    for sock in inputready:

        if sock == server_socket:
            client, address = server_socket.accept()
            input.append(client)
            #data = sock.recv(RECV_BUFFER)
            #if data:
                #sys.stdout.write(data)
        else:
            data = sock.recv(RECV_BUFFER)
            if data:
                sys.stdout.write(data)
            else:
                msg = sys.stdin.readline()
                server_socket.send('\r<Server>: ' + msg)
                prompt()



server_socket.close()
