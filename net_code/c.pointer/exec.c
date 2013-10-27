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
** exec.c
**
**	Execute a new program.  The reason this is a separate routine
** 	is so that the signal handling can be done in one centralized
**	location.
*/

static char sccs_id_distr[] = "@(#)exec.c	1.3      10/15/97";

#include <signal.h>
#include <errno.h>
#include "try.h"
#include "data.h"

void
exec( char *path, char **args, char **env )
{
	extern	int	errno;

	if( ignore_signals ){
		signal( SIGHUP, SIG_IGN );
		signal( SIGINT, SIG_IGN );
		signal( SIGQUIT, SIG_IGN );
		signal( SIGTERM, SIG_IGN );
	}
	execve( path, args, env );
	if( errno == ENOEXEC )
		exec_script( path, args, env );
	/*
	** It is an error for the execve to return, but we'll let the
	** caller handle it.
	*/
}

/*
**	System 5 won't execute scripts with an exec like BSD will,
**	so we have to do it ourselves.  We should really get the shell
**	path from the environment, or read it from the script itself.
*/
void
exec_script( char *path, char **args, char **env )
{
	reg	int	i;
	reg	char	*string;
	reg	int	len;
	char	*sh_args[4];

	sh_args[0] = "/bin/sh";
	sh_args[1] = "-c";
	sh_args[3] = NULL;

	len = strlen( path );
	for( i = 1; args[i]; i++ )
		len += 1 + strlen( args[i] );
	sh_args[2] = string = MALLOC( len, char );
	strcpy( string, path );
	while( *++args ){
		strcat( string, " " );
		strcat( string, *args );
	}
	execve( "/bin/sh", sh_args, env );
}
