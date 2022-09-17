/* **********************************************************************
 *									*
 * Source: testoptionproc.c						*
 * Author: Mark Grant							*
 *									*
 * Purpose:								*
 *	To test the options functions.					*
 * 									*
 * Released under the GPLv3 only.					*
 * SPDX-License-Identifier: GPL-3.0-only				*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include <swoc/libswocclient.h>

/*
 * Main
 */
int main(void)
{
	int prg_err;

	/* Initial status */
	prg_err = swc_show_status();
	printf("prg_err after swc_show_status is %i\n", prg_err);

	/* Set and release lock and wait(0) test */
	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_rel_lock();
	printf("prg_err after swc_rel_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_client_wait("0");
	printf("prg_err after swc_client_wait(0) with no locks is %i\n",
	       prg_err);

	/* Set, block, set, release, unblock */
	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_block();
	printf("prg_err after swc_block is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_rel_lock();
	printf("prg_err after swc_rel_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_unblock();
	printf("prg_err after swc_unblock is %i\n", prg_err);
	swc_show_status();

	/* lock wait(1) */
	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_client_wait("1");
	printf("prg_err after swc_client_wait(1) with 1 lock is %i\n", prg_err);

	/* lock wait(0) should be 2 locks here so manual intervention req */
	prg_err = swc_set_lock();
	printf("prg_err after swc_set_lock is %i\n", prg_err);
	swc_show_status();

	prg_err = swc_client_wait("0");
	printf("prg_err after swc_client_wait(0) with 2 locks is %i\n",
	       prg_err);

	swc_show_status();

	/* lock, lock, block, reset */
	swc_set_lock();
	swc_set_lock();
	swc_block();
	swc_show_status();

	prg_err = swc_reset();
	printf("prg_err after swc_reset is %i\n", prg_err);
	swc_show_status();

	libswocclient_print_pkg_version();
	libswocclient_print_src_version();

	exit(prg_err);
}
