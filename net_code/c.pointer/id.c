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
** id.c
**
**	Routines to change our effective identity.  The first sets the
**	real uid and gid to root in order that we can return to root
**	after having become someone else.  The rest then allows us to
**	become someone else.  None of the "effective" calls are used
**	in System 5.
*/

static char sccs_id_distr[] = "@(#)id.c	1.5      10/15/97";

#include "try.h"
#include "data.h"

int	prev_uid;
int	prev_gid;

void
anchor_root_identity()
{
#ifdef	SETREUID
	reg	int	i;

	prev_gid = i = getegid();
	setregid( i, i );
	prev_uid = i = geteuid();
	setreuid( i, i );
#else
	setgid( getegid() );
	setuid( geteuid() );
#endif
}

void
save_id(){
#ifdef	SETREUID
	prev_gid = getegid();
	prev_uid = geteuid();
#endif
}

void
effective_id( int uid, int gid )
{
#ifdef	SETREUID
	if( uid != 0 && geteuid() != 0 )
		effective_root();
	setregid( -1, gid );
	setreuid( -1, uid );
#endif
}

void
effective_prev()
{
#ifdef	SETREUID
	if( prev_uid != 0 && geteuid() != 0 )
		effective_root();
	setregid( -1, prev_gid );
	setreuid( -1, prev_uid );
#endif
}

void
effective_root()
{
#ifdef	SETREUID
	setregid( -1, getgid() );
	setreuid( -1, getuid() );
#endif
}

void
effective_inst()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	setregid( -1, inst_gid );
	setreuid( -1, inst_uid );
#endif
}

void
effective_try()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	setregid( -1, try_gid );
	setreuid( -1, try_uid );
#endif
}

void
effective_stud()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	setregid( -1, stud_gid );
	setreuid( -1, stud_uid );
#endif
}

void
become_stud()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	initgroups( stu_acct, stud_gid );
	setregid( stud_gid, stud_gid );
	setreuid( stud_uid, stud_uid );
#else
	setgid( stud_gid );
	setuid( stud_uid );
#endif
}

void
become_inst()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	initgroups( inst_acct, inst_gid );
	setregid( inst_gid, inst_gid );
	setreuid( inst_uid, inst_uid );
#else
	setgid( inst_gid );
	setuid( inst_uid );
#endif
}

void
become_try()
{
#ifdef	SETREUID
	if( geteuid() != 0 )
		effective_root();
	initgroups( TRY_ACCT, try_gid );
	setregid( try_gid, try_gid );
	setreuid( try_uid, try_uid );
#else
	setgid( try_gid );
	setuid( try_uid );
#endif
}
