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
** try_log.c
**
**	This program reports the reason for a student program's failure
**	back to the "try" program so it can be recorded in the log file.
*/

static char sccs_id_distr[] = "@(#)try_log.c	1.3      05/04/99";

#include <stdio.h>
#include <signal.h>
#include "try.h"

main( ac, av )
int ac;
char **av;
{
	reg	char	*bp;
	reg	int	i;
	reg	FILE	*f;
	char	buf[BUFSIZ];
	extern	char	*sys_errlist[];
	extern	char	*signal_names[];
	extern	int	last_signal;

	if( ac < 2 )
		strcpy( buf, "No reason given." );

	else if( ac > 2 || ! is_numeric( av[1] ) ){
		strcpy( buf, av[1] );
		for( i = 2; i < ac; i++ ){
			strcat( buf, " " );
			strcat( buf, av[i] );
		}
		for( bp = buf; *bp != NUL; bp++ )
			if( *bp == NL )
				*bp = SP;

	} else {
		i = atoi( av[1] );
		switch( i ){
		case 0:
			strcpy( buf, "Code 0: success!" );
			break;

		case TRYRUN_USAGE:
			strcpy( buf, "tryrun usage error" );
			break;

		case TRYRUN_NO_FORK:
			strcpy( buf, "tryrun couldn't fork" );
			break;

		case TRYRUN_NO_PIPE:
			strcpy( buf, "tryrun couldn't create pipe" );
			break;

		default:
			if( i < last_signal )
				strcpy( buf, signal_names[i] );
			else if( i >= TRYRUN_NO_EXEC )
				sprintf( buf, "Exec failed: %s", sys_errlist[i - TRYRUN_NO_EXEC] );
			else
				sprintf( buf, "Unknown signal %d", i );
			break;
		}
	}

	f = fdopen( 3, "w" );
	fputs( buf, f );
	fclose( f );
}
