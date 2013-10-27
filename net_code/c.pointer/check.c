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
** check.c
**
**	Checks mode sees if the files for a project are properly set up;
**	only the instructor can run in this mode.
*/

static char sccs_id_distr[] = "@(#)check.c	1.5      10/15/97";

#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "try.h"
#include "data.h"

#define	REQUIRED	0x1
#define	DIREC		0x2
#define	CHMOD		0x4
#define	OPTIONAL	0x8
#define	REPORT_BOTH	0x10

void
check_dirs()
{
	/*
	** Check to see if the log_dir, wd_dir and script_dir paths exist.
	** These are relative to the try directory.
	*/
	if( wd_dir != NULL )
		(void)check_file( wd_dir, 0700, DIREC | CHMOD | REQUIRED );
	if( log_dir != NULL )
		(void)check_file( log_dir, 0700, DIREC | CHMOD | REQUIRED );
	if( script_dir != NULL )
		(void)check_file( script_dir, 0500, DIREC | CHMOD | REQUIRED );
}

void
check_files()
{
	char	buf[BUFSIZ];

	strcpy( buf, build_cmd );
	*strrchr( buf, '/' ) = NUL;
	if( ! check_file( buf, 0111, REQUIRED | DIREC | CHMOD ) ){
		fprintf( stderr, "\n" );
		return;
	}

	(void)check_file( init_cmd, 0500, CHMOD );
	check_pair( build_safe_cmd, 0555, build_cmd, 0500, 0 );
	check_pair( run_safe_cmd, 0555, run_cmd, 0500, 0 );
	check_pair( explain_cmds[0], 0500, explain_cmds[1], 0500, OPTIONAL );
	check_pair( dump_log_cmds[0], 0500, dump_log_cmds[1], 0500, OPTIONAL );
	(void)check_file( cleanup_cmd, 0500, CHMOD );
	(void)check_file( query_cmd, 0500, CHMOD );
	fprintf( stderr, "\n" );
}

/*
**	Check to see if either of a pair of scripts exists.
*/
void
check_pair( char *first, int first_mode, char *second, int second_mode,
    int flag )
{
	reg	int	first_actual_mode;
	reg	int	second_actual_mode;

	first_actual_mode = file_mode( first );
	second_actual_mode = file_mode( second );

	if( first_actual_mode == NO_FILE ){
		if( second_actual_mode == NO_FILE ){
			error( ERR_USER + 0, "Neither \"%s\" nor \"%s\" are present%s.",
			    first, second,
			    flag & OPTIONAL ? " (they are optional)" : "" );
			return;
		}
		(void)check_file( second, second_mode, CHMOD );
		return;
	}

	if( ( first_actual_mode & first_mode & 0700 ) ==
	    ( first_mode & 0700 ) ){
		if( second_actual_mode != NO_FILE &&
		    ( second_actual_mode & second_mode & 0700 ) ==
		    ( second_mode & 0700 ) ){
			error( ERR_USER + 0, "\"%s\" and \"%s\" both exist, the former will be used.",
				first, second );
		}
		(void)check_file( first, first_mode, CHMOD );
		return;
	}

	if( second_actual_mode != NO_FILE &&
	    ( second_actual_mode & second_mode & 0700 ) ==
	    ( second_mode & 0700 ) ){
		(void)check_file( second, second_mode, CHMOD );
		error( ERR_USER + 0, "\"%s\" has mode %o, so \"%s\" will be used instead.",
			first, first_actual_mode, second );
	} else
		(void)check_file( first, first_mode, CHMOD );
}

/*
**	Check that a particular file has desired mode.  If the CHMOD flag
**	is given (for -c option), change the modes of files that are wrong.
*/
bool
check_file( char *path, int min_mode, int flag )
{
	reg	int	mode;
	extern	int	errno;
	extern	char	*sys_errlist[];

	if( (mode = file_mode( path )) < 0 ){
		if( ! ( flag & REQUIRED ) )
			return FALSE;
		if( errno == ENOENT ){
			error( ERR_USER + 0, "%s \"%s\" missing.",
				flag & DIREC ? "Directory" : "Required file",
				path );
			return FALSE;
		}

		error( ERR_USER + 0, "%s \"%s\" inaccessible: %s.",
			flag & DIREC ? "Directory" : "Required file",
			path, sys_errlist[errno] );
		return FALSE;
	}

	if( ( mode & min_mode ) != min_mode ){
		if( ! ( flag & CHMOD ) )
			return FALSE;

		error( ERR_USER + 0, "\"%s\" exists but has mode %o.  It needs at least mode %o.",
			path, mode, min_mode );

		mode |= min_mode;
		if( chmod( path, mode ) < 0 ){
			sys_error( ERR_USER + 0, path, "chmod" );
			return FALSE;
		}
		error( ERR_USER + 0, "I changed it to %o for you.",
			mode & 0777 );
	}

	return TRUE;
}

void
check_try_dir( char *dir )
{
	(void)check_file( dir, 0111, REQUIRED | DIREC | CHMOD );
	error( ERR_USER + 0, "Looking in directory \"%s\":", dir );
}

/*
**	Look up and return the mode of the given file.  The mode returned
**	DOES NOT include any set-uid or sticky bits -- in general, we
**	don't want those set on our scripts anyway.
*/
int
file_mode( char *path )
{
	struct	stat	statb;

	if( stat( path, &statb ) < 0 )
		return NO_FILE;

	return statb.st_mode & 0777;
}
