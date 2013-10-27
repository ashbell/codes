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
** alloc.c
**
**	Interface with the real malloc to do memory allocation.  This
**	allows all errror checking for NULL pointers to be done in
**	one place.  The string-save function is also done here.
**
*/

static char sccs_id_distr[] = "@(#)alloc.c	1.2      10/15/97";

#include "try.h"

char *
strsave( reg char *str )
{
	reg	char	*new;

	new = MALLOC( strlen( str ) + 1, char );
	strcpy( new, str );
	return new;
}

#undef	malloc		/* Turn off the saftey net */

char *
alloc( unsigned nbytes )
{
	reg	char	*mem;

	if( (mem = malloc( (unsigned)nbytes )) == NULL ){
		error( ERR_IMPOS + 1, "malloc: out of memory.\n" );
		/*NOTREACHED*/
	}
	return mem;
}
