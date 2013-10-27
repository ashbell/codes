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
** studir.c
**
**	Creates the student's scratch directory (if needed) and enters it.
**
**	Once that is done, we will have all we need to do logging, so arrange
**	to catch all interrupts and log unsuccessful terminations.
*/

static char sccs_id_distr[] = "@(#)studir.c	1.7      01/27/99";

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "try.h"
#include "data.h"

int	parent_pid;

void
enter_scratch_dir( char *stu_dir, char *stu_acct )
{
	extern	int	errno;

	if( chmod( stu_dir, 0777 ) < 0 ){
		switch( errno ){
		case ENOENT:
			create_intermediate_directories( stu_dir, 0711 );
			if( mkdir( stu_dir, 0777 ) < 0 )
				sys_error( ERR_IMPOS + 1, stu_dir, "mkdir" );
				/*NOTREACHED*/

#ifndef	SETREUID
			if( chown( stu_dir, inst_uid, inst_gid ) < 0 )
				sys_error( ERR_IMPOS + 1, stu_dir, "chown" );
#endif
			break;

		default:
			sys_error( ERR_IMPOS + 1, stu_dir, "chmod" );
			/*NOTREACHED*/
		
		}
	}

	if( chdir( stu_dir ) < 0 ){
		sys_error( ERR_IMPOS + 1, stu_dir, "chdir" );
		/*NOTREACHED*/
	}

	/*
	** See if another 'try' is currently running.  If so, abort.
	*/
	parent_pid = getpid();
	is_try_already_running();

	/*
	** Make sure the directory is clean, then create .account file.
	*/
	clean_directory( TRUE );
	write_account_name( stu_acct );
	write_tryid();
}

void
write_account_name( char *acct )
{
	reg	FILE	*f;

	if( (f = fopen( ".account", "w" )) == NULL ){
		sys_error( ERR_IMPOS + 1, ".account", "fopen" );
		/*NOTREACHED*/
	}
	fprintf( f, "%s\n", acct );
	fclose( f );

#ifndef	SETREUID
	(void)chown( ".account", inst_uid, inst_gid );
#endif
}

char	tryid_file[]	= ".tryid";

void
write_tryid()
{
	reg	FILE	*f;
	char	hostname[100];
	extern	time_t	try_time;

	if( (f = fopen( tryid_file, "w" )) == NULL ){
		sys_error( ERR_IMPOS + 1, tryid_file, "fopen" );
		/*NOTREACHED*/
	}
	if( gethostname( hostname, 100 ) != 0 )
		strcpy( hostname, "unknown" );
	fprintf( f, "%d,%s,%s", parent_pid, hostname, ctime( &try_time ) );
	fclose( f );

#ifndef	SETREUID
	(void)chown( tryid_file, inst_uid, inst_gid );
#endif
}

/*
**	Remove all files in the directory.  Only the parent process
**	should do this -- this protection tries to prevent an error in
**	try from blowing away all of the student's files.
**
**	The tryid_file is always removed to prevent false "multiple try"
**	errors.
**
**	KAR 5/9/94: Added code to recursively remove subdirectories.
*/
void
clean_directory( bool unconditional )
{
	if( getpid() != parent_pid )
		return;

	unlink( tryid_file );

	if( !unconditional && save_opt )
		return;

	clean_one_dir();
}

void
clean_one_dir()
{
	reg	DIR	*dirp;
	struct	dirent	*dp;
	struct	stat	statbuf;


	if( (dirp = opendir( "." )) == NULL )
		sys_error( ERR_IMPOS + 1, ".", "opendir" );
		/*NOTREACHED*/

	while( (dp = readdir( dirp )) != NULL ){
		if( streq( dp->d_name, "." ) || streq( dp->d_name, ".." ) )
			continue;

		/*
		** Dangerous bug in sgi OS (at least with NFS): unlinking
		** a directory actually removes it but does not adjust
		** the link count of the parent directory.  So we have
		** to do a stat to find out whether it is really a file
		** before trying to delete it.
		**
		** Note: as of 2/10/95 we want to do the stat anyway, as
		** we need the mode of directories further down.  Besides,
		** I have observed some incorrect link counts even on
		** directories on Sun sparcs.
		*/

		/*
		** 1/27/99: Do an lstat instead of a stat.  If someone
		** creates a symlink to a directory outside of the
		** scratch directory, we do NOT want to follow that
		** link and start deleting stuff!
		*/
		if( lstat( dp->d_name, &statbuf ) < 0 ){
			continue;
		}
		if( ( statbuf.st_mode & S_IFMT ) != S_IFDIR ){
			/*
			** Execution comes here for symlinks; they're
			** deleted here along with the ordinary files.
			*/
			unlink( dp->d_name );
			continue;
		}

		
		/*
		** It is a directory.  If we have SETREUID (and therefore
		** do not have "powerful root"), make sure that the directory
		** has mode 777 so we can delete what we find therein.
		*/
#ifdef	SETREUID
		if( ( statbuf.st_mode & 0777 ) != 0777 ){
			effective_id( statbuf.st_uid, statbuf.st_gid );
			chmod( dp->d_name, 0777 );
			effective_root();
		}
#endif
		if( chdir( dp->d_name ) == 0 ){
			clean_one_dir();
			chdir( ".." );
			rmdir( dp->d_name );
		}
	}
	closedir( dirp );
}
