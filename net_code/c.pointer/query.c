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
** query.c
**
**	Run the instructor's query script, if it exists.
*/

static char sccs_id_distr[] = "@(#)query.c	1.4      10/15/97";

#include "try.h"

void
query( char *query_cmd )
{
	reg	int	mode;

	effective_inst();
	if( (mode = file_mode( query_cmd )) == NO_FILE || ( mode & 0500 ) != 0500 ){
		printf( "Sorry - no inquiries are possible with this project.\n" );
		return;
	}

	child_proc( become_inst, run_query_script, query_cmd );
	(void)wait( NULL );
}

void
run_query_script( char *query_cmd )
{
	char	*args[2];
        extern  int     errno;

	/*
	** Execute the instructor's query script. 
	*/
	args[0] = query_cmd;
	args[1] = NULL;
	exec( query_cmd, args, environ );
	perror( query_cmd );
	exit( 0 );
}
