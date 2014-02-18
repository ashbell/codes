#! /usr/bin/python

import threading
from time import ctime

class MyThread(threading.Thread):
	def __init__(self, func, args, name=''):
		threading.Thread.__init__(self)
		self.func = func
		self.args = args
		self.name = name

	def getResult(self):
		return self.result
	
	def run(self):
		print 'starting', self.name, '@', ctime()
		self.result = apply(self.func, self.args)
		print self.name, 'finished @', ctime()















