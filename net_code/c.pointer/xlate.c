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
** xlate.c
**
**	Translate an entry from the .ttyrc file.  This searches the file
**	for a line whose first entry matches the key, and returns a
**	pointer to the rest of the line.
*/

static char sccs_id_distr[] = "@(#)xlate.c	1.4      10/15/97";

#include "try.h"

static	FILE	*xlate_file;

/*
** getline -- get a line from the .tryrc file.  Lines that end with 
**	a backslash get the following line appended to them, though
**	leading whitespace is deleted from following lines.
*/
char *
getline( char *buf, int len, FILE *f )
{
	reg	int	ch;
	reg	int	prev_ch;

	prev_ch = getc( f );
	switch( prev_ch ){
	case EOF:
		return NULL;

	default:
		while( len > 2 ){
			ch = getc( f );
			if( ch == EOF )
				break;
			if( ch == NL ){
				if( prev_ch != '\\' ){
					*buf++ = prev_ch;
					prev_ch = NL;
					break;
				}
				while( ( ch = getc( f ) ) != EOF && white_space( ch ) )
					;
				prev_ch = ch;
				continue;
			}
			len -= 1;
			*buf++ = prev_ch;
			prev_ch = ch;
		}
		break;

	case '\n':
		break;
	}

	*buf++ = prev_ch;
	*buf++ = NUL;
	return buf;
}

char *
xlate_from_file( char *key, char *fname )
{
	reg	int	len;
	char	tmpbuf[BUFSIZ];
	extern	char	general_buf[BUFSIZ];

	if( xlate_file != NULL )
		fseek( xlate_file, 0L, 0 );
	else
		if( (xlate_file = fopen( fname, "r" )) == NULL )
			return NULL;

	len = strlen( key );

	while( getline( tmpbuf, BUFSIZ, xlate_file ) != NULL ){
		if( *tmpbuf == '#' || *tmpbuf == NL )
			continue;
		if( nstreq( key, tmpbuf, len ) && white_space( tmpbuf[len] ) ){
			while( white_space( tmpbuf[++len] ) )
				;
			strcpy( general_buf, tmpbuf + len );
			stripnl( general_buf );
			return general_buf;
		}
	}
	return NULL;
}

void
xlate_end()
{
	if( xlate_file != NULL ){
		fclose( xlate_file );
		xlate_file = NULL;
	}
}

bool
white_space( int ch )
{
	return ch == SP || ch == TAB;
}
