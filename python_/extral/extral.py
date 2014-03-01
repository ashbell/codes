#! /usr/bin/python

import os
import zlib
import sys
import string
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
	def __init__( self, decomfn, extract_tom ,\
			files,   co_out ):
		self.decomfn     = decomfn 
		self.extract_tom = extract_to
		self.files       = files
		self.co_out      = co_out
		print 'Options Parse OK....'

	def decompress( self, decomfn, extract_to ):
		print '[ Now Decompress archiver: ]', decomfn, 'extract to --> ', extract_to
		print
	def compress( self, files, compress ):
	 	print '[ Now   Compress  file(s): ]', files, 'compress to --> ', co_out
		print
	def de_compress( self, decomfn, extract_to, files, co_out ):
		print '[ Decompress  archiver: ]  %-20s' % decomfn, 'Output_Path--> %40s' % extract_to
		print '[ Now   Compress files: ]  %-20s' % files ,  'Output File--> %40s' % co_out
		print

p = Archiver( decomfn, extract_to, files, co_out )
#p.compress( files)
#p.decompress(decomfn)
p.de_compress( decomfn, extract_to, files, co_out )










