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
** signal_names.c
**
**	Descriptions for each signal, used in logging.
**
** NOTE: YOU MAY NEED TO EDIT THIS TABLE TO MATCH THE SIGNAL DEFINITIONS
**	 ON YOUR SYSTEM!
*/

static char sccs_id_distr[] = "@(#)signal_names.c	1.3      05/04/99";

char	*signal_names[] = {
	"Signal 0",
	"Hangup",
	"Interrupt",
	"Quit",
	"Illegal instruction",
	"Trace trap",
	"IOT instruction",
	"EMT instruction",
	"Floating point exception",
	"Kill",
	"Bus error",
	"Memory fault",
	"Bad argument to system call",
	"Write on pipe with no reader",
	"Alarm clock",
	"Software termination signal",
	"SIGUSR1",
	"SIGUSR2",
	"SIGCHLD",
	"SIGPWR",
	"SIGWINCH",
	"SIGURG",
	"SIGPOLL",
	"SIGSTOP",
	"SIGTSTP",
	"SIGCONT",
	"SIGTTIN",
	"SIGTTOU",
	"SIGVTALRM",
	"SIGPROF",
	"Cpu time limit exceeded",
	"File size limit exceeded",
	"SIGWAITING",
	"SIGLWP",
	"SIGFREEZE",
	"SIGTHAW",
	"SIGCANCEL",
	"SIGLOST"
};

/*
** The number of the last signal whose name appears in the table above.
*/
int	last_signal = sizeof( signal_names ) / sizeof( signal_names[ 0 ] ) - 1;
