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
** try.c
**
**	Main driver
*/

static char sccs_id_distr[] = "@(#)try.c	1.11      10/15/97";

#include <signal.h>
#include "try.h"
#include "data.h"

time_t	try_time;

int
main( int ac, char **av )
{
	reg	int	reader_pid;
	reg	char	**user_args;
	reg	int	i;

	verbose = TRUE;
	(void)time( &try_time );
	stud_uid = getuid();
	stud_gid = getgid();

	av = do_args( ac, av );
	umask( 0 );
	look_up_accts( check_mode,
		inst_acct, &inst_uid, &inst_gid, &inst_dir,
		&stu_acct, &stu_name,
		&try_uid, &try_gid );

	anchor_root_identity();
	effective_inst();
	read_tryrc( inst_acct, inst_dir, project_code );

	if( ! ( query_mode || news_mode || dump_log_mode ) ){
		if( try_dummy ){
			error( ERR_USER + 0, "Submissions are not accepted for \"%s\".\n",
				project_code );
			exit( 0 );
		}

		if( !check_mode && !try_off ){
			effective_stud();
			if( verbose ){
				printf( "Copying files..." );
				fflush( stdout );
			}
			reader_pid = read_student_files( av );
			effective_inst();
		}
	}

	build_file_names();
	if( chdir( inst_dir ) < 0 )
		sys_error( ERR_IMPOS + 1, inst_dir, "chdir" );
		/*NOTREACHED*/

	if( check_mode )
		check_try_dir( try_dir );

	if( chdir( try_dir ) < 0 )
		sys_error( ERR_IMPOS + 1, try_dir, "chdir" );
		/*NOTREACHED*/

	if( check_mode )
		check_dirs();

	enter_scratch_dir( stu_dir, stu_acct );

	if( check_mode ){
		check_files();
		quit( 0 );
	}
	effective_root();

	/*
	** If the program gets this far, the try will be logged no matter
	** how it turns out (except for dump-log and news runs,
	** which are never logged).
	*/
	for( i = SIGHUP; i <= SIGTERM; i++ )
		signal( i, signal_catcher );
	if( !news_mode && !dump_log_mode && !query_mode )
		logging = TRUE;

	user_args = NULL;
	if( !query_mode && !dump_log_mode && !news_mode && !try_off ){
		user_args = write_copies_of_student_files( reader_pid );
		if( verbose ){
			printf( "done\n" );
			fflush( stdout );
		}
	}

	/*
	** Turn on signal ignoring now if it was specified in the
	** .tryrc options.
	*/
	ignore_signals = ignore_sig_opt;
	fix_environment( inst_dir, user_args );

	if( ! background_mode )
		news();
	if( news_mode )
		quit( 0 );

	if( dump_log_mode ){
		dump_log( logfile, dump_log_cmds );
		quit( 0 );
	}

	if( query_mode ){
		query( query_cmd );
		quit( 0 );
	}

	if( try_off ){
		error( ERR_USER + 0, "Sorry, submissions are no longer being accepted for project \"%s\".\n",
			project_code );
		quit( 0 );
	}

	housekeeping( init_cmd, user_args, init_status, "init" );
	add_new_environment_vars();

	i = BUILD_FAILED;
	if( build_student_work( build_cmd, build_safe_cmd, user_args ) ){
		add_new_environment_vars();
		i = run_tests( run_cmd, run_safe_cmd, explain_cmds, user_args );
	}
	add_new_environment_vars();

	user_args[1] = i == RUN_COMPLETED ? "1" : "0";
	user_args[2] = NULL;
	housekeeping( cleanup_cmd, user_args, cleanup_status, "cleanup" );

	if( i == RUN_COMPLETED ){
		reg	char	*comp_fmt;

		if( (comp_fmt = getenv( "TRY_SUCCESS" )) == NULL )
			comp_fmt = "You have successfully completed project %s!";
		if( *comp_fmt ){
			printf( comp_fmt, project_code );
			putchar( NL );
		}
	}
	quit( 0 );
	/*NOTREACHED*/
	return 0;
}

void
backpath( char *buf )
{
	if( wd_backpath == NULL )
		strcpy( buf, DEFAULT_BACKPATH1 );
	else {
		strcpy( buf, wd_backpath );
		strcat( buf, "/" );
	}
}

/*
**	Create the path names to the various scripts and files.
**	These contain the project directory name or student's account.
*/
void
build_file_names()
{
	char	buf[BUFSIZ];
	reg	char	*bp;

	/*
	** Working directory name (relative to try directory; all other paths
	** are relative to the working directory).
	*/
	if( wd_dir != NULL ){
		strcpy( buf, wd_dir );
		strcat( buf, "/" );
	}
	else
		*buf = 0;
	strcat( buf, "wd." );
	strcat( buf, stu_acct );
	stu_dir = strsave( buf );

	/*
	** Log file name
	*/
	backpath( buf );
	if( log_dir != NULL ){
		strcat( buf, log_dir );
		strcat( buf, "/" );
	}
	strcat( buf, "log." );
	strcat( buf, stu_acct );
	logfile = strsave( buf );

	/*
	** Project directory stuff; bp points to end of "../proj_dir_name"
	*/
	backpath( buf );
	strcat( buf, project_dir );
	bp = buf + strlen( buf );

	strcpy( bp, "/build" );
	build_cmd = strsave( buf );

	strcpy( bp, "/build_safe" );
	build_safe_cmd = strsave( buf );

	strcpy( bp, "/run" );
	run_cmd = strsave( buf );

	strcpy( bp, "/run_safe" );
	run_safe_cmd = strsave( buf );

	strcpy( bp, "/init" );
	init_cmd = strsave( buf );

	strcpy( bp, "/cleanup" );
	cleanup_cmd = strsave( buf );

	strcpy( bp, "/query" );
	query_cmd = strsave( buf );

	strcpy( bp, "/explain" );
	explain_cmds[0] = strsave( buf );

	strcpy( bp, "/dump_log" );
	dump_log_cmds[0] = strsave( buf );

	strcpy( bp, "/news" );
	news_file[2] = strsave( buf );

	strcat( bp, "." );
	strcat( bp, project_code );
	news_file[1] = strsave( buf );

	backpath( buf );
	if( script_dir != NULL ){
		strcat( buf, script_dir );
		strcat( buf, "/" );
	}
	bp = buf + strlen( buf );
	strcpy( bp, "explain" );
	explain_cmds[1] = strsave( buf );
	strcpy( bp, "dump_log" );
	dump_log_cmds[1] = strsave( buf );

	backpath( buf );
	bp = buf + strlen( buf );
	strcpy( bp, "news" );
	news_file[0] = strsave( buf );
}

/*
**	Process invocation arguments.  This picks off all options, and
**	saves pointers to the instructor's account and the project.
*/
char **
do_args( int ac, char ** av )
{
	reg	int	ch;
	reg	int	invoke_err = 0;
	extern	int	optind;
	extern	char	*optarg;

	progname = last_path_component( *av );

	while( (ch = getopt( ac, av, "bclnqsd:" )) != EOF ){
		switch( ch ){
		case 'b':
			background_mode = TRUE;
			break;

		case 'c':
			check_mode = TRUE;
			break;

		case 'l':
			dump_log_mode = TRUE;
			break;

		case 'n':
			news_mode = TRUE;
			break;

		case 'q':
			query_mode = TRUE;
			break;

		case 's':
			verbose = FALSE;
			break;

		default:
			invoke_err++;
			break;
		}
	}

	if( news_mode && background_mode )
		error( ERR_USER + 0, "Cannot get news in background mode.\n" );

	av += optind;
	if( *av == NULL )
		invoke_err++;
	else {
		inst_acct = *av++;

		if( *av == NULL )
			invoke_err++;
		else {
			project_code = *av++;
			if( !query_mode && !news_mode &&
			    !check_mode && !dump_log_mode && *av == NULL )
				invoke_err++;
		}
	}

	if( invoke_err ){
		fprintf( stderr, "Usage: try [ -b -l -n -q ] inst-acct project [ file ... ]\nOne or more files are required except when the -l, -n or -q options are given.\n" );
		exit( 1 );
	}

	return av;
}

void
quit( int code )
{
	effective_root();
	clean_directory( FALSE );
	exit( code );
}
