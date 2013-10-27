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
** run.c
**
**	Call the instructor's script to run the student's program.
*/

static char sccs_id_distr[] = "@(#)run.c	1.5      10/15/97";

#include <errno.h>
#include "try.h"
#include "data.h"

int
run_tests( char *run_cmd, char *run_safe_cmd, char *explain_cmds[], char **args )
{
	int	status;
	reg	int	test;
	reg	int	i;
	reg	int	used_run;
	char	logmsg[BUFSIZ];
	extern	int	ignore_signals;

	/*
	** Create try_log message pipe
	*/
	if( pipe( pipedes ) < 0 )
		sys_error( ERR_IMPOS + 1, "run", "pipe" );
		/*NOTREACHED*/

	/*
	** Select script to use, and call it.
	*/
	effective_inst();
        if( (i = file_mode( run_safe_cmd )) != NO_FILE && ( i & 0500 ) == 0500 ){
                if( ( i & 05 ) != 05 ){
                        i |= 0555;
                        (void)chmod( run_safe_cmd, i );
                }
                used_run = FALSE;
                child_proc( become_try, do_build_run_script,
                        run_safe_cmd, args );

        } else if( (i = file_mode( run_cmd )) != NO_FILE && ( i & 0500 ) == 0500 ){                used_run = TRUE;
                child_proc( become_inst, do_build_run_script,
                        run_cmd, args );

        } else {
		error( 1, "No run or run_safe script!\n" );
                /*NOTREACHED*/
        }

	close( pipedes[1] );
	if( (i = read( pipedes[0], logmsg, BUFSIZ )) > 0 )
		logmsg[i] = '\0';
	close( pipedes[0] );

	(void)wait( &status );

	/*
	** Now that the testing is done, ignore signals.  This prevents
	** 'em from interrupting the cleanup script.
	*/
	ignore_signals = TRUE;

	if( ( status & 0xff ) != 0 )
		error( 1, "%s script aborted!\n",
			used_run ? run_cmd : run_safe_cmd );
		/*NOTREACHED*/

	status = ( status >> 8 ) & 0xff;

	if( status != 0 ){
		if( status == 128 ){
			error( 1, "%s script could not be executed.\n",
				used_run ? run_cmd : run_safe_cmd );
			/*NOTREACHED*/
		}
		test = status & 0x7f;
		if( i > 0 )
			log( "Failed test %d: %s\n", test, logmsg );
		else
			log( "Failed test %d.\n", test );

		{
			reg	char	*run_fmt;

			if( (run_fmt = getenv( "TRY_FAIL_RUN" )) == NULL )
				run_fmt = "Your project failed test %d.";
			if( *run_fmt ){
				printf( run_fmt, test );
				putchar( NL );
			}
		}
		child_proc( become_inst, run_explain_script, explain_cmds, status );
		(void)wait( 0 );/* must wait for script to finish before
				returning and deleting files! */
		return RUN_FAILED;
	}

	if( i > 0 )
		log( "Completed: %s\n", logmsg );
	else
		log( "Completed.\n" );
	/*
	** Print the "successfully completed" message in try.c only after the
	** cleanup script has been called.
	*/
	return RUN_COMPLETED;
}

/*
**	Set up to run the instructor's explain script.
*/
void
run_explain_script( char *explain_cmds[], int num )
{
	reg	int	i;
	char	*args[3];
	char	buf[12];

	sprintf( buf, "%d", num );
	args[1] = buf;
	args[2] = NULL;

	for( i = 0; i < 2; i++ ){
		args[0] = explain_cmds[i];
		exec( explain_cmds[i], args, environ );
		if( errno != ENOENT && errno != EACCES )
			perror( explain_cmds[i] );
	}
	/*
	** Explain is optional, so if the exec fails, no problem.
	*/
	exit( 0 );
}
