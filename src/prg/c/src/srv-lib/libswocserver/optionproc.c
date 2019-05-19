/**
 * @file src/prg/c/src/srv-lib/libswocserver/optionproc.c
 *
 * Functions to process server lock flag options.
 *
 * @author Copyright (C) 2016-2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.10 ==== 18/05/2019_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 25/05/2016	MG	1.0.1	First release.Separated out from	*
 *				swocserver-c package.			*
 * 12/06/2016	MG	1.0.2	Make publicly visible function names	*
 *				more unique.				*
 * 16/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 17/07/2016	MG	1.0.4	Further coding style changes.		*
 * 06/05/2017	MG	1.1.0	Convert from NFS 'file as a flag'	*
 *				semaphore to TCP socket messaging to	*
 *				new server daemon.			*
 *				Migrate to use of mge_errno error	*
 *				handling from libmgec.			*
 *				Print the number of clients with locks	*
 *				in sws_show_status().			*
 * 12/09/2017	MG	1.1.1	Change sws_force_unlock() to		*
 *				sws_unlock().				*
 * 15/09/2017	MG	1.1.2	Change refernces to ssl to tls.		*
 * 12/11/2017	MG	1.1.3	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 01/02/2018	MG	1.1.4	Daemon return message now standardised.	*
 *				On error use the error number in the	*
 *				return message to populate mge_errno.	*
 * 28/03/2018	MG	1.1.5	Ensure variables are declared before	*
 *				code, (fixes a sparse warning).		*
 * 29/03/2018	MG	1.1.6	Store the current number of clients	*
 *				holding locks during sws_server_wait()	*
 *				in a global variable. This value can be	*
 *				accessed in a handler if a signal is	*
 *				received.				*
 * 10/05/2018	MG	1.1.7	Improve function name consistency,	*
 *				unlock -> release.			*
 *				Add support for server listing blocked	*
 *				clients.				*
 *				Add client block and unblock.		*
 *				Add server block and unblock.		*
 * 05/08/2018	MG	1.1.8	Change mgemessage struct initialisation	*
 *				following field complete change to type	*
 *				bool.					*
 * 07/09/2018	MG	1.1.9	Use new mgemessage struct initialiser.	*
 * 18/05/2019	MG	1.1.10	Merge sub-projects into one.		*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdbool.h>

#include <mge-errno.h>
#include <mgemessage.h>
#include <libswoccommon.h>
#include <libswocserver.h>


/**
 * Holds the number of clients currently holding locks during sws_server_wait().
 * This value can be accessed in a handler if a signal is received.
 */
char locks_held[10] = "0";


/**
 * Display clients with active locks to stdout.
 * On error mge_errno is set.
 * @return The number of clients with locks or -1 on error.
 */
int sws_show_status(void)
{
	int c = 0;
	int i;
	int prg_err;
	long int x;
	char *end;
	char *out_msg = "swocserver,status;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "status")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return -1;
	}

	for (i = 3; i < msg->argc; i += 2) {
		c++;
		printf("Client %s has %s locks.\n", *(msg->argv + i),
			*(msg->argv + i + 1));
	}
	printf("%i clients have locks on this server.\n", c);

	clear_msg(msg, ';', ',');

	/* Return number of clients with locks. */
	return c;
}

/**
 * Display status of server blocking.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_show_block_status(void)
{
	int prg_err;
	long int x;
	char *end;
	char *out_msg = "swocserver,blockstatus;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strncmp(msg->message, "swocserverd,blockstatus,ok", 26)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "blockstatus")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return -1;
	}

	if (strcmp(msg->argv[3], "0"))
		printf("Server is blocked.\n");
	else
		printf("Server is not blocked.\n");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Request server blocking.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_srv_block(void)
{
	int prg_err;
	long int x;
	char *end;
	char *out_msg = "swocserver,disallow;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,disallow,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "disallow")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return -1;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Server is blocked.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Request removal of server blocking.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_srv_unblock(void)
{
	int prg_err;
	long int x;
	char *end;
	char *out_msg = "swocserver,allow;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,allow,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "disallow")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return -1;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Server is unblocked.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Display list of blocked clients to stdout.
 * On error mge_errno is set.
 * @return The number of blocked or -1 on error.
 */
int sws_show_cli_blocklist(void)
{
	int c = 0;
	int i;
	int prg_err;
	long int x;
	char *end;
	char *out_msg = "swocserver,blocklist;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strncmp(msg->message, "swocserverd,blocklist,ok", 24)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "blocklist")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return -1;
	}

	for (i = 3; i < msg->argc; i++) {
		c++;
		printf("Client %s is blocked.\n", *(msg->argv + i));
	}
	printf("%i clients are blocked on this server.\n", c);

	clear_msg(msg, ';', ',');

	/* Return number of blocked clients. */
	return c;
}

/**
 * Wait until no client locks remain.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_server_wait(void)
{
	int prg_err = 0;
	int empty;
	long int x;
	char *end;
	char *out_msg = "swocserver,status;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	syslog((int) (LOG_USER | LOG_NOTICE), "Waiting for any client locks to "
		"be released.\n");

	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;
	do {
		sleep(pollint);
		prg_err = exch_msg(out_msg, om_length, msg);
		if (prg_err)
			return prg_err;

		if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
			mge_errno = MGE_INVAL_MSG;
			if (msg->argc == 4) {
				if (!(strcmp(msg->argv[0], "swocserverd")) &&
					!(strcmp(msg->argv[1], "status")) &&
				    	!(strcmp(msg->argv[2], "err"))) {
					x = strtol(msg->argv[3], &end, 10);
					if ((*end == '\0') && x <= INT_MAX &&
						x >= INT_MIN)
						mge_errno = (int)x;
				}
			}
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message "
				"- %s", msg->message);
			clear_msg(msg, ';', ',');
			return mge_errno;
		}
		if (msg->argc > 3)
			sprintf(locks_held, "%i", ((msg->argc -3) / 2));
		empty = strcmp(msg->message, "swocserverd,status,ok;");
		clear_msg(msg, ';', ',');
	} while (empty);
	/* Allow one more sleep as a buffer, maybe client is shutting down. */
	sleep(pollint);
	syslog((int) (LOG_USER | LOG_NOTICE), "No outstanding client locks on "
		"this server.");
	return 0;
}

/**
 * Remove a lock.
 * On error mge_errno is set.
 * @param lockname Client whose lock is to be removed.
 * @return 0 on success, non-zero on failure.
 */
int sws_release(char *lockname)
{
	int prg_err = 0;
	long int x;
	char *end;
	char out_msg[20 + strlen(lockname) + 1];
	size_t om_length;
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	sprintf(out_msg, "swocserver,release,%s;", lockname);
	om_length = strlen(out_msg);

	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,release,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "release")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Lock removed manually from "
		"server - %s.", lockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Set a client to blocked.
 * On error mge_errno is set.
 * @param blockname Client to block.
 * @return 0 on success, non-zero on failure.
 */
int sws_cli_block(char *blockname)
{
	int prg_err = 0;
	long int x;
	char *end;
	char out_msg[18 + strlen(blockname) + 1];
	size_t om_length;
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	sprintf(out_msg, "swocserver,block,%s;", blockname);
	om_length = strlen(out_msg);


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,block,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "block")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s blocked.", blockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Unblock a client block.
 * On error mge_errno is set.
 * @param blockname Client to unblock.
 * @return 0 on success, non-zero on failure.
 */
int sws_cli_unblock(char *blockname)
{
	int prg_err = 0;
	long int x;
	char *end;
	char out_msg[20 + strlen(blockname) + 1];
	size_t om_length;
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	sprintf(out_msg, "swocserver,unblock,%s;", blockname);
	om_length = strlen(out_msg);


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,unblock,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "unblock")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s unblocked.",
	       blockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Terminate the daemon.
 * Send a message to the daemon asking it to terminate.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_end_daemon(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	char *out_msg = "swocserver,end;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,end,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "end")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Request to end daemon "
		"acknowledged.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Request the daemon to reload the config file.
 * Send a message to the daemon asking it to reload it's configuration file.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_reload_config(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	char *out_msg = "swocserver,reload;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(out_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,reload,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "reload")) &&
			    	!(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX &&
					x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Config file reloaded.");

	clear_msg(msg, ';', ',');

	return 0;
}

