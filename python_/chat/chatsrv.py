#!/usr/bin/python

from SocketServer import (TCPServer as TCP,
			StreamRequestHandler as SRH)

from time import ctime

HOST = ''
PORT = 25123
ADDR = (HOST, PORT)

class MyRequestHandler(SRH):
	def handle(self):
		print '...connected from:' , self.client_address
		print ('[%s] say:  %s' % (self.client_address, self.rfile.readline()))
		srvbuf = raw_input('you say >>>>')
		self.wfile.write('[%s]' % srvbuf)
##self.wfile.write('[%s] %s' % (ctime(), self.rfile.readline()))

tcpcliServ = TCP(ADDR, MyRequestHandler)
print 'waiting for connection...'
tcpcliServ.serve_forever()

