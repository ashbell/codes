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
** dumplog.c
**
**	Report the student's log file back to them.
*/

static char sccs_id_distr[] = "@(#)dumplog.c	1.5      10/15/97";

#include <errno.h>
#include "try.h"
#include "data.h"

void
dump_log( char *logfile, char *dump_log_cmds[2] )
{
	child_proc( become_inst, run_dump_script, dump_log_cmds, logfile );

	(void)wait( 0 );
}

/*
**	Select and execute the instructor's dump_log script.  If neither
**	exists, report that fact back to the user.
*/
void
run_dump_script( char *dump_log_cmds[2], char *logfile )
{
	reg	int	i;
        extern  int     errno;
	char	*args[4];
	extern	char	**environ;

	/*
	** Execute the instructor's dump_log script.  If there is none,
	** then dumping the log is not allowed; print an appropriate message.
	*/
	args[1] = logfile;
	args[2] = project_code;
	args[3] = NULL;

	for( i = 0; i < 2; i++ ){
		args[0] = dump_log_cmds[i];
		exec( dump_log_cmds[i], args, environ );
		if( errno != ENOENT && errno != EACCES )
			perror( dump_log_cmds[i] );
	}
	if( errno == ENOENT || errno == EACCES )
		printf( "Sorry, you're not allowed to see your log file for project %s.\n", project_code );
	exit( 0 );
}
