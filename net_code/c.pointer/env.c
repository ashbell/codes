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
** env.c
**
**	Diddle with the environment.  For one thing, we will fix the
**	PATH variable that we inheret from the student to remove
**	non-standard directories from it, and change the HOME variable
**	to be the instructor's home directory.  Save the student's home
**	directory in the STUDENT_HOME variable.
**
**	We will also add a variable called PROJ_DIR which will be the
**	name of the directory in which the test scripts and test files
**	reside.  This will simplify the sharing of scripts between
**	different projects.
**
**	In general, build a new environment from
**	a) variables set in the .tryrc file (done before this func is called),
**	b) variables that must be given a new value, then
**	c) variables from the student's environment.
*/

static char sccs_id_distr[] = "@(#)env.c	1.9      03/06/00";

#include <string.h>
#include "try.h"
#include "data.h"

char	shell_env[]	= "SHELL=";
char	home_env[]	= "HOME=";
char	path_env[]	= "PATH=";
char	proj_env[]	= "PROJ=";
char	inst_env[]	= "INST=";
char	stu_env[]	= "STUDENT=";
char	proj_dir_env[]	= "PROJ_DIR=";
char	stu_home_env[]	= "STUDENT_HOME=";
char	bkgnd_env[]	= "BACKGROUND=yes";
char	args_env[]	= "FILES=";
char	newpath[]	= NEWPATH;
char	starttime[]	= "TRY_STARTTIME=";
char	backpath_env[]	= "WD_BACKPATH=";
char	script_env[]	= "SCRIPT_DIR=";
char	wd_env[]	= "WD_DIR=";
char	log_env[]	= "LOG_DIR=";

char	**new_env	= NULL;
int	max_new_env	= 0;
int	n_new_env	= 0;

void
build_default_entry( char *env, char *value, char *def_value )
{
	if( value == NULL || *value == '\0' )
		build_new_env_entry( env, def_value );
	else
		build_new_env_entry( env, value );
}

void
fix_environment( char *newhome, char **user_args )
{
	reg	char	**ep;
	struct	tm	*tm;
	char	buf[10*BUFSIZ];		/* for list of user's file names; I hope it's big enough */
	extern	time_t	try_time;

	/*
	** Add brand-new environment variables (these override anything
	** already in the student's environment, but not values that
	** have already come in from the .tryrc file).
	*/
	tm = localtime( &try_time );
	sprintf( buf, "%02d/%02d/%02d %02d:%02d:%02d",
		tm->tm_year % 100, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec );
	build_new_env_entry( starttime, buf );
	build_new_env_entry( home_env, newhome );
	build_new_env_entry( inst_env, inst_acct );
	build_new_env_entry( proj_env, project_code );
	build_new_env_entry( stu_env, stu_acct );
	build_new_env_entry( proj_dir_env, project_dir );
	build_default_entry( backpath_env, wd_backpath, DEFAULT_BACKPATH0 );
	build_default_entry( script_env, script_dir, "." );
	build_default_entry( wd_env, wd_dir, "." );
	build_default_entry( log_env, log_dir, "." );
	if( background_mode )
		set_new_env( bkgnd_env, FALSE );
	if( user_args != NULL ){
		strcpy( buf, args_env );
		while( *++user_args != NULL ){
			strcat( buf, *user_args );
			strcat( buf, " " );
		}
		set_new_env( buf, FALSE );
	} else
		set_new_env( args_env, FALSE );


	/*
	** Copy existing environment to new one, changing what needs to be
	** changed and deleting those that should be deleted.  Note that
	** the user's env variables set thru .tryrc have already been added;
	** any duplicate entries here will be ignored.
	*/
	for( ep = environ; *ep != NULL; ep++ ){
		if( nstreq( *ep, path_env, 5 ) )
			build_new_env_entry( path_env, newpath );

		else if( nstreq( *ep, shell_env, 6 ) )
			build_new_env_entry( shell_env, "/bin/echo" );

		else if( nstreq( *ep, home_env, 5 ) )
			build_new_env_entry( stu_home_env, *ep + 5 );

		else if( nstreq( *ep, "TRY_", 4 ) )
			;	/* delete this entry */

		else if( nstreq( *ep, bkgnd_env, 11 ) )
			;	/* delete this entry */

		else if( nstreq( *ep, args_env, 6 ) )
			;	/* delete this entry */

		else
			set_new_env( *ep, FALSE );
	}

	/*
	** Install the new environment.
	*/
	environ = new_env;
}

/*
**	Build a new env entry from a name and a value argument.
*/
void
build_new_env_entry( char *name, char *value )
{
	char	env_buf[BUFSIZ];

	strcpy( env_buf, name );
	strcat( env_buf, value );
	set_new_env( env_buf, FALSE );
}

/*
**	Add an entry to the new environment (after first making sure that
**	it is not already there).  We always make sure that the list of
**	pointers is terminated with a NULL pointer.
**
**	If 'overwrite' is TRUE, then this entry will replace an existing
**	entry with the same name.
*/
void
set_new_env( char *value, bool overwrite )
{
	reg	char	**ep;
	reg	int	i;
	reg	int	len;

	if( new_env != NULL ){
		len = strchr( value, '=' ) - value + 1;
		for( i = 0, ep = new_env; i < n_new_env; i++ )
			if( nstreq( value, *ep++, len ) ){
				if( overwrite ){
					free( *--ep );
					*ep = strsave( value );
				}
				return;
			}
	}

	if( n_new_env >= max_new_env ){
		max_new_env += 50;
		/*
		** The +1 below reserves room for the trailing NULL.
		*/
		ep = MALLOC( max_new_env + 1, char * );
		if( new_env != NULL ){
			for( i = 0; i < n_new_env; i++ )
				ep[i] = new_env[i];
			free( (char *)new_env );
			if( environ == new_env )
				environ = ep;
		}
		new_env = ep;
	}
	new_env[n_new_env++] = strsave( value );
	new_env[n_new_env] = NULL;
}

/*
**	If the init, build, or run script created a file called .ENVIRONMENT,
**	read the variable definitions contained therein and add them
**	to the environment, too.  The file is then deleted.
*/
#define	EBUFSIZE	512

void
add_new_environment_vars()
{
	reg	FILE	*f;
	char	buf[EBUFSIZE];
	reg	char	*cp;
	static	char	env_file[] = ".ENVIRONMENT";

	f = fopen( env_file, "r" );
	if( f == NULL )
		return;

	while( fgets( buf, EBUFSIZE, f ) != NULL ){
		/*
		** See if it is a valid definition; if not, skip it.
		*/
		cp = strchr( buf, '=' );
		if( cp == NULL )
			continue;

		/*
		** Remove the trailing newline
		*/
		cp = strchr( buf, '\n' );
		if( cp != NULL )
			*cp = NUL;

		/*
		** Add to the environment!
		*/
		set_new_env( buf, TRUE );
	}
	fclose( f );
	unlink( env_file );
}
