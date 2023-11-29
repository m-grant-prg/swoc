/**
 * @file src/prg/c/src/srv-lib/optionproc.c
 *
 * Functions to process server lock flag options.
 *
 * @author Copyright (C) 2016-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 26/11/2023_
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

#include <libmgec/libmgec.h>
#include <libmgec/mge-errno.h>
#include <libmgec/mge-message.h>
#include <swoc/libswoccommon.h>
#include <swoc/libswocserver.h>

/**
 * Holds the number of clients currently holding locks during sws_server_wait().
 * This value can be accessed in a handler if a signal is received.
 */
char locks_held[12] = "0";

/**
 * Display clients with active locks to stdout.
 * On error mge_errno is set.
 * @return The number of clients with locks or < zero on error.
 */
int sws_show_status(void)
{
	int c = 0;
	int i;
	int prg_err;
	long int x;
	char *end;
	const char *out_msg = "swocserver,status;";
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
 * @return 0 on success, < zero on failure.
 */
int sws_show_block_status(void)
{
	int prg_err;
	long int x;
	char *end;
	const char *out_msg = "swocserver,blockstatus;";
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
 * Request server blocking.
 * On error mge_errno is set.
 * @return 0 on success, < zero on failure.
 */
int sws_srv_block(void)
{
	int prg_err;
	long int x;
	char *end;
	const char *out_msg = "swocserver,disallow;";
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
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "disallow"))
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Server is blocked.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Request removal of server blocking.
 * On error mge_errno is set.
 * @return 0 on success, < zero on failure.
 */
int sws_srv_unblock(void)
{
	int prg_err;
	long int x;
	char *end;
	const char *out_msg = "swocserver,allow;";
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
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "disallow"))
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Server is unblocked.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Display list of blocked clients to stdout.
 * On error mge_errno is set.
 * @return The number of blocked or < zero on error.
 */
int sws_show_cli_blocklist(void)
{
	int c = 0;
	int i;
	int prg_err;
	long int x;
	char *end;
	const char *out_msg = "swocserver,blocklist;";
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
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "blocklist"))
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
 * @return 0 on success, < zero on failure.
 */
int sws_server_wait(void)
{
	int prg_err = 0;
	int empty;
	long int x;
	char *end;
	const char *out_msg = "swocserver,status;";
	size_t om_length = strlen(out_msg);
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	syslog((int)(LOG_USER | LOG_NOTICE), "Waiting for any client locks to "
					     "be released.\n");

	prg_err = swcom_validate_config();
	if (prg_err)
		return prg_err;
	do {
		sleep((unsigned int)pollint);
		prg_err = exch_msg(out_msg, om_length, msg);
		if (prg_err)
			return prg_err;

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
		if (msg->argc > 3)
			snprintf(locks_held, ARRAY_SIZE(locks_held), "%i",
				 ((msg->argc - 3) / 2));
		empty = strcmp(msg->message, "swocserverd,status,ok;");
		clear_msg(msg, ';', ',');
	} while (empty);
	/* Allow one more sleep as a buffer, maybe client is shutting down. */
	sleep((unsigned int)pollint);
	syslog((int)(LOG_USER | LOG_NOTICE), "No outstanding client locks on "
					     "this server.");
	return 0;
}

/**
 * Remove a lock.
 * On error mge_errno is set.
 * @param lockname Client whose lock is to be removed.
 * @return 0 on success, < zero on failure.
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

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserver,release,%s;",
		 lockname);
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

	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Lock removed manually from "
	       "server - %s.",
	       lockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Set a client to blocked.
 * On error mge_errno is set.
 * @param blockname Client to block.
 * @return 0 on success, < zero on failure.
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

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserver,block,%s;",
		 blockname);
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s blocked.", blockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Unblock a client block.
 * On error mge_errno is set.
 * @param blockname Client to unblock.
 * @return 0 on success, < zero on failure.
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

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserver,unblock,%s;",
		 blockname);
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s unblocked.", blockname);

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Terminate the daemon.
 * Send a message to the daemon asking it to terminate.
 * On error mge_errno is set.
 * @return 0 on success, < zero on failure.
 */
int sws_end_daemon(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	const char *out_msg = "swocserver,end;";
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
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "end"))
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Request to end daemon "
					     "acknowledged.");

	clear_msg(msg, ';', ',');

	return 0;
}

/**
 * Request the daemon to reload the config file.
 * Send a message to the daemon asking it to reload it's configuration file.
 * On error mge_errno is set.
 * @return 0 on success, < zero on failure.
 */
int sws_reload_config(void)
{
	int prg_err = 0;
	long int x;
	char *end;
	const char *out_msg = "swocserver,reload;";
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
			if (!(strcmp(msg->argv[0], "swocserverd"))
			    && !(strcmp(msg->argv[1], "reload"))
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

	syslog((int)(LOG_USER | LOG_NOTICE), "Config file reloaded.");

	clear_msg(msg, ';', ',');

	return 0;
}
