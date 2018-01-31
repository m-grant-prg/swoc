/* **********************************************************************
 *									*
 * Source: testoptionproc.c						*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	To test the options functions.					*
 * 									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0					*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>

#include <libswocclient.h>


/*
 * Main
 */
int main(int argc, char **argv)
{
	int prg_err = 0;

	prg_err = swc_show_status();
	printf("prg_err after swc_show_status is %i\n", prg_err);

	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);

	swc_show_status();
	prg_err = swc_rel_lock();
	printf("prg_err after swc_rel_lock is %i\n", prg_err);

	swc_show_status();
	prg_err = swc_client_wait();
	printf("prg_err after swc_client_wait with no locks is %i\n", prg_err);

	swc_set_lock();
	swc_show_status();
	prg_err = swc_client_wait();
	printf("prg_err after swc_client_wait with 1 lock is %i\n", prg_err);

	swc_set_lock();
	swc_show_status();
	prg_err = swc_client_wait();
	printf("prg_err after swc_client_wait with 2 locks is %i\n", prg_err);

	swc_show_status();
	prg_err = swc_rel_lock();
	printf("prg_err after swc_rel_lock is %i\n", prg_err);

	libswocclient_print_pkg_version();
	libswocclient_print_src_version();

	exit(prg_err);
}
