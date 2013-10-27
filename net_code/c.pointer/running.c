/******************************************************************************
*									      *
*    Copyright (c) 1989 by Kenneth A. Reek.  Permission to copy or use this   *
*    software without fee is granted to non-profit and not-for-profit	      *
*    institutions and organizations provided that:			      *
*    1) such copying or use is not for commercial advantage, and	      *
*    2) no portion of this software is incorporated into any other program    *
*       or device for commercial advantage, and				      *
*    3) this copyright notice remains with the software.		      *
*    Permission is also granted to make this software available to other      *
*    non-profit or not-for-profit institutions and organizations whose use    *
*    satisfies (1) through (3) above.  Any use of this software for	      *
*    commercial advantage, or incorporation of any part of this software      *
*    into any other program or device for commercial advantage, or any use    *
*    or copying other than by non-profit or not-for-profit institutions or    *
*    organizations requires written permission of the copyright holder.       *
*									      *
******************************************************************************/


/*
** running.c
**
**	Check to see whether there is already a 'try' running.
*/

static char sccs_id_distr[] = "@(#)running.c	1.4      10/15/97";

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "try.h"

static	char	warning_msg[] = "\n\
\n\
WARNING: It appears that you already have a \"try\" running:\n\
process #%s on host %s started %s.\n\
\n\
Running more than one \"try\" at a time is almost guaranteed to\n\
cause errors, even if they are for different projects. \n\
\n\
If you kill this \"try\" now, the other one will continue without\n\
error.  But if you really want this \"try\" to continue, you\n\
should kill the other(s) before allowing this one to continue.\n\
\n\
Either kill this \"try\" now, or press RETURN to continue: ";

void
is_try_already_running()
{
	FILE	*f;
	char	pid[100], hostname[100], date[100];
	extern	char	tryid_file[];

	f = fopen( tryid_file, "r" );
	if( f == NULL )
		return;

	if( fscanf( f, "%[^,]", pid ) != 1 )
		strcpy( pid, "unknown" );
	if( fscanf( f, ",%[^,]", hostname ) != 1 )
		strcpy( hostname, "unknown" );
	if( fscanf( f, ",%[^\n]", date ) != 1 )
		strcpy( date, "unknown" );

	fclose( f );
	fprintf( stderr, warning_msg, pid, hostname, date );
	fgets( pid, 100, stdin );
}
