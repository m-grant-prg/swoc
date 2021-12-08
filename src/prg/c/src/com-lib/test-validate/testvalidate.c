/* **********************************************************************
 *									*
 * Source: testvalidate.c						*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	To test the validate config functions.				*
 *									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0-only				*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include <libswoccommon.h>

/*
 * Main
 */
int main(void)
{
	int retval = 0;

	retval = swcom_validate_config();

	printf("retval after validate is %i\n", retval);

	printf("pollint is:- %i\n", pollint);

	printf("SSH is:- %i\n", ssh);

	printf("server name is:- %s\n", server);

	printf("srvportno is:- %i\n", srvportno);

	printf("sshportno is:- %i\n", sshportno);

	printf("user name is:- %s\n", sshuser);

	printf("\n");
	libswoccommon_print_src_version();
	libswoccommon_print_pkg_version();

	exit(retval);
}
