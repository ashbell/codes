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
** misc.c
**
**	Miscellaneous routines.
*/

static char sccs_id_distr[] = "@(#)misc.c	1.3      10/15/97";

#include <ctype.h>
#include <string.h>
#include "try.h"

bool
is_numeric( char *arg )
{
	do {
		if( !isdigit( *arg ) )
			return FALSE;
	} while( *++arg != '\0' );

	return TRUE;
}

char *
last_path_component( char *name )
{
	reg	char	*last;

	if( (last = strrchr( name, '/' )) != NULL )
		last++;
	else
		last = name;

	return last;
}

void
stripnl( char *bp )
{
	while( *bp != '\0' )
		if( *bp++ == NL ){
			*--bp = '\0';
			return;
		}
}
