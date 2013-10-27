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
** try_deblank.c
**
**	Remove leading and trailing blanks; reduce runs of blanks to a
**	single blank.
*/

static char sccs_id_distr[] = "@(#)try_deblank.c	1.3      08/26/98";

#include <stdio.h>
#include <string.h>
#include "try.h"

int	aflg;
int	bflg;
int	rflg;
int	tflg;
int	lflg;
int	oflg;

char	*progname;

main( ac, av )
int ac;
char **av;
{
	char	buf[BUFSIZ];
	reg	int	i;
	reg	char	*bp;

	tdb_do_args( ac, av );
	while( fgets( buf, BUFSIZ, stdin ) != NULL ){
		deblank( buf );

		/*
		** Delete empty lines
		*/
		if( lflg && ( *buf == NL || *buf == NUL ) )
			continue;

		/*
		** One word per line?  Change each blank to a newline.
		*/
		if( oflg )
			for( bp = buf; *bp != NUL; bp++ )
				if( *bp == SP )
					*bp = NL;

		/*
		** Add one leading and one trailing blank
		*/
		if( aflg ){
			i = tdb_stripnl( buf );
			fprintf( stdout, " %s ", buf );
			if( i )
				fputc( NL, stdout );
		} else
			fputs( buf, stdout );
	}
}

deblank( src )
reg char *src;
{
	reg	char	*dst;

	dst = src;

	/*
	** Skip leading blanks
	*/
	if( bflg )
		while( is_a_blank( *src ) )
			src++;

	/*
	** Copy rest of buffer
	*/
	do {
		*dst = *src;
		if( tflg && *dst == TAB )
			*dst = SP;
		if( ! rflg || ! is_a_blank( *dst ) || ! is_a_blank( src[1] ) )
			dst++;
	} while( *src++ != NUL );

	/*
	** Remove trailing blanks
	*/
	if( bflg ){
		dst -= 2;
		if( *dst == NL ){
			if( is_a_blank( *--dst ) ){
				*dst++ = NL;
				*dst = NUL;
			}
		} else {
			if( is_a_blank( *dst ) )
				*dst = NUL;
		}
	}
}

is_a_blank( ch )
reg int ch;
{
	return ch == SP || ( tflg && ch == TAB );
}

tdb_stripnl( bp )
reg char *bp;
{
	reg	char	ch;

	while( (ch = *bp++) != NUL )
		if( ch == NL ){
			*--bp = NUL;
			return TRUE;
		}

	return FALSE;
}

tdb_do_args( ac, av )
reg int ac;
char **av;
{
	reg	int	ch;
	char	*last_path_component();

	progname = last_path_component( av[0] );

	while( (ch = getopt( ac, av, "abrtlo" )) != EOF )
		switch( ch ){
		case 'a': aflg++; break;
		case 'b': bflg++; break;
		case 'r': rflg++; break;
		case 't': tflg++; break;
		case 'l': lflg++; break;
		case 'o': oflg++; break;
		}

	if( ! ( aflg || bflg || rflg || lflg || oflg ) ){
		rflg++;
	}

	if( oflg ){
		rflg++;
		if( aflg ){
			fprintf( stderr, "%s: Warning: -o and -a incompatible, -a turned off.\n", progname );
			aflg = 0;
		}
	}
}

char *
last_path_component( name )
reg char *name;
{
	reg	char	*last;

	if( (last = strrchr( name, '/' )) != NULL )
		last++;
	else
		last = name;

	return last;
}
