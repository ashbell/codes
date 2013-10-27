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
** inst.c
**
**	Look up instructor's uid, gid, and home dir.  Look up the nonexistent
**	uid and gid.  Look up the student's information (incl. name), too.
*/

static char sccs_id_distr[] = "@(#)passwd.c	1.3      10/15/97";

#include <pwd.h>
#include "try.h"

void
look_up_accts( bool check_mode, char *inst_acct, int *uidp, int *gidp, char **dirpp, char **stu_acct, char **stu_name, int *try_uid, int *try_gid )
{
	reg	struct	passwd	*pp;
	reg	char	*start, *last;

	if( check_mode || (pp = getpwnam( TRY_ACCT )) == NULL )
		*try_uid = *try_gid = TRY_UID;
	else {
		*try_uid = pp->pw_uid;
		*try_gid = pp->pw_gid;
	}

	setpwent();
	if( (pp = getpwnam( inst_acct )) == NULL )
		error( ERR_USER + 1, "Instructor account \"%s\" does not exist.\n", inst_acct );
		/*NOTREACHED*/
	*uidp = pp->pw_uid;
	*gidp = pp->pw_gid;
	*dirpp = strsave( pp->pw_dir );

	setpwent();
	if( (pp = getpwuid( getuid() )) == NULL )
		error( ERR_IMPOS + 1, "Uid %d (that's you!) is not in the password file.\n",
			getuid() );
		/*NOTREACHED*/
	*stu_acct = strsave( pp->pw_name );

	if( check_mode && strne( inst_acct, *stu_acct ) )
		error( ERR_USER + 1, "Invalid option: -c\n" );
		/*NOTREACHED*/

	last = start = pp->pw_gecos;
	while( *last != '\0' )
		if( *last == ';' )
			*last = '\0';
		else
			last++;
	*stu_name = strsave( start );
	endpwent();
}
