/**
 * @file srv-prg/src/prg/c/src/swocserver/main.c
 *
 * Server Wait On Clients server program.
 * To enable a server to wait on clients releasing flags prior to further server
 * processing. (By use of swocserver -w).
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 19/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 16/05/2016	MG	1.0.2	Add --force-unlock option.		*
 *				Make --status and --wait ignore		*
 *				directories.				*
 * 28/05/2016	MG	1.0.3	Modified following introduction of	*
 *				libswocserver library.			*
 * 13/06/2016	MG	1.0.4	Use new more unique library function	*
 *				names.					*
 * 17/07/2016	MG	1.0.5	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.6	Further coding style changes.		*
 *				Improve in-source documentation.	*
 *				Enable cmdlineargs support for multiple	*
 *				command line programs in a single	*
 *				project.				*
 *				Use more informative name - sws_err.	*
 * 06/05/2017	MG	1.1.0	Migrate from NFS 'file as a flag' 	*
 *				semaphore to TCP socket stream		*
 *				messaging between applications and	*
 *				daemon.					*
 *				Add end_daemon option to command line	*
 *				program.				*
 * 12/09/2017	MG	1.1.1	Change 'force unlock' to just 'unlock'.	*
 * 17/11/2017	MG	1.1.2	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 19/11/2017	MG	1.1.3	Make program exit with EXIT_SUCCESS or	*
 *				EXIT_FAILURE only.			*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>

#include <mge-errno.h>
#include <cmdlineargs.h>
#include <signalhandle.h>
#include <swocserver.h>

char *prog_name;	/**< This program's name. */
int sws_err;		/**< Global swocserver error flag. */

/**
 * Program entry point.
 * @param argc Standard CLA argc.
 * @param argv Standard CLA argv.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv)
{
	prog_name = argv[0];
	sws_err = 0;

	/* Initialise signal handling. */
	init_sig_handle();

	/* Command line argument flags. */
	struct cla end_flag = { 0, "" };
	struct cla reload_flag = { 0, "" };
	struct cla status_flag = { 0, "" };
	struct cla unlock_flag = { 0, "" };
	struct cla wait_flag = { 0, "" };

	/* Process command line. */
	if (process_cla(argc, argv, &end_flag, &reload_flag, &status_flag,
			&unlock_flag, &wait_flag))
		exit(EXIT_FAILURE);

	/* Invoke main processing. */
	if (end_flag.is_set) {
		sws_err = sws_end_daemon();
		if (!sws_err)
			printf("Request to end daemon acknowledged.\n");
	}
	else if (reload_flag.is_set) {
		sws_err = sws_reload_config();
		if (!sws_err)
			printf("Daemon reloaded it's config file.\n");
	}
	else if (status_flag.is_set) {
		sws_err = sws_show_status();
		/* sws_show_status() returns -1 on error. */
		if (sws_err > 0)
			sws_err = 0;
	}
	else if (unlock_flag.is_set) {
		sws_err = sws_unlock(unlock_flag.argument);
		if (!sws_err)
			printf("Lock removed manually from server - %s.\n",
				unlock_flag.argument);
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