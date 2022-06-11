/**
 * @file src/prg/c/src/cli-lib/libswocclient/optionproc.c
 *
 * Functions to process client lock flag options.
 *
 * @author Copyright (C) 2016-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.16 ==== 11/06/2022_
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
 * 02/05/2018	MG	1.1.6	Add support for clinet blocked list.	*
 * 18/05/2018	MG	1.1.7	Add show server block status.		*
 * 05/08/2018	MG	1.1.8	Change mgemessage struct initialisation	*
 *				following .complete field change to	*
 *				type bool.				*
 * 07/09/2018	MG	1.1.9	Use new mgemessage struct initialiser.	*
 * 18/05/2019	MG	1.1.10	Merge sub-projects into one.		*
 * 02/06/2019	MG	1.1.11	Add explicit cast to pollint for sign	*
 *				conversion. It has already been		*
 *				validated > 0.				*
 * 08/11/2019	MG	1.1.12	Use standard GNU ifdeffery around use	*
 *				of AC_HEADER_STDBOOL.			*
 * 10/10/2021	MG	1.1.13	Use new internal common header.		*
 * 08/12/2021	MG	1.1.14	Tighten SPDX tag.			*
 * 01/04/2022	MG	1.1.15	Improve error handling consistency.	*
 * 11/06/2022	MG	1.1.16	Replace sprintf with safer snprintf.	*
 *									*
 ************************************************************************
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

/* Standard GNU AC_HEADER_STDBOOL ifdeffery. */
#ifdef HAVE_STDBOOL_H
	#include <stdbool.h>
#else
	#ifndef HAVE__BOOL
		#ifdef __cplusplus /* clang-format off */
			typedef bool _Bool; /* clang-format on */
		#else
			#define _Bool signed char
		#endif
	#endif
	#define bool _Bool
	#define false 0
	#define true 1
	#define __bool_true_false_are_defined 1
#endif

#include <libmgec.h>
#include <libswocclient.h>
#include <libswoccommon.h>
#include <mge-errno.h>
#include <mgemessage.h>
#include <remsyslog.h>

/**
 * Holds the number of locks currently held during swc_client_wait(). This
 * value can be accessed in a handler if a signal is received.
 */
char locks_held[10] = "0";

/**
 * Display client's lock status.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_show_status(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "status"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	printf("Client has %s locks on server.\n", *(msg->argv + 3));
	if (!strcmp(msg->argv[4], "0"))
		printf("Client is not blocked on server.\n");
	else
		printf("Client is blocked on server.\n");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Display status of server blocking.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_show_srv_block_status(void)
{
	int ret;
	long int x;
	char *end;
	char *out_msg = "swocclient,blockstatus;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(out_msg, om_length, msg);
	if (ret)
		return ret;

	if (strncmp(msg->message, "swocserverd,blockstatus,ok", 26)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "blockstatus"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	if (strcmp(msg->argv[3], "0"))
		printf("Server is blocked.\n");
	else
		printf("Server is not blocked.\n");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Set block flag on server to prevent this client from setting any more locks.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_block(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,block;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strcmp(msg->message, "swocserverd,block,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "block"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	syslog((int)(LOG_USER | LOG_NOTICE), "Server block flag is set.");
	sndremsyslogmsg(server, "swocclient", "Server block flag is set.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Remove block flag on server to allow this client to set locks.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_unblock(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,unblock;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strcmp(msg->message, "swocserverd,unblock,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "unblock"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	syslog((int)(LOG_USER | LOG_NOTICE), "Server block flag removed.");
	sndremsyslogmsg(server, "swocclient", "Server block flag removed.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Set lock flag on server.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_set_lock(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,lock;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strcmp(msg->message, "swocserverd,lock,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "lock"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	syslog((int)(LOG_USER | LOG_NOTICE), "Server lock flag is set.");
	sndremsyslogmsg(server, "swocclient", "Server lock flag is set.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Release lock flag on server.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_rel_lock(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,release;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strcmp(msg->message, "swocserverd,release,ok;")) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "release"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	syslog((int)(LOG_USER | LOG_NOTICE), "Server lock flag released.");
	sndremsyslogmsg(server, "swocclient", "Server lock flag released.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Wait until only a maximum of cnumlocks for this client remains.
 * If cnumlocks > 0 this realistically means that a previous command in this
 * sequence would have been a lock request.
 * On error mge_errno will be set.
 * @param cnumlocks Wait until the number of locks is <= this value. This value
 * must be 0 or 1.
 * @return 0 on success, < zero on failure.
 */
int swc_client_wait(char *cnumlocks)
{
	int ret;
	long int x;
	char *end;
	long int locks, numlocks;
	char *outgoing_msg = "swocclient,status;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	numlocks = strtol(cnumlocks, &end, 10);
	if ((*end != '\0') || (numlocks < 0) || (numlocks > 1)) {
		mge_errno = MGE_PARAM;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Invalid number of locks "
		       "specified for wait - %s",
		       cnumlocks);
		return -mge_errno;
	}

	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Waiting until no more than %li "
	       "locks remain for this client.",
	       numlocks);

	ret = swcom_validate_config();
	if (ret)
		return ret;
	do {
		sleep((unsigned int)pollint);
		if ((ret = exch_msg(outgoing_msg, om_length, msg)))
			return ret;

		if (strncmp(msg->message, "swocserverd,status,ok", 21)) {
			mge_errno = MGE_INVAL_MSG;
			if (msg->argc == 4) {
				if (!(strcmp(msg->argv[0], "swocserverd"))
				    && !(strcmp(msg->argv[1], "status"))
				    && !(strcmp(msg->argv[2], "err"))) {
					x = strtol(msg->argv[3], &end, 10);
					if ((*end == '\0') && x <= INT_MAX
					    && x >= INT_MIN)
						mge_errno = (int)x;
				}
			}
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Invalid message "
			       "- %s",
			       msg->message);
			clear_msg(msg, ';', ',');
			return -mge_errno;
		}
		locks = strtol(msg->argv[3], &end, 10);
		if (*end != '\0') {
			mge_errno = MGE_INVAL_MSG;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Invalid message "
			       "- %s",
			       msg->message);
			clear_msg(msg, ';', ',');
			return -mge_errno;
		}
		snprintf(locks_held, ARRAY_SIZE(locks_held), "%li", locks);

		clear_msg(msg, ';', ',');
	} while (locks > numlocks);
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "%li locks remain for this "
	       "client.",
	       locks);
	return 0;
}

/**
 * Reset the client on the server to 0 locks and unblocked.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int swc_reset(void)
{
	int ret;
	long int x;
	char *end;
	char *outgoing_msg = "swocclient,reset;";
	size_t om_length = strlen(outgoing_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	ret = swcom_validate_config();
	if (ret)
		return ret;

	ret = exch_msg(outgoing_msg, om_length, msg);
	if (ret)
		return ret;

	if (strncmp(msg->message, "swocserverd,reset,ok", 20)) {
		mge_errno = MGE_INVAL_MSG;
		if (msg->argc == 4) {
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "reset"))
			    && !(strcmp(msg->argv[2], "err"))) {
				x = strtol(msg->argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       msg->message);
		clear_msg(msg, ';', ',');
		return -mge_errno;
	}

	printf("%s Client locks removed on server.\n", *(msg->argv + 3));
	if (!strcmp(msg->argv[4], "0"))
		printf("Client was not blocked on server.\n");
	else
		printf("Client block on server removed.\n");

	clear_msg(msg, ';', ',');

	return 0;
}
