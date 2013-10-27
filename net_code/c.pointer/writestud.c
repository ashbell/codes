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
** writestud.c
**
**	Write copies of the student's files in the working (current)
**	directory.
*/

static char sccs_id_distr[] = "@(#)writestud.c	1.4      10/15/97";

#include <sys/types.h>
#include "try.h"
#include "data.h"

char **
write_copies_of_student_files( int reader_pid )
{
	char	buf[BUFSIZ];
	int	size;
	reg	int	i;
	reg	FILE	*f;
	reg	char	**names;
	reg	int	n_names;
	reg	int	max_names;
	extern	FILE	*pipein;

	max_names = 49;
	names = MALLOC( 50, char * );
	names[0] = "";
	n_names = 1;

	effective_try();
	while( fgets( buf, BUFSIZ, pipein ) != NULL ){
		stripnl( buf );
		if( file_mode( buf ) != NO_FILE )
			error( ERR_USER + 1, "Two (or more) files \"%s\" were given; they will overwrite each other.\n", buf );
			/*NOTREACHED*/

		if( (f = fopen( buf, "w" )) == NULL )
			sys_error( ERR_IMPOS + 1, buf, "fopen" );
			/*NOTREACHED*/

		if( fread( (char *)&size, sizeof( int ), 1, pipein ) != 1 )
			error( ERR_IMPOS + 1, "Cannot read mode of \"%s\".\n", buf );
			/*NOTREACHED*/
		
#ifndef	SETREUID
		if( chown( buf, inst_uid, inst_gid ) < 0 )
			sys_error( ERR_IMPOS + 1, buf, "chown" );
			/*NOTREACHED*/
#endif

		if( chmod( buf, size ) < 0 )
			sys_error( ERR_IMPOS + 1, buf, "chmod" );
			/*NOTREACHED*/

		if( fread( (char *)&size, sizeof( off_t ), 1, pipein ) != 1 )
			error( ERR_IMPOS + 1, "Cannot read size of \"%s\".\n", buf );
			/*NOTREACHED*/

		if( n_names >= max_names ){
			max_names += 50;
			names = (char **)realloc( (char *)names, (unsigned)max_names * sizeof( char *) );
		}
		names[n_names++] = strsave( buf );

		while( size > 0 ){
			i = BUFSIZ <= size ? BUFSIZ : size;
			if( fread( buf, 1, i, pipein ) != i )
				error( ERR_IMPOS + 1, "Pipe read too short.\n" );
				/*NOTREACHED*/
			fwrite( buf, 1, i, f );
			size -= i;
		}

		fclose( f );
	}
	fclose( pipein );
	names[n_names] = NULL;
	effective_root();

	while( (i = wait( &size )) != -1 ){
		if( i != reader_pid )
			continue;
		/*
		** See how it terminated.  If normal termination and zero
		** exit code, it was successful.  If normal termination and
		** nonzero exit code, don't print a message (as one was
		** already printed).  If abnormal termination, print a
		** message.
		*/
		if( ( size & 0xff ) == 0 )
			if( ( size & 0xff00 ) == 0 )
				return names;
			else
				quit( 1 );
				/*NOTREACHED*/
		else
			error( ERR_USER + 1, "Fatal error while reading your files.\n" );
		/*NOTREACHED*/
	}
	/*NOTREACHED*/
}
