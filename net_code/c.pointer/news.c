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
** news.c
**
**	Print the news file(s) if they exist.  Update a .try_news file
**	in the student's home directory to prevent them from seeing a news
**	item more than once.
*/

static char sccs_id_distr[] = "@(#)news.c	1.2      12/6/94";

#include <sys/stat.h>
#include <errno.h>
#include "try.h"
#include "data.h"

#define	GENERAL_NEWS	0
#define	SHARED_PROJECT_NEWS	1
#define	SPECIFIC_PROJECT_NEWS	2

FILE	*news_log = NULL;
FILE	*more_file = NULL;

void
news()
{
	reg	int	i;
	struct	stat	statb;

	effective_inst();

	for( i = 0; i < 3; i++ ){
		if( stat( news_file[i], &statb ) < 0 ||
		    ( statb.st_mode & 0400 ) == 0 ){
			continue;
		}
		if( new_news( i, statb.st_mtime ) )
			report_news( i, statb.st_mtime );

		if( try_dummy )
			break;
	}

	end_news();

	effective_root();
}

/*
**	Maintain a log file in the student's home directory to keep track
**	of what news the student has read so far.
**
**	Format of log file:
**
**		name0 date0
**		name1 date1
**		name2 date2
**		...
**
**	The dates are modification dates of news files.  
**	Each name is the concatenation of the inst-acct and project-code
**	given to try (no project code for general news).  A '\001'
**	separates the two parts.
**
**	Entries are kept in this file in NO PARTICULAR ORDER.  When an
**	entry is printed, its modification time is stored in the file.
*/

#define	NEWS_BUF	275	/* MAXDIRNAM (BSD) length plus project-code */

bool
new_news( int i, time_t mtime )
{
	char	buf[NEWS_BUF];
	char	news_name[NEWS_BUF];
	time_t	last_read;

	if( news_mode )
		return TRUE;
	open_news_log();
	if( news_log == NULL )
		return TRUE;

	strcpy( news_name, inst_acct );
	strcat( news_name, "\1" );
	if( i > 0 )
		strcat( news_name, project_code );

	do_forever {
		if( fgets( buf, NEWS_BUF, news_log ) == NULL ){
			fprintf( news_log, "%s\n", news_name );
			fwrite( (char *)&mtime, sizeof( mtime ), 1, news_log );
			fflush( news_log );
			return TRUE;
		}
		(void)fread( (char *)&last_read, sizeof( last_read ), 1, news_log );

		stripnl( buf );
		if( streq( news_name, buf ) ){
			if( mtime <= last_read )
				return FALSE;
			fseek( news_log, (long)( -sizeof( last_read ) ), 1 );
			fwrite( (char *)&mtime, sizeof( mtime ), 1, news_log );
			return TRUE;
		}
	}
}

void
open_news_log()
{
	reg	char	*bp;
	char	buf[BUFSIZ];
	extern	int	errno;

	if( news_log != NULL ){
		fseek( news_log, 0L, 0 );
		return;
	}
	
	if( (bp = getenv( "STUDENT_HOME" )) == NULL )
		return;

	strcpy( buf, bp );
	strcat( buf, "/.try_news" );

	effective_stud();
	if( (news_log = fopen( buf, "r+" )) == NULL && errno == ENOENT ){
		if( (news_log = fopen( buf, "w+" )) != NULL ){
			fseek( news_log, 0L, 0 );
			(void)chmod( buf, 0600 );
#ifndef	SETREUID
			if( chown( buf, stud_uid, stud_gid ) < 0 )
				sys_error( ERR_IMPOS, buf, "chown" );
				/*NOTREACHED*/
#endif
		}
	}
	effective_inst();
}

void
end_news()
{
	if( news_log != NULL )
		fclose( news_log );
	if( more_file != NULL ){
		fclose( more_file );
		(void)wait( 0 );
		printf( "\nPress RETURN to continue... " );
		while( getchar() != '\n' )
			;
		putchar( NL );
	}
}

void
report_news( int type, time_t mtime )
{
	FILE	*file;
	char	buf[BUFSIZ];
	reg	int	len;

	if( (file = fopen( news_file[type], "r" )) == NULL )
		return;

	if( more_file == NULL && (more_file = run_more()) == NULL )
		return;

	news_heading( type, mtime );

	while( (len = fread( buf, 1, BUFSIZ, file )) > 0 )
		fwrite( buf, 1, len, more_file );

	fprintf( more_file, "--------------------------------------------------------\n" );
	fclose( file );
}

void
news_heading( int type, time_t mtime )
{
	if( type == 0 )
		fprintf( more_file, "\n\nGeneral News" );
	else
		fprintf( more_file, "\n\nNews for project %s", project_code );

	fprintf( more_file, " -- last updated %24.24s:\n\n", ctime( &mtime ) );
}

FILE *
run_more()
{
	int	more_pipe[2];

	if( pipe( more_pipe ) < 0 )
		return NULL;

	child_proc( become_inst, more_proc, more_pipe[0] );
	close( more_pipe[0] );
	return fdopen( more_pipe[1], "w" );
}

void
more_proc( int pipe_fd )
{
	reg	int	i;

	close( 0 );
	dup( pipe_fd );
	for( i = 3; i < 20; i++ )
		close( i );
#ifdef	PG_PAGER
	execl( "/usr/bin/pg", "pg", "-e", NULL );
	perror( "/usr/bin/pg" );
#else
	execl( "/usr/ucb/more", "more", "-d", NULL );
	perror( "/usr/ucb/more" );
#endif
	exit( 1 );
}
