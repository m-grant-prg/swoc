/**
 * @file srv-lib/source/progs/c/src/libswocserver/optionproc.c
 *
 * Functions to process server lock flag options.
 *
 * @author Copyright (C) 2016-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 12/11/2017_
 */

 /***********************************************************************
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
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>

#include <mge-errno.h>
#include <mgemessage.h>
#include <libswoccommon.h>
#include <libswocserver.h>


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
	char *outgoing_msg = "swocserver,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(outgoing_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
		mge_errno = MGE_INVAL_MSG;
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
 * Wait until no client locks remain.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_server_wait(void)
{
	int prg_err = 0;
	int empty;
	char *outgoing_msg = "swocserver,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	syslog((int) (LOG_USER | LOG_NOTICE), "Waiting for any client locks to "
		"be released.\n");

	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;
	do {
		sleep(pollint);
		prg_err = exch_msg(outgoing_msg, om_length, msg);
		if (prg_err)
			return prg_err;

		if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
			mge_errno = MGE_INVAL_MSG;
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message "
				"- %s", msg->message);
			clear_msg(msg, ';', ',');
			return mge_errno;
		}
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
int sws_unlock(char *lockname)
{
	int prg_err = 0;
	char outgoing_msg[17 + strlen(lockname) + 1];
	sprintf(outgoing_msg, "swocserver,unlock,%s;", lockname);
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(outgoing_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strncmp(msg->message, "swocserverd,unlock,ok", 20)) {
		mge_errno = MGE_INVAL_MSG;
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
 * Terminate the daemon.
 * Send a message to the daemon asking it to terminate.
 * On error mge_errno is set.
 * @return 0 on success, non-zero on failure.
 */
int sws_end_daemon(void)
{
	int prg_err = 0;
	char *outgoing_msg = "swocserver,end;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(outgoing_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,end,ok;")) {
		mge_errno = MGE_INVAL_MSG;
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
	char *outgoing_msg = "swocserver,reload;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;

	prg_err = exch_msg(outgoing_msg, om_length, msg);
	if (prg_err)
		return prg_err;

	if (strcmp(msg->message, "swocserverd,reload,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message - %s",
			msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Config file reloaded.");

	clear_msg(msg, ';', ',');

	return 0;
}
