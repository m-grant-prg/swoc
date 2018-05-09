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

#include <libswocserver.h>


/*
 * Main
 */
int main(int argc, char **argv)
{
	int retval = 0;

	retval = sws_show_status();
	printf("retval after sws_show_status is %i\n", retval);

	retval = sws_release("localhost");
	printf("retval after sws_release of localhost is %i\n", retval);
	sws_show_status();

	retval = sws_server_wait();
	printf("retval after sws_server_wait is %i\n", retval);
	sws_show_status();

	retval = sws_reload_config();
	printf("retval after sws_reload_config is %i\n", retval);
	sws_show_status();

	retval = sws_cli_block("localhost");
	printf("retval after sws_cli_block is %i\n", retval);
	sws_show_status();

	retval = sws_cli_unblock("localhost");
	printf("retval after sws_cli_unblock is %i\n", retval);
	sws_show_status();

	//retval = sws_end_daemon();

	//printf("retval after sws_end_daemon is %i\n", retval);

	libswocserver_print_pkg_version();
	libswocserver_print_src_version();

	exit(retval);
}
