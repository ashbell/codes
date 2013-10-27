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
** readstud.c
**
**	Creates a new process to assume the student's identity and read
**	his files.  
*/

static char sccs_id_distr[] = "@(#)readstud.c	1.3      10/15/97";

#include <signal.h>
#include <sys/stat.h>
#include "try.h"
#include "data.h"

FILE	*pipein;
FILE	*pipeout;

int
read_student_files( char **namep )
{
	reg	int	i;
	reg	int	len;
	reg	int	errors;
	reg	FILE	*f;
	int	mode;
	char	buf[BUFSIZ];
	struct	stat	statb;
	extern	int	errno;
	extern	char	*sys_errlist[];

	if( (i = make_pipe_and_child()) != 0 )
		return i;	/* parent returns with pid */

	/*
	** The parent returned above; only the child executes below here.
	** We now change to the user's identity to access his files.  If
	** the parent is interrupted, we'll get a SIGPIPE; arrange to exit
	** without a message in this case, as the parent will already have
	** reported the error.  Catch all other signals similarly.
	*/
	become_stud();
	for( i = SIGHUP; i <= SIGTERM; i++ )
		signal( i, exit_child );

	/*
	** Copy each named file through the pipe to the parent.  Protocol:
	** send name as a newline-terminated string, send the mode that
	** the copy should be created with, then send the size (both as
	** binary integers), then send "size" bytes.  Repeat for each file;
	** when done, exit with number of errors as the exit code.
	*/
	for( errors = 0; *namep != NULL; namep++ ){
		if( (f = fopen( *namep, "r" )) == NULL ){
			error( ERR_USER + 0, "Cannot open \"%s\": %s.\n",
				*namep, sys_errlist[errno] );
			errors++;
		} else if( stat( *namep, &statb ) < 0 ){
			sys_error( ERR_USER + 0, *namep, NULL );
			errors++;
		}

		if( errors ){
			fclose( f );
			continue;
		}

		fprintf( pipeout, "%s\n", last_path_component( *namep ) );
		mode = statb.st_mode & 0700;
		mode |= ( mode >> 3 ) | ( mode >> 6 );
		fwrite( (char *)&mode, sizeof( int ), 1, pipeout );
		fwrite( (char *)&statb.st_size, sizeof( off_t ), 1, pipeout );
		
		for( i = statb.st_size; i > 0; i -= len ){
			if( (len = fread( buf, 1, BUFSIZ, f )) == 0 )
				error( ERR_USER + 1, "Premature EOF on \"%s\".\n", *namep );
				/*NOTREACHED*/

			fwrite( buf, 1, len, pipeout );
		}

		fclose( f );
	}
	fclose( pipeout );

	exit( errors );
	/*NOTREACHED*/
}

int
make_pipe_and_child()
{
	reg	int	i;

	if( pipe( pipedes ) < 0 )
		sys_error( ERR_IMPOS + 1, "readstud", "pipe" );
		/*NOTREACHED*/

	if( (i = fork()) == -1 )
		sys_error( ERR_IMPOS + 1, "readstud", "fork" );
		/*NOTREACHED*/

	/*
	** Close off unneeded ends of the pipe: child will never read it,
	** parent will never write it; open FILE streams on the ones we use.
	*/
	if( i == 0 ){
		logging = FALSE;
		close( pipedes[0] );
		if( (pipeout = fdopen( pipedes[1], "w" )) == NULL )
			error( ERR_IMPOS + 1, "Cannot fdopen %d,w\n", pipedes[1] );
			/*NOTREACHED*/
	} else {
		close( pipedes[1] );
		if( (pipein = fdopen( pipedes[0], "r" )) == NULL )
			error( ERR_IMPOS + 1, "Cannot fdopen %d,r\n", pipedes[0] );
			/*NOTREACHED*/
	}

	return i;
}

void
exit_child( int sig_number )
{
	exit( 1 );
}
