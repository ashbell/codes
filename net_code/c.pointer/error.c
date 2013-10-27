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
** error.c
**
**	Error handling/printing routines.
*/

static char sccs_id_distr[] = "@(#)error.c	1.5      10/16/97";

#include <errno.h>
#include <signal.h>
#include "try.h"
#include "data.h"

int	died_by_signal;

/*
**	System errors are handled as above, except that errno is used to
**	create a more descriptive message.
*/
void
sys_error( int code, char *name, char *syscall )
{
	extern	char	*sys_errlist[];
	extern	int	errno;

	if( syscall != NULL )
		error( code, "%s( %s ): %s\n", syscall, name, sys_errlist[errno] );
	else
		error( code, "%s: %s\n", name, sys_errlist[errno] );
}

/*
**	Errors are handled in various ways.  The code contains both flags
**	and a code value. 
*/
/*VARARGS*/
void
error( int code, char *fmt, ... )
{
	va_list	args;
	char	buf[BUFSIZ];
	extern	int	parent_pid;

	va_start( args, fmt );

	/*
	**	Log the error if logging is turned on and
	**	we have a nonzero code.
	*/
	if( logging && ( code & ERR_CODE_MASK ) != 0 )
		log0( fmt, args );

	/*
	**	If the error is something that is the user's fault (or the
	**	instructor's during a -c checking run), inform him.  For
	**	any non-user error, inform him if we're not logging so we
	**	don't lose the message.  Otherwise, the instructor can
	**	get the message from the log file, so print a generic
	**	error message for the user.
	*/
	if( (code & ERR_USER) || !logging ){
		vsprintf( buf, fmt, args );
		fprintf( stderr, "\n%s: ", progname );
		if( code & ERR_IMPOS )
			fputs( "(shouldn't happen) ", stderr );
		fputs( buf, stderr );
	} else
		fputs( ERR_SEE_INST_MSG, stderr );

	fflush( stderr );

	/*
	**	Terminate try if the code value was nonzero.
	*/
	code &= ERR_CODE_MASK;
	if( code != 0 ){
		if( died_by_signal && parent_pid == getpid() )
			kill_off_children();
		quit( code );
	}
	va_end( args );
}

/*
**	All processes in the process group are killed unless they have
**	previously arranged to catch or ignore signals.  We must ignore
**	the signal ourselves to avoid suicide.
*/
void
kill_off_children()
{
	signal( SIGINT, SIG_IGN );
	kill( 0, SIGINT );
}
