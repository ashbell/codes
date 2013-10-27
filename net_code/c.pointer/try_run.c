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
** try_run.c
**
**	This program runs a student program in a controlled way.  The
**	caller can limit any of the parameters that Unix allows.
**
**	If an error occurs, a description is written to the standard
**	output, and an appropriate error status is returned.  If the
**	program finished without any errors, a zero status is returned.
*/

static char sccs_id_distr[] = "@(#)try_run.c	1.4      05/04/99";

#include "try.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/param.h>
#ifdef	BSD_RLIMIT
#	include <sys/time.h>
#	include <sys/resource.h>
#else
#	include <time.h>
#endif
#include <sys/signal.h>

char	*progname;
char	protect_root	= TRUE;
char	dup_stderr	= TRUE;

extern	char	*sys_errlist[];

typedef struct {
	char	namelen;
	char	*name;
} Name;

typedef	struct {
	int	resource;
	int	value;
	Name	names0;
	Name	names1;
} Limit;
#define	name0		names0.name
#define	namelen0	names0.namelen
#define	name1		names1.name
#define	namelen1	names1.namelen

/*
** Solaris 2.6 has dropped RLIMIT_RSS in favor of RLIMIT_VMEM, which
** seems to be about the same thing.  The define below enables this
** code to work with either version.
*/
#ifndef	RLIMIT_RSS
#	define	RLIMIT_RSS	RLIMIT_VMEM
#endif

Limit	limits[] = {
#ifndef	BSD_RLIMIT
	{ 0,	0,	4, "cpu=", 8, "cputime=" },
	{ 0,	10240,	6, "fsize=", 9, "filesize=" },
	{ 0,	0,	5, "data=", 9, "datasize=" },
	{ 0,	0,	6, "stack=", 10, "stacksize=" },
	{ 0,	0,	5, "core=", 13, "coredumpsize=" },
	{ 0,	0,	4, "rss=", 1, "\001" },
#else
	{ RLIMIT_CPU,	10,	{ 4, "cpu=" }, { 8, "cputime=" } },
	{ RLIMIT_FSIZE,	10240,	{ 6, "fsize=" }, { 9, "filesize=" } },
	{ RLIMIT_DATA,	RLIM_INFINITY,	{ 5, "data=" }, { 9, "datasize=" } },
	{ RLIMIT_STACK,	RLIM_INFINITY,	{ 6, "stack=" }, { 10, "stacksize=" } },
	{ RLIMIT_CORE,	0,	{ 5, "core=" }, { 13, "coredumpsize=" } },
	{ RLIMIT_RSS,	RLIM_INFINITY,	{ 4, "rss=" }, { 1, "\001" } },
#endif
	{ -1 }
};

void
main( ac, av, ep )
int ac;
reg char **av;
char **ep;
{
	char	**tr_do_args();
	reg	int	i;
	int	pipedes[2];

	av = tr_do_args( av );

	/*
	** Close off non-standard files.  Then make a pipe in which exec
	** failure code is passed back.
	*/
	for( i = 3; i < NOFILE; i++ )
		close( i );

	if( pipe( pipedes ) < 0 )
		tr_error( TRYRUN_NO_PIPE, "Can't make pipe\n" );
		/*NOTREACHED*/

	switch( fork() ){
	default:			/* parent */
		setgid( getgid() );
		setuid( getuid() );
		close( pipedes[1] );
		break;

	case 0:				/* child */
		if( protect_root )
			chroot( "." );
		setgid( getgid() );
		setuid( getuid() );
		close( pipedes[0] );
		run_prog( av, ep, pipedes[1] );
		/*NOTREACHED*/

	case -1:			/* fork failed */
		tr_error( TRYRUN_NO_FORK, "Can't fork\n" );
		/*NOTREACHED*/
	}

	exit( wait_for_child( pipedes[0] ) );
}

char **
tr_do_args( av )
reg char **av;
{
	progname = last_path_component( *av++ );

	if( *av == NULL ){
		tr_error( TRYRUN_USAGE, "Usage: %s [ options ] prog [ args ]\n", progname );
		/*NOTREACHED*/
	}

	for( ; *av != NULL; av++ ){
		if( resource_arg( *av ) )
			continue;
		
		if( streq( *av, "-e" ) ){
			dup_stderr = FALSE;
			continue;
		}
		if( streq( *av, "-p" ) ){
			protect_root = FALSE;
			continue;
		}

		break;
	}

	return av;
}


resource_arg( arg )
reg char *arg;
{
	reg	Limit	*lp;
	reg	int	len;
	int	s, m;
	char	scale;

	for( lp = limits; lp->resource != -1; lp++ ){
		if( nstreq( arg, lp->name0, lp->namelen0 ) )
			len = lp->namelen0;
		else if( nstreq( arg, lp->name1, lp->namelen1 ) )
			len = lp->namelen1;
		else
			continue;

		arg += len;

		if( streq( arg, "inf" ) )
#ifndef	BSD_RLIMIT
			lp->value = 0x7fffffff;
#else
			lp->value = RLIM_INFINITY;
#endif

		else if( is_numeric( arg ) )
			lp->value = atoi( arg );

		else if( *arg >= '0' && *arg <= '9' ){
			if( lp == limits && sscanf( arg, "%d:%d", &m, &s ) == 2 )
				lp->value = m * 60 + s;

			else if( sscanf( arg, "%d%c", &m, &scale ) == 2 ){
				switch( scale ){
				case 'h':
					if( lp != limits )
						goto ResArgErr;
					m *= 3600;
					break;

				case 'k':
					if( lp == limits )
						goto ResArgErr;
					m *= 1024;
					break;

				case 'm':
					m *= lp == limits ? 60 : 1024 * 1024;
					break;

				default:
					goto ResArgErr;
				}
				lp->value = m;
			}

		} else
ResArgErr:		tr_error( TRYRUN_USAGE, "Bad resource limit value: %s\n",
				arg - len );
			/*NOTREACHED*/

		return TRUE;
	}

	return FALSE;
}

run_prog( av, ep, efd )
reg char **av;
char **ep;
reg int efd;
{
	reg	int	old_stderr;
	extern	int	errno;

	/*
	** Set exec-error fd to close on successful exec.
	*/
	fcntl( efd, F_SETFD, 1 );

	/*
	** Save copy of original stderr, set this copy to be closed
	** on exec.  Then dup stdout to stderr (for user's program).
	*/
	if( dup_stderr ){
		fcntl( old_stderr = dup( 2 ), F_SETFD, 1 );
		close( 2 );
		dup( 1 );
	} else
		old_stderr = -1;

	/*
	** Impose the resource limits.
	*/
	set_limits();

	/*
	** Execute the given program with whatever arguments were passed.
	*/
	if( execve( av[0], av, ep ) < 0 ){
		/*
		** Can't exec!  Write errno back to the parent thru the
		** pipe, then write English error message to both stderr's.
		*/
		write( efd, &errno, sizeof( errno ) );
		close( efd );

		do {
			fprintf( stderr, "Cannot execute %s: %s\n",
				av[0], sys_errlist[errno] );
			fflush( stderr );
		} while( close( 2 ), dup( old_stderr ), close( old_stderr ) >= 0 );

		/*
		** We wrote the status back thru the pipe; the value we
		** exit with is not used.
		*/
		exit( 1 );
	}
}

set_limits()
{
#ifndef	BSD_RLIMIT
	ulimit( 2, ( limits[1].value + 511 ) / 512 );
#else
	struct	rlimit	rl;
	reg	Limit	*lp;

	for( lp = limits; lp < &limits[RLIM_NLIMITS]; lp++ ){
		rl.rlim_max = rl.rlim_cur = lp->value;
		setrlimit( lp->resource, &rl );
	}
#endif
}

wait_for_child( efd )
int efd;
{
	int	status;
	reg	int	user_code;
	reg	int	coremsg;
	extern	char	*signal_names[];
	extern	int	last_signal;
	reg	FILE	*f;

	/*
	** See if status comes back through the pipe.  If so, the exec
	** must have failed, so add 0x80 to the number.  Otherwise, the
	** exec worked, so wait for the child termination status.
	*/
	if( read( efd, &status, sizeof( status ) ) == sizeof( status ) ){
		status += TRYRUN_NO_EXEC;
		coremsg = 0;
	} else {
		wait( &status );
		coremsg = status & 0x80;
		user_code = ( status >> 8 ) & 0xff;
		status &= 0x7f;
	}

	if( status != 0 ){
		char	*message;
		char	bad_signal[ 50 ];

		/*
		** Get the name of the signal that killed the program.
		*/
		if( status <= last_signal )
			message = signal_names[ status ];
		else {
			sprintf( bad_signal, "unknown signal #%d", status );
			message = bad_signal;
		}

		/*
		** Write the signal message to stdout.  Also write it
		** to stderr if either is not a terminal and the
		** other is.
		*/
		fputs( "\nProgram failed: ", stdout );
		fputs( message, stdout );
		if( coremsg )
			fputs( ", core dumped", stdout );
		fputc( '\n', stdout );

		if( isatty( 1 ) != isatty( 2 ) ){
			fputs( "\nProgram failed: ", stderr );
			fputs( message, stderr );
			if( coremsg )
				fputs( ", core dumped", stderr );
			fputc( '\n', stderr );
		}
	} else {
		if( (f = fopen( ",user.exit.status", "w" )) != NULL ){
			fprintf( f, "%d\n", user_code );
			fclose( f );
		}
	}

	return status;
}

tr_error( code, fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 )
int code;
char *fmt;
int a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
{
	fprintf( stderr, "%s: ", progname );
	fprintf( stderr, fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 );

	if( isatty( 1 ) != isatty( 2 ) ){
		fprintf( stdout, "%s: ", progname );
		fprintf( stdout, fmt, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 );
	}

	exit( code );
}
