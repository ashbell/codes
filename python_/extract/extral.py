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
		self.extract_to = extract_to
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
				#print '-- %s ...file' % items 
				if os.path.exists( items ):  # file's path is exist. tips it!
				    print 'File: %s is exist ! What do u want? Rename ? Overwrite?' % items
				    choice = raw_input( 'Choice: ' )[0]
				    if ( choice == 'R' ) or ( choice == 'r' ):
				    	file_name = raw_input( 'Another file name: ' )
					dir_name = os.path.dirname( items )
					ex_newpath = '%s/%s' % ( dir_name, file_name )
					print 'New extract path: %s ' % ex_newpath 
					if not os.path.exists( ex_newpath ):
						f.extract( items, ex_newpath )
						ex_path = '%s/%s' % ( ex_newpath, items )
						print ex_path
						real_path = '%s/%s' % ( dir_name, file_name )
						print real_path
						shutil.copy( ex_path, './tmpfile')
						os.unlink(ex_path)
						os.removedirs( os.path.dirname(ex_path ))
						os.renames( './tmpfile', real_path )
						print 'Remove %s...' % ex_path
						print 'Rename Ok...'
				        else:
						print 'You are The Fucking Fool!, I wont\'t Working For you!'
						exit( 0 )
				    elif ( choice == 'O' ) or ( choice == 'o' ):
				    	extract_to = '%s%s' % ( extract_to, '../' )
					f.extract( items, extract_to )
				    else:
				    	print 'Bad options, exit !'
					exit( 0 )
				time.sleep( 0.5 )
		else:
			print 'Create %s Fail! Choose another path' % extract_to
			exit( 0 )
			 	

	def zip_compress( self, files, compress ):
	 	print '[ Now   Compress  file(s): ]', files, 'compress to --> ', co_out
		print
	def zip_de_compress( self, decomfn, extract_to, files, co_out ):
		print '[ Decompress  archiver: ]  %-20s' % decomfn, 'Output_Path--> %40s' % extract_to
		print '[ Now   Compress files: ]  %-20s' % files ,  'Output File--> %40s' % co_out
		print

p = Archiver( decomfn, extract_to, files, co_out )
#p.compress( files)
p.zip_decompress( decomfn, extract_to)
#p.de_compress( decomfn, extract_to, files, co_out )







