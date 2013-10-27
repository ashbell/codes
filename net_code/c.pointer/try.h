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
** try.h
**
**	Try parameters.
**
**	sccs id: @(#)try.h	1.10      08/26/98
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef	SYS_TIME
	#include <sys/time.h>
#else
	#include <time.h>
#endif

/*
** Architecture dependent definitions.  One group is selected by the
** architecture specified in the make script.
*/
#undef	GOT_ARCH
#ifdef	ARCH_sparc
#	define	GOT_ARCH
#	define	BSD_RLIMIT
#	define	SETREUID
#	define	NEWPATH		"/usr/local/bin:/usr/ucb:/bin:/usr/bin"
#	define	TRY_UID		42
#	define	TRY_ACCT	"try"
#endif
#ifdef	ARCH_sgi
#	define	GOT_ARCH
#	define	BSD_RLIMIT
#	define	SETREUID
#	define	NEWPATH		"/usr/local/bin:/usr/ucb:/bin:/usr/bin"
#	define	TRY_UID		42
#	define	TRY_ACCT	"try"
#endif
#ifdef	ARCH_solaris
#	define	GOT_ARCH
#	define	BSD_RLIMIT
#	define	SETREUID
#	define	UNISTD
#	define	NEWPATH		"/usr/local/bin:/usr/ccs/bin:/bin:/usr/bin:/usr/ucb"
#	define	TRY_UID		42
#	define	TRY_ACCT	"try"
#endif

#ifdef	GOT_ARCH
#	undef	GOT_ARCH
#else
	error! no architecture defined
#endif

/*
**	Shorthand notations
*/
#define	reg		register

#define until(x)	while(!(x))
#define do_forever	for(;;)

#define streq(x,y)	(strcmp(x,y)==0)
#define nstreq(x,y,n)	(strncmp(x,y,n)==0)
#define strne(x,y)	(strcmp(x,y)!=0)
#define nstrne(x,y,n)	(strncmp(x,y,n)!=0)

#define CR		'\r'
#define NL		'\n'
#define FF		'\f'
#define BS		'\b'
#define TAB		'\t'
#define	SP		' '
#define NUL		'\0'

#ifndef	NULL
#define	NULL		0
#endif

/*
**	New types.
*/
typedef	int		bool;
#ifndef	TRUE
#define TRUE		1
#define FALSE		0
#endif

/*
**	Define MALLOC macro to do casts and compute size.
**	Then make sure malloc is not called directly.
*/
#define	malloc		DONT_USE_MALLOC
#define	MALLOC(x,y)	(y *)alloc( (x) * sizeof(y) )

/*
**	Return value from the file_mode function, indicating that the
**	named file does not exist or is not accessible.
*/
#define	NO_FILE		-1

/*
**	Error codes.  The flags indicate whether or not the error is something
**	caused by the user, and whether or not it is considered impossible.
**	The code portion is used as the exit status.
*/
#define	ERR_USER	0x10000
#define ERR_IMPOS	0x20000
#define	ERR_CODE_MASK	0xffff
#define	ERR_SEE_INST_MSG "Cannot check out your project -- please see your instructor.\n"

/*
**	Codes for reporting status to the log file.
*/
#define	BUILD_FAILED	-1
#define	RUN_FAILED	0
#define	RUN_COMPLETED	1

/*
**	Codes for failures in try_run itself (as opposed to the user's
**	program executed by try_run).
*/
#define	TRYRUN_USAGE	0xff
#define	TRYRUN_NO_FORK	0xfe
#define	TRYRUN_NO_PIPE	0xfd
#define	TRYRUN_NO_EXEC	0x80

/*
**	Function prototypes.
*/
void	add_new_environment_vars( void );
char	*alloc( unsigned nbytes );
void	anchor_root_identity( void );
void	backpath( char *buf );
void	become_inst( void );
void	become_prev( void );
void	become_root( void );
void	become_stud( void );
void	become_try( void );
void	build_default_entry( char *env, char *value, char *def_value );
void	build_file_names( void );
void	build_new_env_entry( char *name, char *value );
bool	build_student_work( char *build_cmd, char *build_safe_cmd, char **args );
void	change_file_modes( bool used_build );
time_t	change_modes( char *newest_name );
void	check_dirs( void );
bool	check_file( char *path, int min_mode, int flag );
void	check_files( void );
void	check_pair( char *first, int first_mode, char *second, int second_mode, int flag );
void	check_try_dir( char *dir );
void	child_proc();
void	clean_directory( bool unconditional );
void	clean_one_dir( void );
void	create_intermediate_directories( char *pathname, int mode );
char	**do_args( int ac, char ** av );
void	do_build_run_script( char *cmd, char **args );
void	dump_log( char *logfile, char *dump_log_cmds[2] );
void	effective_id( int uid, int gid );
void	effective_inst( void );
void	effective_prev( void );
void	effective_root( void );
void	effective_try( void );
void	effective_try( void );
void	end_news( void );
void	enter_scratch_dir( char *stu_dir, char *stu_acct );
void	error( int code, char *fmt, ... );
void	exec( char *path, char **args, char **env );
void	exec_script( char *path, char **args, char **env );
void	exit_child( int );
char	*extract_field( char *start, char **pptr );
void	fix_environment( char *newhome, char **user_args );
char	*getline( char *buf, int len, FILE *f );
void	housekeeping( char *cmd, char **args, bool check_status, char *which_script );
bool	is_numeric( char *arg );
void	is_try_already_running( void );
void	kill_off_children( void );
char	*last_path_component( char *name );
void	log( char *fmt, ... );
void	log0( char *fmt, va_list args );
void	look_up_accts( bool check_mode, char *inst_acct, int *uidp, int *gidp, char **dirpp, char **stu_acct, char **stu_name, int *try_uid, int *try_gid );
int	make_pipe_and_child( void );
void	more_proc( int pipe_fd );
bool	new_news( int i, time_t mtime );
void	news( void );
void	news_heading( int type, time_t mtime );
void	open_news_log( void );
void	post_signal( int sig );
void	process_options( char *options );
void	query( char *query_cmd );
void	quit( int code );
int	read_student_files( char **namep );
void	read_tryrc( char *inst_acct, char *inst_dir, char *project_code );
void	report_news( int type, time_t mtime );
void	run_dump_script( char *dump_log_cmds[2], char *logfile );
void	run_explain_script( char *explain_cmds[], int num );
FILE	*run_more( void );
void	run_query_script( char *query_cmd );
int	run_tests( char *run_cmd, char *run_safe_cmd, char *explain_cmds[], char **args );
void	save_id( void );
void	set_new_env( char *value, bool overwrite );
void	signal_catcher( int sig );
bool	string_env_opt( char *opt );
bool	string_opt( char *opt );
void	stripnl( char *bp );
char	*strsave( char * str );
void	sys_error( int code, char *name, char *syscall );
bool	white_space( int ch );
void	write_account_name( char *acct );
char	**write_copies_of_student_files( int reader_pid );
void	write_tryid( void );
void	xlate_end( void );
char	*xlate_from_file( char *key, char *fname );

/*
**	Externals that come from the library
*/
extern	char	**environ;
