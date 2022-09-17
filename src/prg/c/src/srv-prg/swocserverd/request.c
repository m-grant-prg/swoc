/**
 * @file src/prg/c/src/srv-prg/swocserverd/request.c
 *
 * Request processing functions.
 *
 * @author Copyright (C) 2016-2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.26 ==== 17/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.1	First release.				*
 * 08/01/2017	MG	1.0.2	Add creation / deletion of pid file.	*
 *				(Recommended for a systemd service of	*
 *				type forking.)				*
 * 02/02/2017	MG	1.0.3	Migrate to use of mge_errno in libmgec.	*
 * 13/02/2017	MG	1.0.4	Implement config file reload		*
 *				functionality.				*
 * 22/04/2017	MG	1.0.5	Change to use new bstree struct.	*
 * 27/05/2017	MG	1.0.6	Give daemon its own config file.	*
 * 				Use new local validateconfig.		*
 * 				Add support for temporary include	*
 *				directory.				*
 * 04/06/2017	MG	1.0.7	Split comms functions out of main into	*
 *				their own source file.			*
 *				Tidy up unnecessary include statements.	*
 * 				Use more meaningful name for client	*
 *				lock bstree.				*
 * 07/06/2017	MG	1.0.8	Implement epoll controlled use of	*
 *				multiple ports.				*
 * 14/09/2017	MG		Change force_unlock to unlock.		*
 * 18/11/2017	MG	1.0.9	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 31/01/2018	MG	1.0.10	On unlock and release return a more	*
 *				informative mge_errno.			*
 * 24/03/2018	MG	1.0.11	Add missing mgemessage.h include.	*
 * 28/03/2018	MG	1.0.12	Declare variables before code, (fixes	*
 *				a sparse warning).			*
 * 10/05/2018	MG	1.0.13	Add support for blocked clients list.	*
 *				Add server client block and unblock.	*
 *				Add server block and unblock.		*
 * 18/05/2018	MG	1.0.14	Add client show server block status.	*
 * 22/05/2018	MG	1.0.15	Change from swocserverd.h to internal.h	*
 * 03/06/2018	MG	1.0.16	Log number of clients and locks		*
 *				existing when an end request is		*
 *				processed.				*
 * 25/08/2018	MG	1.0.17	Extract a core config reload function	*
 *				from srv_reload_req so that it can be	*
 *				used by the signal handler on receipt	*
 *				of SIGHUP, (which is a convention).	*
 * 12/09/2018	MG	1.0.18	Remove use of no longer available	*
 *				DEF_MSG_SIZE macro, (it is not part of	*
 *				the API).				*
 * 18/05/2019	MG	1.0.19	Merge sub-projects into one.		*
 * 02/06/2019	MG	1.0.20	Remove 2 incorrect casts.		*
 *				Correct cut & paste error in param msg.	*
 * 08/11/2019	MG	1.0.21	Use standard GNU ifdeffery around use	*
 *				of AC_HEADER_STDBOOL.			*
 * 21/03/2020	MG	1.0.22	Add id request type and use it to set	*
 *				the client name. Due to always getting	*
 *				localhost when over an SSH tunnel.	*
 * 08/12/2021	MG	1.0.23	Tighten SPDX tag.			*
 * 14/04/2022	MG	1.0.24	Improve error handling consistency.	*
 * 12/06/2022	MG	1.0.25	Replace sprintf with safer snprintf.	*
 * 17/09/2022	MG	1.0.26	Rename mgemessage.h			*
 *				Rename mgememory.h			*
 *				Rename bstree.h				*
 *				Use pkginclude location.		*
 *				Correct included header files.		*
 *									*
 ************************************************************************
 */

#include <arpa/inet.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <syslog.h>

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

#include "internal.h"
#include <libmgec/libmgec.h>
#include <libmgec/mge-bstree.h>
#include <libmgec/mge-errno.h>
#include <libmgec/mge-memory.h>
#include <libmgec/mge-message.h>
#include <swoc/libswoccommon.h>

/**
 * swocserver requesting the daemon to terminate.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_end_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}
	end = 1;

	if (debug)
		printf("Termination request received - exiting.\n");
	syslog((int)(LOG_USER | LOG_NOTICE), "Termination request received - "
					     "exiting.");
	if (debug) {
		printf("%i clients have %i locks on this server.\n",
		       cli_locks->node_total, cli_locks->count_total);
	}
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "%i clients have %i locks on "
	       "this server.",
	       cli_locks->node_total, cli_locks->count_total);

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,end,ok;");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * swocserver request to reload config file.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_reload_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	ret = swsd_reload_config();
	if (ret) {
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,reload,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		exit(ret);
	}
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,reload,ok;");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Reload the config file.
 * This function should only ever be called by srv_reload_req or the signal
 * handler on receipt of SIGHUP which is a convention for daemons.
 * @return 0 on success, < zero on failure.
 */
int swsd_reload_config(void)
{
	int ret;

	ret = swsd_validate_config();
	if (ret) {
		if (debug)
			fprintf(stderr, "Validate config errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Validate config errored "
		       "with %i.",
		       mge_errno);
		return ret;
	}
	if (debug)
		printf("Config file reloaded.\n");
	syslog((int)(LOG_USER | LOG_NOTICE), "Config file reloaded.");
	return ret;
}

/**
 * Server status request.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, > zero on failure.
 */
int srv_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char *out_msg, *t_out_msg = NULL;
	size_t out_msg_size = 30; /* Large enough for an error message. */
	char tmp_msg[_POSIX_HOST_NAME_MAX + 100] = "";
	char *client_lu = "";
	int counter, ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	out_msg = mg_realloc(NULL, out_msg_size);
	if (out_msg == NULL)
		return -mge_errno;

	*out_msg = '\0';
	strcat(out_msg, "swocserverd,status,ok");

	while ((client_lu = (char *)find_next_bst_node(cli_locks, client_lu))
	       != NULL) {
		counter = get_counter_bst_node(cli_locks, client_lu);
		if (counter < 0) {
			if (debug)
				fprintf(stderr, "Node count errored with %i.\n",
					mge_errno);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Node count "
			       "errored with %i.",
			       mge_errno);
			snprintf(out_msg, out_msg_size,
				 "swocserverd,status,err,%i;", mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			return -mge_errno;
		}

		snprintf(tmp_msg, ARRAY_SIZE(tmp_msg), ",%s,%i", client_lu,
			 counter);
		if (((int)out_msg_size - (int)strlen(out_msg))
		    < ((int)strlen(tmp_msg) + 2)) {
			out_msg_size += strlen(tmp_msg) + 2;
			t_out_msg = out_msg;
			out_msg = mg_realloc(t_out_msg, out_msg_size);
			if (out_msg == NULL) {
				free(t_out_msg);
				return -mge_errno;
			}
		}

		if (debug)
			printf("Client %s has %i locks.\n", client_lu, counter);
		strcat(out_msg, tmp_msg);
	}
	strcat(out_msg, ";");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);

	free(out_msg);
	return ret;
}

/**
 * Server client block list request.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_cli_blocklist_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char *out_msg, *t_out_msg = NULL;
	size_t out_msg_size = 30;
	char tmp_msg[_POSIX_HOST_NAME_MAX + 100] = "";
	char *client_lu = "";
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	out_msg = mg_realloc(NULL, out_msg_size);
	if (out_msg == NULL)
		return -mge_errno;

	*out_msg = '\0';
	strcat(out_msg, "swocserverd,blocklist,ok");

	while ((client_lu = (char *)find_next_bst_node(cli_blocked, client_lu))
	       != NULL) {
		snprintf(tmp_msg, ARRAY_SIZE(tmp_msg), ",%s", client_lu);
		if (((int)out_msg_size - (int)strlen(out_msg))
		    < ((int)strlen(tmp_msg) + 2)) {
			out_msg_size += strlen(tmp_msg) + 2;
			t_out_msg = out_msg;
			out_msg = mg_realloc(t_out_msg, out_msg_size);
			if (out_msg == NULL) {
				free(t_out_msg);
				return -mge_errno;
			}
		}

		if (debug)
			printf("Client %s is blocked.\n", client_lu);
		strcat(out_msg, tmp_msg);
	}
	strcat(out_msg, ";");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);

	free(out_msg);
	return ret;
}

/**
 * Server requests client lock release.
 * Exactly 1 argument, the client lock to be released.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char out_msg[100];
	int ret;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return 0;
	}

	plocks = del_bst_node(cli_locks, *(msg->argv + 2));
	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node removal errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Node removal errored "
		       "with %i.",
		       mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,release,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}

	cli_locks = plocks;
	if (debug)
		printf("Lock %s removed by server.\n", *(msg->argv + 2));
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Client %s lock removed by server.", *(msg->argv + 2));
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,release,ok;");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server requests client to be blocked.
 * Exactly 1 argument, the client to be blocked.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	int ret;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return 0;
	}

	pblocked = find_bst_node(cli_blocked, *(msg->argv + 2));
	if (pblocked == NULL) {
		pblocked = add_bst_node(cli_blocked, *(msg->argv + 2),
					strlen(*(msg->argv + 2)) + 1);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node add errored with %i.\n",
					mge_errno);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Node add errored"
			       " with %i.",
			       mge_errno);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd,block,err,%i;", mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			return -mge_errno;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s blocked.\n", *(msg->argv + 2));
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s blocked.",
	       *(msg->argv + 2));
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,block,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server requests client to be unblocked.
 * Exactly 1 argument, the client to be unblocked.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	int ret;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return 0;
	}

	pblocked = find_bst_node(cli_blocked, *(msg->argv + 2));
	if (pblocked != NULL) {
		pblocked = del_bst_node(cli_blocked, *(msg->argv + 2));
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node del errored with %i.\n",
					mge_errno);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Node del errored"
			       " with %i.",
			       mge_errno);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd,unblock,err,%i;", mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			return -mge_errno;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s unblocked.\n", *(msg->argv + 2));
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s unblocked.",
	       *(msg->argv + 2));
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,unblock,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server requests server level blocking.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	srv_blocked = true;

	if (debug)
		printf("Server blocked.\n");
	syslog((int)(LOG_USER | LOG_NOTICE), "Server blocked.");

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,disallow,ok;");
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server requests removal of server level blocking.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	srv_blocked = false;

	if (debug)
		printf("Server unblocked.\n");
	syslog((int)(LOG_USER | LOG_NOTICE), "Server unblocked.");

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,allow,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server requests status of server level blocking.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int srv_block_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	if (debug)
		printf("Server block status.\n");

	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,blockstatus,ok,%i;",
		 srv_blocked);

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Client block further locks request.
 * No parameters allowed.
 * Set a block on this client so that it cannot instantiate any more locks until
 * the client is unblocked. The client can release locks while blocked.
 * If the client is already blocked the function succedes.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	pblocked = find_bst_node(cli_blocked, client);
	if (pblocked == NULL) {
		pblocked
			= add_bst_node(cli_blocked, client, strlen(client) + 1);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node add errored with %i.\n",
					mge_errno);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Node add errored"
			       " with %i.",
			       mge_errno);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd,block,err,%i;", mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			return -mge_errno;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s blocked.\n", client);
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s blocked.", client);
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,block,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Client unblock further locks request.
 * No parameters allowed.
 * Remove a block on this client so that it can instantiate locks again.
 * If the client is already unblocked the function succedes.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	pblocked = find_bst_node(cli_blocked, client);
	if (pblocked != NULL) {
		pblocked = del_bst_node(cli_blocked, client);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node del errored with %i.\n",
					mge_errno);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Node del errored"
			       " with %i.",
			       mge_errno);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd,unblock,err,%i;", mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			return -mge_errno;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s unblocked.\n", client);
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s unblocked.", client);
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,unblock,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Client requests status of server level blocking.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_srv_block_status_req(struct mgemessage *msg,
			     enum msg_arguments *msg_args)
{
	return srv_block_status_req(msg, msg_args);
}

/**
 * Client lock request.
 * No parameters allowed, add client lock.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_lock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	pblocked = find_bst_node(cli_blocked, client);
	if (pblocked != NULL || srv_blocked) {
		mge_errno = MGE_CLIENT_BLOCKED;
		if (debug)
			fprintf(stderr, "Client blocked - %i.\n", mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE), "Client blocked - %i.",
		       mge_errno);
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,lock,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}

	plocks = add_bst_node(cli_locks, client, strlen(client) + 1);
	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node add errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Node add errored "
		       "with %i.",
		       mge_errno);
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,lock,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}
	cli_locks = plocks;
	if (debug)
		printf("Lock %s added.\n", client);
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s lock added.", client);
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,lock,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Release request from client.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char out_msg[100];
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	plocks = del_bst_node(cli_locks, client);
	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node delete errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Node delete errored "
		       "with %i.",
		       mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,release,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}
	cli_locks = plocks;
	if (debug)
		printf("Lock %s removed.\n", client);
	syslog((int)(LOG_USER | LOG_NOTICE), "Client %s lock removed.", client);
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,release,ok;");

	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Status request from client.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int counter, block, ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}

	counter = get_counter_bst_node(cli_locks, client);
	if (counter < 0) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Node count errored "
		       "with %i.",
		       mge_errno);
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}

	block = get_counter_bst_node(cli_blocked, client);
	if (block < 0) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Node count errored "
		       "with %i.",
		       mge_errno);
		snprintf(out_msg, ARRAY_SIZE(out_msg),
			 "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return -mge_errno;
	}

	if (debug) {
		printf("Client %s has %i locks.\n", client, counter);
		printf("Client has blocked status of %i.\n", block);
	}
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,status,ok,%i,%i;",
		 counter, block);
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Reset request from client.
 * Remove all locks and any block.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, < zero on failure.
 */
int cli_reset_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char out_msg[100];
	int b = 0;
	int l = 0;
	int ret;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return 0;
	}
	while ((plocks = del_bst_node(cli_locks, client)) != NULL) {
		l++;
		cli_locks = plocks;
	}

	pblocked = del_bst_node(cli_blocked, client);
	if (pblocked != NULL) {
		b = 1;
		cli_blocked = pblocked;
	}

	if (debug) {
		printf("%i locks removed.\n", l);
		printf("%i blocks removed.\n", b);
	}
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "Client %s, %i locks removed and "
	       "%i blocks removed.",
	       client, l, b);
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,reset,ok,%i,%i;", l,
		 b);
	ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return ret;
}

/**
 * Server or client submits id.
 * The host name and IP address are submitted for the daemon to know the
 * machine identity even if it is coming over SSH (which always appears as
 * localhost to the daemon).
 * Daemon is unaffected by errors which are relayed back to the sender.
 * Exactly 2 arguments, the host name and IP address.
 * @param msg The message being processed.
 * @param msg_args The message arguments.
 */
void id_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct addrinfo *result, *rp;
	struct addrinfo hints;
	char address[INET6_ADDRSTRLEN];
	char canonname[_POSIX_HOST_NAME_MAX];
	char out_msg[100];
	void *na;
	int count, s;

	client[0] = '\0';

	if (msg->argc != 4) {
		*msg_args = args_err;
		return;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_CANONNAME;
	s = getaddrinfo(*(msg->argv + 2), NULL, &hints, &result);
	if (s) {
		sav_errno = s;
		mge_errno = MGE_GAI;
		syslog((int)(LOG_USER | LOG_NOTICE), "getaddrinfo error - %s",
		       mge_strerror(mge_errno));
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,id,err,%i;",
			 mge_errno);
		goto send_exit;
	}

	count = 0;
	/* getaddrinfo() returns a list of address structures. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		switch (rp->ai_family) {
		case AF_INET:
			na = &((struct sockaddr_in *)rp->ai_addr)->sin_addr;
			break;
		case AF_INET6:
			na = &((struct sockaddr_in6 *)rp->ai_addr)->sin6_addr;
			break;
		default:
			sav_errno = EAI_FAMILY;
			mge_errno = MGE_GAI;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "getaddrinfo error - %s",
			       mge_strerror(mge_errno));
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd,id,err,%i;", mge_errno);
			goto free_exit;
		}
		if (!count)
			strcpy(canonname, rp->ai_canonname);
		inet_ntop(rp->ai_family, na, address, sizeof(address));
		if (debug) {
			printf("address is %s\n", address);
			if (!count)
				printf("canonname is %s\n", canonname);
		}
		if (!strcmp(address, *(msg->argv + 3))
		    && !strcmp(canonname, *(msg->argv + 2))) {
			break;
		}
		count++;
	}
	if (rp == NULL) { /* No address succeeded */
		mge_errno = MGE_ID;
		if (debug)
			fprintf(stderr, "Host and IP not matched - %s %s\n",
				*(msg->argv + 2), *(msg->argv + 3));
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Host and IP not matched - %s %s", *(msg->argv + 2),
		       *(msg->argv + 3));
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,id,err,%i;",
			 mge_errno);
		goto free_exit;
	}
	if (debug)
		printf("Host and IP matched - %s %s\n", *(msg->argv + 2),
		       *(msg->argv + 3));
	syslog((int)(LOG_USER | LOG_NOTICE), "Host and IP matched - %s %s",
	       *(msg->argv + 2), *(msg->argv + 3));
	snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,id,ok;");
	strcpy(client, canonname);

free_exit:
	freeaddrinfo(result);
send_exit:
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	return;
}
