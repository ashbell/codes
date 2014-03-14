#! /usr/bin/python

import os
import zipfile
import sys
import string
import time
import shutil
from optparse import OptionParser

Usage = 'Usage:  %s -de or -d CompressedFiles -to Path \n\t%s -co or -c Files -to ArchFile' % (sys.argv[0], sys.argv[0])
parser = OptionParser( Usage )
parser.add_option( "-d", "--de",
		   action  = "store",
		   help    = "decompress archiver.",
		   type    = "string",
		   dest    = "decomfn" 
		 )
parser.add_option( "-t", "--to",
		   action  = "store",
		   help    = "the output path.",
		   type    = "string",
		   dest    = "extract_to"
		 )
parser.add_option( "-c", "--co",
		   action  = "store",
		   help    = "compress files.",
		   type    = "string",
		   dest    = "files"
		 )
parser.add_option( "-o", "--out",
		   action  = "store",
		   help    = "the files you wan to compress out.",
		   type    = "string",
		   dest    = "co_out"
		 )

if len( sys.argv ) < 2:
	print Usage
	exit( 0 )
args = sys.argv[1:]
(opts, args) =  parser.parse_args( args )

if opts.decomfn:
	action = 'decompress'
if opts.files:
	action = 'compress'
if opts.decomfn and opts.files:
	action = 'de_compress'
if ( not opts.extract_to ) or ( not opts.co_out ):
	print
	print 'Oops, you must specify the extract to   path!'
	print 'Or the file you want to compress from where.'
	print 
	exit(0)
# for decompress
decomfn     = opts.decomfn 
extract_to  = opts.extract_to
# for compress
files       = opts.files
co_out      = opts.co_out

class Archiver:
	def __init__( self, decomfn, extract_to ,\
			files,   co_out ):
		self.decomfn     = decomfn 
		self.extract_to  = extract_to
		self.files       = files
		self.co_out      = co_out
		print 'Options Parse OK....'

	def zip_decompress( self, decomfn, extract_to ):
		print '[ Now Decompress archiver: ]', decomfn, 'extract to --> ', extract_to
		print
		if not zipfile.is_zipfile( decomfn ):
			print 'Oh, %s is not a zip file' % decomfn
			exit( 0 )
		if not os.path.exists( extract_to ):
			print 'Path: %s is not exist! Try to create it. MODE = 777' % extract_to
			os.makedirs( extract_to )
		if os.path.exists( extract_to ):
			print 'Extract %s to %s . exsisted !!!' % ( decomfn, extract_to)
			f        = zipfile.ZipFile( decomfn , 'r')
			namelist = f.namelist()
			# Extract all
			#f.extractall( extract_to, namelist )	
			for items in  namelist:
				if os.path.isdir( items ): # Root path is existed.
					print '%s is directory and existed .!' % items,
					choice = raw_input('(A)nother place? or (O)verwrite it?' )[0]
					if ((choice == 'A') or (choice == 'a')):
						while True:
							newdirname = raw_input( 'new place: ')
							print 'getcwd: %s ' % os.getcwdu()
							newpalce = '%s/%s' % ( os.getcwdu(), newdirname )
							print 'New dir palce: %s' % newpalce
							if os.path.isdir( newpalce ):
								print 'Choose another one !'
							else:
								extract_to = newpalce
								break
					elif ((choice == '0') or (choice == 'o')):
						extract_to = os.getcwdu()
					print 'Target: ', extract_to
				elif os.path.exists( items ): # File is existed.
					print 'File: %s is existed.!, (R)ename or (O)verwrite it?  ' % items,
					c = raw_input( )[0]
					if ( c == 'R' ) or ( c == 'r' ):
						while True:
							rename = raw_input( 'Another file name: ' )
							newfilepath = '%s%s%s' % ( os.path.dirname( items),'/', rename )
							print 'New file path: %s ' % newfilepath
							if os.path.exists( newfilepath ):
								print 'Fuck u, choose another one.!'
							else:
								nf = open( newfilepath, 'ab+')
								if not nf:
									print 'create newfile % error !' % newfilepath
									exit( 0 )
								nf.write( f.read( items ) )
								nf.close()
								break
					elif (( c == 'O') or ( c == 'o')):
						print 'Default is overwrite it'
					else:
						print 'Unknown cmd!'
						exit( 0 )
				else:
					print 'Ex---> %s' % items
					f.extract( items, extract_to )
			f.close()
		else:
			print 'Create %s Fail! Choose another path' % extract_to
			exit( 0 )

	@staticmethod
	def path_check_modify( path ):
		if not os.path.exists( path ):
			print 'File Pat:  %s not exist.!' % path,
			while ( not os.path.exists( path ) ):
				files = raw_input( 'Waht do u want:  ')
		if os.path.isdir( path ) and (path[-1:] != '/'):
			path = '%s%s' % (path, '/')
		return path

	def zip_compress( self, files, co_out ):
	 	print '[ Now   Compress  file(s): ]', files, 'compress to --> ', co_out
		print
		namelist = []
		files = Archiver.path_check_modify( files )

		if os.path.isdir( files ):
			print 'DIR: %s, Compress all files in it.!' % files
			list = os.listdir( files )
			for x in list:
				namelist.append( '%s%s' % ( files, x ) )
		
	def zip_de_compress( self, decomfn, extract_to, files, co_out ):
		print '[ Decompress  archiver: ]  %-20s' % decomfn, 'Output_Path--> %40s' % extract_to
		print '[ Now   Compress files: ]  %-20s' % files ,  'Output File--> %40s' % co_out
		print

p = Archiver( decomfn, extract_to, files, co_out )
p.zip_compress( files, co_out ) 
#p.zip_decompress( decomfn, extract_to)
#p.de_compress( decomfn, extract_to, files, co_out )







