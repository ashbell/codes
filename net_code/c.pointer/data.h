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
** data.h
**
**	Global data (too much of this stuff!)
**
**	sccs id: @(#)data.h	1.4      03/10/99
*/

/*
**	Command-line execution options.
*/
extern	int	verbose;		/* verbose (default) or quiet mode */
extern	int	check_mode;		/* -c flag */
extern	int	dump_log_mode;		/* -l flag */
extern	int	news_mode;		/* -n flag */
extern	int	query_mode;		/* -q flag */
extern	int	background_mode;	/* -b flag */

/*
**	Options set in the .tryrc file
*/
extern	int	yyyy;			/* 4-digit year in log file option */
extern	int	init_status;		/* initstatus option */
extern	int	cleanup_status;		/* cleanupstatus option */
extern	int	try_off;		/* off option */
extern	int	try_dummy;		/* dummy option */
extern	int	save_opt;		/* save option */
extern	int	ignore_sig_opt;		/* ignore option */
extern	int	post_signals;		/* post option */
extern	char	*log_dir;		/* directory for log files */
extern	char	*wd_dir;		/* directory in which wd.dirs go */
extern	char	*wd_backpath;		/* gets from wd back to try dir */
extern	char	*script_dir;		/* dir in which common scripts go */
#define	DEFAULT_BACKPATH1	"../"	/* default path back to try dir */
#define	DEFAULT_BACKPATH0	".."	/* default path back to try dir */

/*
**	Misc global variables (I know, there are too many of these).
*/
extern	int	ignore_signals;		/* set when signals are being ignored */
extern	char	*progname;		/* for error messages */
extern	int	logging;		/* set when logging is turned on */
extern	char	general_buf[BUFSIZ];	/* scratch buffer */

/*
**	Uid's and account names
*/
extern	int	try_uid;		/* nonexistent uid */
extern	int	try_gid;		/* nonexistent gid */
extern	char	*inst_acct;		/* instructor's acct name */
extern	int	inst_uid;		/*	"	uid */
extern	int	inst_gid;		/*	"	gid */
extern	char	*inst_dir;		/* 	"	home directory */
extern	char	*try_dir;		/* the try directory */
extern	char	*logfile;		/* student's log file name */
extern	char	*stu_acct;		/*	"    account name */
extern	char	*stu_name;		/*	"    real name */
extern	char	*stu_dir;		/*	"    scratch directory name */
extern	int	stud_uid;		/*	"    uid */
extern	int	stud_gid;		/*	"    gid */

/*
**	Things associated with the current project
*/
extern	char	*project_code;		/* project code */
extern	char	*project_dir;		/* project directory */
extern	char	*build_cmd;		/* pathname to: build script */
extern	char	*build_safe_cmd;	/*	"	build_safe script */
extern	char	*run_cmd;		/*	"	run script */
extern	char	*run_safe_cmd;		/*	"	run_safe script */
extern	char	*explain_cmds[2];	/*	"	explain scripts */
extern	char	*dump_log_cmds[2];	/*	"	dump_log scripts */
extern	char	*init_cmd;		/*	"	init script */
extern	char	*cleanup_cmd;		/*	"	cleanup script */
extern	char	*query_cmd;		/*	"	query script */
extern	char	*news_file[3];		/*	"	news files */

/*
**	Variables used when creating pipes
*/
extern	int	pipedes[2];
