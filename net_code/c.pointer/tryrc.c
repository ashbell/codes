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
** tryrc.c
**
**	Read the instructor's .tryrc file to find the project entry.
*/

static char sccs_id_distr[] = "@(#)tryrc.c	1.9      03/10/99";

#include "try.h"
#include "data.h"

typedef	struct	{
	char	*opt_name;
	int	*opt_flag;
} Option;

Option	option[] = {
	{ "yyyy",	&yyyy },
	{ "initstatus",	&init_status },
	{ "cleanupstatus", &cleanup_status },
	{ "off",	&try_off },
	{ "save",	&save_opt },
	{ "ignore",	&ignore_sig_opt },
	{ "post",	&post_signals },
	{ "dummy",	&try_dummy }
};
#define	N_OPTS	( sizeof( option ) / sizeof( option[0] ) )

typedef	struct	{
	char	*opt_name;
	int	opt_len;
	char	**opt_value;
} Optstr;

Optstr	optstr[] = {
	{ "log_dir=",		8,	&log_dir },
	{ "wd_dir=",		7,	&wd_dir },
	{ "wd_backpath=",	12,	&wd_backpath },
	{ "script_dir=",	11,	&script_dir }
};
#define	N_STROPTS	( sizeof( optstr ) / sizeof( optstr[0] ) )

void
read_tryrc( char *inst_acct, char *inst_dir, char *project_code )
{
	reg	char	*proj_info;
	reg	char	*tryrc_name;
	char	*options;
	char	*def_options;

	/*
	** Create pathname to inst's .tryrc file.
	*/

	tryrc_name = MALLOC( strlen( inst_dir ) + 10, char );
	strcpy( tryrc_name, inst_dir );
	strcat( tryrc_name, "/.tryrc" );

	if( (proj_info = xlate_from_file( project_code, tryrc_name )) == NULL )
		error( ERR_USER + 1, "Instructor \"%s\" does not expect assignment \"%s\"!\n",
			inst_acct, project_code );
		/*NOTREACHED*/

	proj_info = extract_field( proj_info, &try_dir );
	proj_info = extract_field( proj_info, &project_dir );
	(void)extract_field( proj_info, &options );

	/*
	** Look for default options and, if found, process them first.
	*/
	if( (proj_info = xlate_from_file( try_dir, tryrc_name )) != NULL ){
		proj_info = extract_field( proj_info, NULL );
		proj_info = extract_field( proj_info, NULL );
		(void)extract_field( proj_info, &def_options );
		if( def_options != NULL ){
			process_options( def_options );
			free( def_options );
		}
	}
	xlate_end();
	free( tryrc_name );

	if( project_dir == NULL )
		project_dir = project_code;

	if( options != NULL ){
		process_options( options );
		free( options );
	}
}

/*
**	Extract a field from the information line.  We:
**	1) save where the stuff begins,
**	2) move to the end of the stuff (white space) and plug in a NUL,
**	3) pass back a pointer to a copy of the stuff,
**	4) advance past the trailing white space,
**	5) return a pointer to the next field (or NUL).
*/
char *
extract_field( char *start, char **pptr )
{
	reg	char	*bp;
	reg	int	quoted;

	if( pptr != NULL )
		*pptr = NULL;
	if( *start == NUL )
		return start;

	quoted = 0;
	for( bp = start; *bp != NUL; bp++ ){
		if( *bp == '"' || *bp == '\'' ){
			if( quoted == 0 )
				quoted = *bp;
			else if( quoted == *bp )
				quoted = 0;
			continue;
		}
		if( white_space( *bp ) && quoted == 0 )
			break;
	}

	if( *bp != NUL )
		*bp++ = NUL;

	if( pptr != NULL )
		*pptr = strsave( start );

	for( ; *bp != NUL; bp++ )
		if( ! white_space( *bp ) )
			break;

	return bp;
}

/*
**	Options are a comma-separated list of option names.  Portions
**	of the list may be quoted; those portions may contain commas
**	or white space.  We pick up each option one by one and
**	set the appropriate flags for 'em.
*/
void
process_options( char *options )
{
	reg	char	*bp;
	reg	int	i;
	reg	int	quoted;

	while( *options != NUL ){
		quoted = 0;
		for( bp = options; *bp != NUL; bp++ ){
			if( *bp == '"' || *bp == '\'' ){
				if( quoted == 0 )
					quoted = *bp;
				else if( quoted == *bp )
					quoted = 0;
				continue;
			}
			if( *bp == ',' && quoted == 0 ){
				*bp++ = NUL;
				break;
			}
		}

		for( i = 0; i < N_OPTS; i++ )
			if( streq( options, option[i].opt_name ) ){
				*option[i].opt_flag = 1;
				break;
			} else if( nstreq( options, "no_", 3 ) &&
				   streq( options + 3, option[i].opt_name ) ){
				*option[i].opt_flag = 0;
				break;
			}

		if( i == N_OPTS ){
			if( nstrne( options, "no_", 3 ) )
				i = N_STROPTS;
			else
				for( i = 0; i < N_STROPTS; i++ )
					if( nstreq( options + 3, optstr[i].opt_name, optstr[i].opt_len - 1 ) ){
						*optstr[i].opt_value = NULL;
						break;
					}

			if( i == N_STROPTS && !string_env_opt( options ) )
				error( 1, "Invalid option: %s\n", options );
		}

		options = bp;
	}
}

/*
**	Process string and environment variable options.
**	Set an environment variable from a value found in a .tryrc entry.
**	This is done before the environment is massaged, so we can use the
**	'overwrite' flag when setting environment variables; this will
**	allow environment variables on individual project entries to
**	overwrite those on the default entry.
*/
bool
string_env_opt( char *opt )
{
	reg	char	*bp;
	reg	int	quote;
	reg	char	*from;

	for( bp = opt; *bp != NUL; bp++ )
		if( *bp == '=' )
			break;

	if( *bp == NUL )
		return FALSE;

	/*
	** See if the value is quoted; if so, remove the quotes.
	*/
	if( (quote = *++bp) == '"' || quote == '\'' ){
		from = bp + 1;
		while( *bp++ = *from++ ){
			if( *from == quote ){
				from++;
				quote = 0;
			}
		}
	}

	if( !string_opt( opt ) )
		set_new_env( opt, TRUE );
	return TRUE;
}

bool
string_opt( char *opt )
{
	reg	int	i;

	for( i = 0; i < N_STROPTS; i++ ){
		if( nstreq( opt, optstr[i].opt_name, optstr[i].opt_len ) ){
			*optstr[i].opt_value = strsave( opt + optstr[i].opt_len );
			return TRUE;
		}
	}
	return FALSE;
}
