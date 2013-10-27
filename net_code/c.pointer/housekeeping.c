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
** housekeeping.c
**
**	Call the instructor's housekeeping scripts.  The argument will be
**	either a 0 or a 1; we must pass a character "0" or "1" thru exec.
*/

static char sccs_id_distr[] = "@(#)housekeeping.c	1.4      10/15/97";

#include <errno.h>
#include "try.h"
#include "data.h"

void
housekeeping( char *cmd, char **args, bool check_status, char *which_script )
{
	int	status;
	reg	int	i;
	char	logmsg[BUFSIZ];

	/*
	** See if the script exists.
	*/
	effective_inst();

	if( (i = file_mode( cmd )) == NO_FILE || ( i & 0500 ) != 0500 )
		return;

	/*
	** Create try_log message pipe
	*/
	if( pipe( pipedes ) < 0 )
		sys_error( ERR_IMPOS + 1, cmd, "pipe" );
		/*NOTREACHED*/

	child_proc( become_inst, do_build_run_script, cmd, args );

	close( pipedes[1] );
	if( (i = read( pipedes[0], logmsg, BUFSIZ )) > 0 )
		logmsg[i] = '\0';
	close( pipedes[0] );

	(void)wait( &status );

	if( ( status & 0xff ) != 0 ){
		error( 1, "%s: script aborted!\n", cmd );
		/*NOTREACHED*/
	}

	status = ( status >> 8 ) & 0xff;

	if( status != 0 ){
		if( status == 128 ){
			error( 1, "%s script could not be executed.\n",
			    cmd );
			/*NOTREACHED*/
		}
		if( !check_status )
			return;

		if( i > 0 )
			log( "Failed %s: %s\n", which_script, logmsg );
		else
			log( "Failed %s.\n", which_script );

		quit( 0 );
	}
}
