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
** build.c
**
**	Call the instructor's script to build the student's work.  The
**	umask is modified to prevent temporary files created by compilers
**	and such from being accessible to other students.  This is ok
**	because after the build is done, the modes of all files in the
**	scratch directory are changed explicitly.  The pipe is used by
**	try_log to send messages back to the log file.
*/

static char sccs_id_distr[] = "@(#)build.c	1.7      10/15/97";

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "try.h"
#include "data.h"

/*
**	Names for use in log file
*/
char	build_name[] = "build";
char	build_safe_name[] = "build_safe";

bool
build_student_work( char *build_cmd, char *build_safe_cmd, char **args )
{
	int	status;
	char	logmsg[BUFSIZ];
	reg	int	i;
	reg	int	used_build;
	extern	int	ignore_signals;

	/*
	** Create try_log message pipe
	*/
	if( pipe( pipedes ) < 0 )
		sys_error( ERR_IMPOS + 1, "build", "pipe" );
		/*NOTREACHED*/

	/*
	** Select script to use, and call it
	*/
	effective_inst();
	umask( 077 );
	if( (i = file_mode( build_safe_cmd )) != NO_FILE && ( i & 0500 ) == 0500 ){
		if( ( i & 05 ) != 05 ){
			i |= 0555;
			(void)chmod( build_safe_cmd, i );
		}
		used_build = FALSE;
		child_proc( become_try, do_build_run_script,
			build_safe_cmd, args );

	} else if( (i = file_mode( build_cmd )) != NO_FILE && ( i & 0500 ) == 0500 ){
		used_build = TRUE;
		child_proc( become_inst, do_build_run_script,
			build_cmd, args );

	} else {
		error( 1, "No build or build_safe script!\n" );
		/*NOTREACHED*/
	}
	umask( 0 );

	close( pipedes[1] );
	if( (i = read( pipedes[0], logmsg, BUFSIZ )) > 0 )
                logmsg[i] = '\0';
        close( pipedes[0] );

	(void)wait( &status );

	/*
	** Interpret exit status, log results.
	*/
	if( ( status & 0xff ) != 0 )
		error( 1, "%s script aborted!\n",
			used_build ? build_cmd : build_safe_cmd );
		/*NOTREACHED*/

	status = ( status >> 8 ) & 0xff;

        if( status != 0 ){
		/*
		** The build failed: ignore interrupts so that the
		** logging and cleanup script can't be interrupted.
		*/
		ignore_signals = TRUE;

                if( status == 128 ){
                        error( 1, "%s script could not be executed.\n",
                                used_build ? build_cmd : build_safe_cmd );
			/*NOTREACHED*/
                }
                if( i > 0 )
                        log( "Failed %s: %s\n",
			    used_build ? build_name : build_safe_name, logmsg );
                else
                        log( "Failed %s.\n",
			    used_build ? build_name : build_safe_name );

		{
			reg	char	*build_fmt;

			if( (build_fmt = getenv( "TRY_FAIL_BUILD" )) == NULL )
				build_fmt = "Couldn't build your project.";
			if( *build_fmt ){
				printf( build_fmt, project_code );
				putchar( NL );
			}
		}

        } else
		change_file_modes( used_build );

	return status == 0;
}

/*
**	Set up to run the instructor's build (or build_safe) script.
*/
void
do_build_run_script( reg char *cmd, reg char **args )
{
        extern  int     errno;

        /*
        ** Arrange for the logging pipe have file descriptor 3.
        */
        close( pipedes[0] );
        if( pipedes[1] != 3 ){
		close( 3 );
                dup( pipedes[1] );
                close( pipedes[1] );
        }

	/*
	** Execute the instructor's build or run script. 
	*/
	args[0] = cmd;
	exec( cmd, args, environ );
	perror( cmd );
	exit( 128 );
}

/*
**	After doing a build_safe, we will have created files owned by a junk
**	account, with a junk group.  These files may not have public read
**	and execute privelege; we must therefore change their modes to
**	allow the instructor to access them.
**
**	After a build, the instructor will own any files created; but they
**	might need to be accessed from the run_safe script, so their modes
**	must also be changed to allow public access.
**
**	While we're at it, create a file containing the name of the
**	most recently created file in the directory, presumably the
**	output of the build or build_safe script.
**
**	KAR 5/9/94: Added code to recurse down through subdirectories.
*/
void
change_file_modes( bool used_build )
{
	reg	FILE	*f;
	char	newest_name[BUFSIZ];
	time_t	newest_time;

	if( ! used_build )
		effective_try();

	newest_time = change_modes( newest_name );


	/*
	** After changing all of the modes, revert to the instructor to
	** create the "newest_file" file.
	*/
	if( ! used_build )
		effective_inst();

	if( newest_time > 0 ){
		if( (f = fopen( ",newest_file", "w" )) != NULL ){
			fprintf( f, "%s\n", newest_name );
			fclose( f );
		}
		(void)chmod( ",newest_file", 0666 );
#ifndef	SETREUID
		(void)chown( ",newest_file", inst_uid, inst_gid );
#endif
	}
}

time_t
change_modes( char *newest_name )
{
	reg	DIR	*dirp;
	struct	dirent	*dp;
	struct	stat	statb;
	reg	int	owner_perm;
	reg	int	mode;
	time_t	newest_time;

	if( (dirp = opendir( "." )) == NULL )
		sys_error( ERR_IMPOS + 1, ".", "opendir" );
		/*NOTREACHED*/

	newest_time = 0;

	while( (dp = readdir( dirp )) != NULL ){
		if( streq( dp->d_name, "." ) || streq( dp->d_name, ".." ) )
			continue;

		/*
		** Must do a stat here, as we need the modification time too.
		*/
		if( stat( dp->d_name, &statb ) < 0 )
			continue;

		mode = statb.st_mode & 0777;
		owner_perm = ( mode & 0700 ) >> 6;
		if( ( mode & 07 ) != owner_perm ||
		    ( ( mode & 070 ) >> 3 ) != owner_perm ){
			mode |= owner_perm | ( owner_perm << 3 );
			(void)chmod( dp->d_name, mode );
		}

		/*
		** If this is a directory, enter it and recurse.  If not
		** a directory, then see if it is the newest file.
		*/
		if( statb.st_mode & S_IFDIR && chdir( dp->d_name ) == 0 ){
			(void)change_modes( NULL );
			chdir( ".." );
		}
		else if( newest_name != NULL && statb.st_mtime > newest_time ){
			newest_time = statb.st_mtime;
			strcpy( newest_name, dp->d_name );
		}
	}
	closedir( dirp );
	return newest_time;
}
