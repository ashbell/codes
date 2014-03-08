#! /usr/bin/python
#-*-coding:utf-8-*-

import re
import os
import string
import time

#=============================
# The head_part of index_page
#=============================
index_start = '''

<!DOCTYPE html>
<html>
  <head>
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="chrome=1">
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    <link href='https://fonts.googleapis.com/css?family=Architects+Daughter' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" type="text/css" href="stylesheets/stylesheet.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="stylesheets/pygment_trac.css" media="screen" />
    <link rel="stylesheet" type="text/css" href="stylesheets/print.css" media="print" />

    <!--[if lt IE 9]>
    <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->

    <title>小生在此恭候多时了:)</title>
  </head>

  <body>
    <header>
      <div class="inner">
        <h2>你不努力，谁都给不了你想要的生活</h2>
       <font color="#3B0B0B"> <h3>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		&nbsp;&nbsp;&nbsp;Ashbell[2014.2.11,@Shenzhen]</h3> </font>
        <a href="https://github.com/ashbell/blogs" class="button"><small>View project on</small>GitHub</a>
      </div>
    </header>

    <div id="content-wrapper">
      <div class="inner clearfix">
        <section id="main-content">
          <h3>
<a name="welcome-to-github-pages" class="anchor" href="#welcome-to-github-pages"><span class="octicon octicon-link"></span></a>2014-02-11 >>>> 欢迎您，来自CyberWorld的朋友 ：P</h3>

<p>这是我在Github上的博客，今天决定把博客搬进这里来了，：P .本人Diaosi一个，今年24,目前大学本科毕业1年半，在深圳打工混日子，从事着和自己专业兴趣完全不对口的生产业. 喜欢Linux，C， Bash脚本，热爱编程，打算入职嵌入式程序员，本科学的是电子信息工程. 欢迎您的到来.
<h3>
<a name="delimiter" class="anchor" ><span class="octicon octicon-link"></span></a>----------------------------------------------------------------------------------------------------------</h3>
<pre><code> '''

#=============================
# The tail_part of index_page
#=============================
index_end = '''

既然你在22岁时贪图安逸，就不要抱怨自己在32岁时进退两难。

</code></pre>

        </section>

        <aside id="sidebar">
          <a href="https://github.com/ashbell/blogs/zipball/master" class="button">
            <small>Download</small>
            .zip file
          </a>
          <a href="https://github.com/ashbell/blogs/tarball/master" class="button">
            <small>Download</small>
            .tar.gz file
          </a>

          <p class="repo-owner"><a href="https://github.com/ashbell/blogs"></a> is maintained by <a href="https://github.com/ashbell">ashbell</a>.</p>

          <p>感谢<a href="https://github.com/"> Github </a>的免费提供.</p>
        </aside>
      </div>
    </div>

  
  </body>
</html> 
'''
#========Immovable HTML CODE of index_pagecode  END====================

#=================Python start @ index.html files================
# Two files : tree, path. created by shell cmd: tree, find | sort
#================================================================
reg = '[0-9]{4}-[0-9]{2}-[0-9]{2}'  #use to match the xxxx-xx-xx-article_name.html files.
Ltree = []
Lpath = []
idxpath = 0
index_out = './index.html'
Lque_hyperlk = [] 


Fp_path = open('./path', 'r')
lines = Fp_path.readlines()
for i in lines:
	Lpath.append(i)
Fp_path.close()

Fp_OtIndex = open(index_out, 'w')
Fp_OtIndex.write(index_start)

idx = 0
Lque_hyperlk.append('None')

Fp_tree = open('./tree', 'r')
lines = Fp_tree.readlines()
for tl in lines:
	m = re.search(reg, tl) 
	tl = str(tl)
	if m is not None: # We find the article html files' path.
		# The HTML in Browser look like this:
		# │   └──    2014-02-24  >>>> THE_ARTICLE_THEME
		#--tl[:18]---date_format------article_theme-----  
		# OK,we convert it  to HTML code:
		# │   ├── 2014-02-24  >>>> <font color="#0000c6"><a href="ARTICLE_PATH">THEMES</a></font>

		date_format = '%s%s' % (tl[18:28], '  >>>> ')
		filepath = str(Lpath[idxpath]).replace('\n', '') # do not use replace('\n', '\0'), error.
		theme = tl[29:-6]

		HtmlPathCode = '%s%s%s%s%s%s%s' % ( tl[:18], date_format,\
			       '<font color="#0000c6"><a href="', filepath,\
			       '">', theme, '</a></font>' )
		
		idxpath += 1
		Fp_OtIndex.write('%s%s' %(HtmlPathCode, os.linesep))
		idx += 1  # The numbers of article in catalong directory.
		Lque_hyperlk.append(filepath)

	else :  # It's not a article, it is a catalog  directory.
		if cmp('./article/\n', tl) == 0: #Oh, fuck, forget the '\n'
						 #cmp('./article/, tl) never equal.
			DirPathCode = tl.replace('\n','')
		else:
			# ├── c.python_language
			# We change the color of catalog directory.
			# ├──    <font color="#0000cd", style="font-size:100%">c.python_language</font>
			# -tl[:10]-------HTML color and style code-------------Dir_name---------End---- 

			dir_name = str(tl[10:]).replace('\n', '')
			DirPathCode = '%s%s%s%s' % (tl[:10], '<font color="#0000cd", style="font-size:100%">', \
					dir_name, '</font>')
		Fp_OtIndex.write('%s%s' % (DirPathCode, os.linesep)) #Fuck the os.linesep.

		# Okay, Now the ARTICLE_PATH AND CATALOG_DIR_PATH is done. 
		# Process the Prevage and Next_page. Auto insert file path in artciles.
		if idx > 0: # It's no empty directory.
			Lque_hyperlk.append('None') # for [0] [1] `[2]
			for i in range(1, idx + 1):
				f   = open(Lque_hyperlk[i], "r")
				fp  = open('outfile', "w")
				frd = f.readlines()
				t   = 0
				print '%s%s%s' % ("----[", Lque_hyperlk[i],"]" )
				time.sleep(0.5)
				print '%s%s%s'   % ("\tPrev<\t", Lque_hyperlk[i-1], "\t>")
				print '%s%s%s%s' % ("\tNext<\t", Lque_hyperlk[i+1], "\t>", "[Ok]".rjust(10,'-'))
				time.sleep(0.5)
				for text in frd:
					text = str(text)
					# Search the prevage and next_page in HTML files.
					if re.search("a href=.* class=\"button\"", text):
						t += 1
						if t == 1: # Prevage.
						# God Damn it! absolute path is wrong. In a dir named 'a'
						# The next page is ./File, not ./a/File, so we need to add
					        # '../' to the start postion in absolut path.  In dir A:
						# ../A == ./
							tmp = '%s%s%s%s' % ("<a href=\"../.", Lque_hyperlk[i-1],
									   "\"", "  class=\"button\">")
					 	if t == 2: # Next page.
							tmp = '%s%s%s%s' % ("<a href=\"../.", Lque_hyperlk[i+1],
									   "\"", "  class=\"button\">")
					else:
						tmp = text.replace('\n', '')
					fp.write('%s%s' % (tmp, os.linesep))  
				f.close()
				fp.close()
			        print '%s%s%20s' % ("\tDelete: ", Lque_hyperlk[i],"Update finished.")
				os.unlink(Lque_hyperlk[i])
				os.rename('outfile', Lque_hyperlk[i])
		Lque_hyperlk = [] 
		Lque_hyperlk.append('None') # for `[0] [1] [2]
		idx = 0

Fp_tree.close()
Fp_OtIndex.write(index_end)
Fp_OtIndex.close()

# ok, finish the index page !











