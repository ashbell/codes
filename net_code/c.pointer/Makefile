###############################################################################
#									      #
#    Copyright (c) 1989 by Kenneth A. Reek.  Permission to copy or use this   #
#    software without fee is granted to non-profit and not-for-profit	      #
#    institutions and organizations provided that:			      #
#    1) such copying or use is not for commercial advantage, and	      #
#    2) no portion of this software is incorporated into any other program    #
#       or device for commercial advantage, and				      #
#    3) this copyright notice remains with the software.		      #
#    Permission is also granted to make this software available to other      #
#    non-profit or not-for-profit institutions and organizations whose use    #
#    satisfies (1) through (3) above.  Any use of this software for	      #
#    commercial advantage, or incorporation of any part of this software      #
#    into any other program or device for commercial advantage, or any use    #
#    or copying other than by non-profit or not-for-profit institutions or    #
#    organizations requires written permission of the copyright holder.       #
#									      #
###############################################################################

#
# TRY Makefile		%W%	%G%
#

#
# Architecture dependent flags:
#
#	Set CC and CFLAGS in the make script.  If you have a Sun with
#	dynamic linking, be sure to include -Bstatic .
#

OBJS =		alloc.o build.o check.o child.o data.o dumplog.o env.o \
		error.o exec.o housekeeping.o id.o log.o misc.o \
		news.o passwd.o query.o readstud.o run.o running.o \
		signal_names.o studir.o try.o tryrc.o writestud.o xlate.o

MISC =		misc.o signal_names.o

try:		verify_make_script $(OBJS)
		$(CC) $(CFLAGS) -o try $(OBJS)

try_log:	try_log.o $(MISC)
		$(CC) $(CFLAGS) -o try_log try_log.o $(MISC)

try_run:	try_run.o $(MISC)
		$(CC) $(CFLAGS) -o try_run try_run.o $(MISC)

try_deblank:	try_deblank.o
		$(CC) $(CFLAGS) -o try_deblank try_deblank.o
		
all:		try try_run try_log try_deblank

verify_make_script:
		@if [ "$(ARCH_MAKE_SCRIPT)" != Y ] ;\
		then \
			echo Error: not invoked from make script! ;\
			exit 1 ;\
		fi

try.ps:		try.me
		com try.me
		mv ,try.ps try.ps

try.pdf:	try.ps
		echo Making PDF file -- this takes a while
		ps2pdf try.ps

try_lint:
		lint `echo $(OBJS) | sed 's/\.o/.c/g'`

get:
		sccs get SCCS

edit:
		sccs edit SCCS

delta:
		sccs delta SCCS

clean:
		rm -f try try_log try_deblank try_run *.o Arch.* ,tar tar.Z \
			CONTENTS tmac.manual try.pdf try.ps

SRC =		try.h data.h $(OBJS:.o=.c)
AUX_SRC =	try_log.c try_run.c try_deblank.c
USERS_GUIDE =	try.me try.ps tmac.manual try.pdf
MANUAL =	try_deblank.1r try_log.1r try_run.1r try.1r try-s.1r
OTHER =		README Makefile make
FTP_FILES =	$(SRC) $(AUX_SRC) $(USERS_GUIDE) $(MANUAL) $(OTHER)

CONTENTS:	$(FTP_FILES)
		ls -l $(FTP_FILES) > CONTENTS

tmac.manual:
		cp $$HOME/lib/tmac.manual .

,tar:		$(FTP_FILES) CONTENTS
		tar cfv ,tar CONTENTS $(FTP_FILES)

try.tar.Z:	,tar
		compress ,tar
		mv ,tar.Z try.tar.Z

try.zip:	CONTENTS $(FTP_FILES)
		zip try.zip CONTENTS $(FTP_FILES)

ftp:		try.tar.Z try.zip
