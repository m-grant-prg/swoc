/**
 * @file src/prg/c/src/srv-prg/swocserver/main.c
 *
 * Server Wait On Clients server program.
 * To enable a server to wait on clients releasing flags prior to further server
 * processing. (By use of swocserver -w).
 *
 * @author Copyright (C) 2015-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 26/11/2023_
 */

#include <stdio.h>
#include <stdlib.h>

#include "internal.h"
#include <libmgec/mge-errno.h>
#include <swoc/cmdlineargs.h>
#include <swoc/libswocserver.h>
#include <swoc/signalhandle.h>

static char *prog_name; /**< This program's name. */
int sws_err;		/**< Global swocserver error flag. */

/**
 * Program entry point.
 * @param argc Standard CLA argc.
 * @param argv Standard CLA argv.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv)
{
	/* Command line argument flags. */
	struct cla allow_flag = { 0, "" };
	struct cla block_flag = { 0, "" };
	struct cla disallow_flag = { 0, "" };
	struct cla end_flag = { 0, "" };
	struct cla release_flag = { 0, "" };
	struct cla reload_flag = { 0, "" };
	struct cla status_flag = { 0, "" };
	struct cla unblock_flag = { 0, "" };
	struct cla wait_flag = { 0, "" };

	prog_name = argv[0];
	sws_err = 0;

	/* Initialise signal handling. */
	init_sig_handle();

	/* Process command line. */
	if (process_cla(argc, argv, &allow_flag, &block_flag, &disallow_flag,
			&end_flag, &release_flag, &reload_flag, &status_flag,
			&unblock_flag, &wait_flag))
		exit(EXIT_FAILURE);

	/* Invoke main processing. */
	if (allow_flag.is_set) {
		sws_err = sws_srv_unblock();
		if (!sws_err)
			printf("Server is unblocked.\n");
	} else if (block_flag.is_set) {
		sws_err = sws_cli_block(block_flag.argument);
		if (!sws_err)
			printf("Client %s blocked.\n", block_flag.argument);
	} else if (disallow_flag.is_set) {
		sws_err = sws_srv_block();
		if (!sws_err)
			printf("Server is blocked.\n");
	} else if (end_flag.is_set) {
		sws_err = sws_end_daemon();
		if (!sws_err)
			printf("Request to end daemon acknowledged.\n");
	} else if (release_flag.is_set) {
		sws_err = sws_release(release_flag.argument);
		if (!sws_err)
			printf("Lock removed manually from server - %s.\n",
			       release_flag.argument);
	} else if (reload_flag.is_set) {
		sws_err = sws_reload_config();
		if (!sws_err)
			printf("Daemon reloaded it's config file.\n");
	} else if (status_flag.is_set) {
		sws_err = sws_show_status();
		/* sws_show_status() returns -1 on error. */
		if (sws_err > 0)
			sws_err = 0;
		printf("\n");
		if (!sws_err)
			sws_err = sws_show_cli_blocklist();
		if (sws_err > 0)
			sws_err = 0;
		printf("\n");
		if (!sws_err)
			sws_err = sws_show_block_status();
		if (sws_err > 0)
			sws_err = 0;
	} else if (unblock_flag.is_set) {
		sws_err = sws_cli_unblock(unblock_flag.argument);
		if (!sws_err)
			printf("Client %s unblocked.\n", unblock_flag.argument);
	} else {
		printf("Waiting for any client locks to be released.\n");
		sws_err = sws_server_wait();
		if (!sws_err)
			printf("No outstanding client locks on this server.\n");
	}

	if (sws_err)
		fprintf(stderr, "%s failed with error - %s\n", prog_name,
			mge_strerror(mge_errno));

	if (sws_err)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}
