/**
 * @file cli-lib/source/progs/c/src/libswocclient/optionproc.c
 *
 * Functions to process client lock flag options.
 *
 * @author Copyright (C) 2016-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.1 ==== 11/11/2017_
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
 *									*
 ************************************************************************
 */


#include <stdio.h>
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
 * Display client's lock status.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int swc_show_status(void)
{
	int prg_err = 0;
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
 * Wait until only 1 lock for this client remains.
 * Realistically this means that a previous command in this sequence would have
 * been a lock request.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int swc_client_wait(void)
{
	int prg_err = 0;
	int locks;
	char *outgoing_msg = "swocclient,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;


	syslog((int) (LOG_USER | LOG_NOTICE), "Waiting until 1 or fewer locks "
		"remain for this client.");

	if ((prg_err = swcom_validate_config()))
		return prg_err;
	do {
		sleep(pollint);
		if ((prg_err = exch_msg(outgoing_msg, om_length, msg)))
			return prg_err;

		if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
			mge_errno = MGE_INVAL_MSG;
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message "
				"- %s", msg->message);
		clear_msg(msg, ';', ',');
		return mge_errno;
		}
		locks = atoi(*(msg->argv + 3));
		clear_msg(msg, ';', ',');
	} while (locks > 1);
	syslog((int) (LOG_USER | LOG_NOTICE), "%i locks remain for this "
		"client.", locks);
	return 0;
}
