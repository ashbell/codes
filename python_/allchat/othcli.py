#!/usr/bin/python


import socket
import os
import select
import sys

def prompt():
   sys.stdout.write('<You> ')
   sys.stdout.flush()

HOST = '127.0.0.1'
PORT = 9050
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
    print 'Failed to create socket'
    sys.exit()

s.connect((HOST, PORT))

print 'Connected to remote host. Start sending messages'
prompt()

while 1:

    socket_list = [sys.stdin, s]

    read_sockets, write_sockets, error_sockets = select.select(socket_list, [], [])

    for sock in read_sockets:
        if sock == s:
            data = sock.recv(4096)
            if not data:
                print '\nDisconnected from chat server'
                sys.exit()
            else:
                sys.stdout.write(data)
                prompt()
        else:
            msg = sys.stdin.readline()
            s.send('\r<Client>: ' + msg)
            prompt()
