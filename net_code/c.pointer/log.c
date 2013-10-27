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
** log.c
**
**	Add an entry to the log file.  It is assumed that each run of the
**	program will add only one entry to this file; hence it is opened
**	and closed each time.
*/

static char sccs_id_distr[] = "@(#)log.c	1.9      05/04/99";

#include "try.h"
#include <string.h>
#ifndef SYS_TIME
#	include <fcntl.h>
#endif
#ifdef	UNISTD
#	include <unistd.h>
#endif
#include <sys/file.h>
#include <sys/signal.h>
#include "data.h"

bool	yyyy = FALSE;
static	char	log_format_yy[] = "%s\t%s\t%02d/%02d/%02d    %02d:%02d:%02d\t";
static	char	log_format_yyyy[] = "%s\t%s\t%04d/%02d/%02d  %02d:%02d:%02d\t";
static	char	created_fmt[] = "Log file created for %s\n";

/*VARARGS*/
void
log( char *fmt, ... )
{
	va_list	args;

	va_start( args, fmt );
	log0( fmt, args );
	va_end( args );
}

void
log0( char *fmt, va_list args )
{
	reg	FILE	*f;
	reg	int	i;
	reg	struct	tm	*tm;
	reg	int	log_mode;
	int	year;
	char	*log_format;
	extern	time_t	try_time;

	save_id();
	effective_inst();

	log_mode = file_mode( logfile );

	(void)chmod( logfile, 0600 );
	if( (i = open( logfile, O_WRONLY | O_APPEND | O_CREAT, 0600 )) < 0 ){
		create_intermediate_directories( logfile, 0700 );
		if( (i = open( logfile, O_WRONLY | O_APPEND | O_CREAT, 0600 )) < 0 ){
			logging = FALSE;
			sys_error( 1, logfile, "open" );
			/*NOTREACHED*/
		}
	}
	f = fdopen( i, "w" );

	if( log_mode == NO_FILE ){
		log_mode = 0400;
#ifndef	SETREUID
		if( chown( logfile, inst_uid, inst_gid ) < 0 )
			sys_error( ERR_IMPOS, logfile, "chown" );
			/*NOTREACHED*/
#endif

		/*
		** Guarantee that the "Log file created" message is first
		** after a sort by time by subtracting one second from
		** startup time before generating message.
		*/
		try_time--;
		log( created_fmt, stu_name );
		try_time++;
	}

	tm = localtime( &try_time );
	year = tm->tm_year;
	if( yyyy ){
		year += 1900;
		log_format = log_format_yyyy;
	}
	else {
		year %= 100;
		log_format = log_format_yy;
	}
	fprintf( f, log_format, stu_acct,
	    fmt == created_fmt ? "!" : project_code,
	    year, tm->tm_mon + 1, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec );
	vfprintf( f, fmt, args );
	fclose( f );
	if( log_mode != 0600 )
		(void)chmod( logfile, log_mode );
	effective_prev();
}

/*
**	Catch signals.  Our reaction to a signal depends on whether they're
**	being ignored (by the user's request), being ignored (as a result
**	of where we currently are in execution), or not being ignored.
*/
void
signal_catcher( int sig )
{
	extern	char	*signal_names[];
	extern	int	last_signal;
	extern	int	died_by_signal;

	if( sig == SIGHUP || sig == SIGINT || sig == SIGQUIT || sig == SIGTERM ){
		if( ignore_signals || post_signals ){
			if( post_signals )
				post_signal( sig );
			signal( sig, signal_catcher );
			return;
		}
	}

	died_by_signal = TRUE;
	if( sig >= 1 && sig <= last_signal )
		error( ERR_USER + 1, "Interrupted: %s.\n", signal_names[sig] );
	else
		error( ERR_USER + 1, "Interrupted: signal %d.\n", sig );
	/*NOTREACHED*/
}

/*
**	Add an environment variable saying which signal we received.
**	This is only done fir the first signal (subsequent adds are
**	ineffecive).
**
**	[N.B. There used to be no way to add an environment variable
**	if there was already one in the environment; now there is, but
**	I left this code the way it was to remain compatible with
**	its previous behavior.]
*/
void
post_signal( int sig )
{
	char	buffer[64];

	sprintf( buffer, "SIGNAL=%d", sig );
	set_new_env( buffer, FALSE );
}

/*
**	Create the directories leading up to a file or directory that
**	could not be accessed.
*/
void
create_intermediate_directories( char *pathname, int mode )
{
	reg	char	*slash;

	slash = strrchr( pathname, '/' );
	if( slash == NULL )
		return;

	*slash = '\0';
	if( access( pathname, F_OK ) < 0 ){
		create_intermediate_directories( pathname, mode );
		if( mkdir( pathname, mode ) < 0 ){
			sys_error( 1, pathname, "mkdir" );
			/*NOTREACHED*/
		}
	}
	*slash = '/';
}
