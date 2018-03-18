/**
 * @file cli-lib/src/prg/c/src/libswocclient/optionproc.c
 *
 * Functions to process client lock flag options.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.5 ==== 18/03/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 11/06/2016	MG	1.0.1	First release.Separated out from	*
 *				swocclient-c package.			*
 * 12/06/2016	MG	1.0.2	Adopt convention of function definition	*
 *				with empty parameter list to use void,	*
 *				just as with prototypes.		*
 * 16/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 17/07/2016	MG	1.0.4	Further coding style changes.		*
 * 06/05/2017	MG	1.1.0	Migrate from NFS 'file as a flag'	*
 *				semaphore to TCP stream socket		*
 *				messaging to new swocserverd daemon.	*
 *				Add new swc_client_wait function.	*
 *				Migrate to use of mge_errno and libmgec	*
 *				error handling.				*
 * 11/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 31/01/2018	MG	1.1.2	Daemon return message now standardised.	*
 *				On error use the error number in the	*
 *				return message to populate mge_errno.	*
 * 01/02/2018	MG	1.1.3	On error path check argc == 4 before	*
 *				indexing into argv.			*
 * 02/02/2018	MG	1.1.4	Use safer strtol instead of atoi.	*
 *				Add number of locks as parameter to	*
 *				the wait function.			*
 * 18/03/2018	MG	1.1.5	Store the number of locks currently	*
 *				held during swc_client_wait() in a	*
 *				global variable. This value can be	*
 *				accessed in a handler if a signal is	*
 *				received.				*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>

#include <mge-errno.h>
#include <mgemessage.h>
#include <libswoccommon.h>
#include <remsyslog.h>
#include <libswocclient.h>


/**
 * Holds the number of locks currently held during swc_client_wait(). This
 * value can be accessed in a handler if a signal is received.
 */
char locks_held[10] = "0";


/**
 * Display client's lock status.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int swc_show_status(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	if ((prg_err = swcom_validate_config()))
		return prg_err;

	if ((prg_err = exch_msg(outgoing_msg, om_length, msg)))
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
		return mge_errno;
	}

	printf("Client has %s locks on server.\n", *(msg->argv + 3));

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Set lock flag on server.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int swc_set_lock(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,lock;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	if ((prg_err = swcom_validate_config()))
		return prg_err;

	if ((prg_err = exch_msg(outgoing_msg, om_length, msg)))
		return prg_err;

	if (strcmp(msg->message, "swocserverd,lock,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd")) &&
				!(strcmp(msg->argv[1], "lock")) &&
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

	syslog((int) (LOG_USER | LOG_NOTICE), "Server lock flag is set.");
	sndremsyslogmsg(server, "swocclient", "Server lock flag is set.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Release lock flag on server.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int swc_rel_lock(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,release;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	if ((prg_err = swcom_validate_config()))
		return prg_err;

	if ((prg_err = exch_msg(outgoing_msg, om_length, msg)))
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

	syslog((int) (LOG_USER | LOG_NOTICE), "Server lock flag released.");
	sndremsyslogmsg(server, "swocclient", "Server lock flag released.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Wait until only a maximum of cnumlocks for this client remains.
 * If cnumlocks > 0 this realistically means that a previous command in this
 * sequence would have been a lock request.
 * On error mge_errno will be set.
 * @param cnumlocks Wait until the number of locks is <= this value.
 * @return 0 on success, non-zero on failure.
 */
int swc_client_wait(char *cnumlocks)
{
	int prg_err = 0;
	long int x;
	char *end;
	long int locks, numlocks;
	char *outgoing_msg = "swocclient,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	numlocks = strtol(cnumlocks, &end, 10);
	if ((*end != '\0') || (numlocks < 0) || (numlocks > 1)) {
		mge_errno = MGE_PARAM;
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid number of locks "
			"specified for wait - %s", cnumlocks);
		return mge_errno;
	}

	syslog((int) (LOG_USER | LOG_NOTICE), "Waiting until no more than %li "
		"locks remain for this client.", numlocks);

	if ((prg_err = swcom_validate_config()))
		return prg_err;
	do {
		sleep(pollint);
		if ((prg_err = exch_msg(outgoing_msg, om_length, msg)))
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
		locks = strtol(msg->argv[3], &end, 10);
		if (*end != '\0') {
			mge_errno = MGE_INVAL_MSG;
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message "
				"- %s", msg->message);
			clear_msg(msg, ';', ',');
			return mge_errno;
		}
		sprintf(locks_held, "%li", locks);

		clear_msg(msg, ';', ',');
	} while (locks > numlocks);
	syslog((int) (LOG_USER | LOG_NOTICE), "%li locks remain for this "
		"client.", locks);
	return 0;
}
