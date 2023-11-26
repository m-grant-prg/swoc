/**
 * @file src/prg/c/src/cli-prg/main.c
 *
 * Server Wait On Clients client program.
 * To enable a client to place lock flags on a server which will then wait for
 * them to be cleared before continuing processing, (by use of swocserver -w).
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 26/11/2023_
 */

#include <stdio.h>
#include <stdlib.h>

#include <libmgec/mge-errno.h>
#include <swoc/cmdlineargs.h>
#include <swoc/libswocclient.h>
#include <swoc/signalhandle.h>

/** The name of this program. */
static char *prog_name;

/**
 * Program entry point.
 * @param argc Standard CLA argc.
 * @param argv Standard CLS argv.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv)
{
	int ret;

	/* Command line argument flags. */
	struct cla block_flag = { 0, "" };
	struct cla lock_flag = { 0, "" };
	struct cla release_flag = { 0, "" };
	struct cla reset_flag = { 0, "" };
	struct cla status_flag = { 0, "" };
	struct cla unblock_flag = { 0, "" };
	struct cla wait_flag = { 0, "" };

	prog_name = argv[0];

	/* Initialise signal handling. */
	init_sig_handle();

	/* Process command line. */
	ret = process_cla(argc, argv, &block_flag, &lock_flag, &release_flag,
			  &reset_flag, &status_flag, &unblock_flag, &wait_flag);
	if (ret)
		exit(EXIT_FAILURE);

	/* Invoke main processing. */
	if (status_flag.is_set) {
		ret = swc_show_status();
		if (!ret)
			ret = swc_show_srv_block_status();
	} else if (block_flag.is_set) {
		ret = swc_block();
		if (!ret)
			printf("Client blocked on server.\n");
	} else if (lock_flag.is_set) {
		ret = swc_set_lock();
		if (!ret)
			printf("Lock flag set on server.\n");
	} else if (release_flag.is_set) {
		ret = swc_rel_lock();
		if (!ret)
			printf("Lock flag released on server.\n");
	} else if (reset_flag.is_set) {
		ret = swc_reset();
	} else if (unblock_flag.is_set) {
		ret = swc_unblock();
		if (!ret)
			printf("Client blocking removed on server.\n");
	} else if (wait_flag.is_set) {
		printf("Waiting for maximum of %s lock flags on server.\n",
		       wait_flag.argument);
		ret = swc_client_wait(wait_flag.argument);
		if (!ret)
			printf("Maximum of %s lock flags set on server.\n",
			       wait_flag.argument);
	}

	if (ret) {
		fprintf(stderr, "%s failed with error - %s\n", prog_name,
			mge_strerror(mge_errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
