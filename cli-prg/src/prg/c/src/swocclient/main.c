/**
 * @file cli-prg/src/prg/c/src/swocclient/main.c
 *
 * Server Wait On Clients client program.
 * To enable a client to place lock flags on a server which will then wait for
 * them to be cleared before continuing processing, (by use of swocserver -w).
 *
 * @author Copyright (C) 2015-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 02/02/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 11/06/2016	MG	1.0.2	Modified following introduction of	*
 *				libswocclient library.			*
 * 13/06/2016	MG	1.0.3	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.4	Move towards kernel coding style.	*
 * 07/05/2017	MG	1.1.0	Migrate to use mge_errno and handling	*
 *				from libmgec. Add error message output	*
 *				to prevent silence on error.		*
 *				Add --wait as a command line argument.	*
 * 12/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 19/11/2017	MG	1.1.2	Make program exit with EXIT_SUCCESS or	*
 *				EXIT_FAILURE only.			*
 * 02/02/2018	MG	1.1.3	Allow wait to take an optional number	*
 *				of locks argument (default 0).		*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>

#include <mge-errno.h>
#include <swocclient.h>
#include <signalhandle.h>
#include <cmdlineargs.h>
#include <libswocclient.h>


/** The name of this program. */
char *prog_name;


/**
 * Program entry point.
 * @param argc Standard CLA argc.
 * @param argv Standard CLS argv.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv)
{
	prog_name = argv[0];

	/* Initialise signal handling. */
	init_sig_handle();

	int prog_error = 0;

	/* Command line argument flags. */
	struct cla lock_flag = { 0, "" };
	struct cla release_flag = { 0, "" };
	struct cla status_flag = { 0, "" };
	struct cla wait_flag = { 0, "" };


	/* Process command line. */
	if ((prog_error = process_cla(argc, argv, &lock_flag, &release_flag,
		&status_flag, &wait_flag)))
		exit(EXIT_FAILURE);

	/* Invoke main processing. */
	if (status_flag.is_set)
		prog_error = swc_show_status();
	else if (lock_flag.is_set) {
		prog_error = swc_set_lock();
		if (!prog_error)
			printf("Lock flag set on server.\n");
	}
	else if (release_flag.is_set) {
		prog_error = swc_rel_lock();
		if (!prog_error)
			printf("Lock flag released on server.\n");
	}
	else if (wait_flag.is_set) {
		printf("Waiting for maximum of %s lock flags on server.\n",
			wait_flag.argument);
		prog_error = swc_client_wait(wait_flag.argument);
		if (!prog_error)
			printf("Maximum of %s lock flags set on server.\n",
				wait_flag.argument);
	}

	if (prog_error) {
		fprintf(stderr, "%s failed with error - %s\n", prog_name,
				mge_strerror(mge_errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}