/**
 * @file src/prg/c/src/srv-prg/swocserverd/request.c
 *
 * Request processing functions.
 *
 * @author Copyright (C) 2016-2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.20 ==== 02/06/2019_
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
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include <mgememory.h>
#include <mge-errno.h>
#include <mgemessage.h>
#include "internal.h"
#include <bstree.h>


/**
 * swocserver requesting the daemon to terminate.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_end_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	end = 1;

	if (debug)
		printf("Termination request received - exiting.\n");
	syslog((int) (LOG_USER | LOG_NOTICE), "Termination request received - "
		"exiting.");
	if (debug) {
		printf("%i clients have %i locks on this server.\n",
		       cli_locks->node_total, cli_locks->count_total);
	}
	syslog((int) (LOG_USER | LOG_NOTICE), "%i clients have %i locks on "
		"this server.", cli_locks->node_total, cli_locks->count_total);

	sprintf(out_msg, "swocserverd,end,ok;");
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * swocserver request to reload config file.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_reload_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	swsd_err = swsd_reload_config();
	if (swsd_err) {
		sprintf(out_msg, "swocserverd,reload,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		exit(swsd_err);
	}
	sprintf(out_msg, "swocserverd,reload,ok;");
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Reload the config file.
 * This function should only ever be called by srv_reload_req or the signal
 * handler on receipt of SIGHUP which is a convention for daemons.
 * @return 0 on success, non-zero on failure.
 */
int swsd_reload_config(void)
{
	swsd_err = swsd_validate_config();
	if (swsd_err) {
		if (debug)
			fprintf(stderr, "Validate config errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Validate config errored "
			"with %i.", mge_errno);
		return swsd_err;
	}
	if (debug)
		printf("Config file reloaded.\n");
	syslog((int) (LOG_USER | LOG_NOTICE), "Config file reloaded.");
	return swsd_err;
}

/**
 * Server status request.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char *out_msg, *t_out_msg = NULL;
	size_t out_msg_size = 30;	/* Large enough for an error message. */
	char tmp_msg[_POSIX_HOST_NAME_MAX + 100] = "";
	char *client_lu = "";
	int counter;
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	out_msg = mg_realloc(NULL, out_msg_size);
	if (out_msg == NULL) {
		swsd_err = errno;
		return swsd_err;
	}
	*out_msg = '\0';
	strcat(out_msg, "swocserverd,status,ok");

	while((client_lu = (char *) find_next_bst_node(cli_locks, client_lu))
	      != NULL) {

		counter = get_counter_bst_node(cli_locks, client_lu);
		if (counter < 0) {
			if (debug)
				fprintf(stderr, "Node count errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node count "
				"errored with %i.", mge_errno);
			sprintf(out_msg, "swocserverd,status,err,%i;",
				mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}

		sprintf(tmp_msg, ",%s,%i", client_lu, counter);
		if (((int) out_msg_size - (int) strlen(out_msg))
				< ((int) strlen(tmp_msg) + 2)) {
			out_msg_size += strlen(tmp_msg) + 2;
			t_out_msg = out_msg;
			out_msg = mg_realloc(t_out_msg, out_msg_size);
			if (out_msg == NULL) {
				swsd_err = errno;
				free(t_out_msg);
				return swsd_err;
			}
		}

		if (debug)
			printf("Client %s has %i locks.\n", client_lu, counter);
		strcat(out_msg, tmp_msg);
	}
	strcat(out_msg, ";");
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;

	free(out_msg);
	return swsd_err;
}

/**
 * Server client block list request.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_cli_blocklist_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char *out_msg, *t_out_msg = NULL;
	size_t out_msg_size = 30;
	char tmp_msg[_POSIX_HOST_NAME_MAX + 100] = "";
	char *client_lu = "";
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	out_msg = mg_realloc(NULL, out_msg_size);
	if (out_msg == NULL) {
		swsd_err = errno;
		return swsd_err;
	}
	*out_msg = '\0';
	strcat(out_msg, "swocserverd,blocklist,ok");

	while((client_lu = (char *) find_next_bst_node(cli_blocked, client_lu))
		!= NULL) {

		sprintf(tmp_msg, ",%s", client_lu);
		if (((int) out_msg_size - (int) strlen(out_msg))
				< ((int) strlen(tmp_msg) + 2)) {
			out_msg_size += strlen(tmp_msg) + 2;
			t_out_msg = out_msg;
			out_msg = mg_realloc(t_out_msg, out_msg_size);
			if (out_msg == NULL) {
				swsd_err = errno;
				free(t_out_msg);
				return swsd_err;
			}
		}

		if (debug)
			printf("Client %s is blocked.\n", client_lu);
		strcat(out_msg, tmp_msg);
	}
	strcat(out_msg, ";");
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;

	free(out_msg);
	return swsd_err;
}

/**
 * Server requests client lock release.
 * Exactly 1 argument, the client lock to be released.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return swsd_err;
	}
	plocks = del_bst_node(cli_locks, *(msg->argv + 2));

	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node removal errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node removal errored "
			"with %i.", mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		sprintf(out_msg, "swocserverd,release,err,%i;", mge_errno);
	} else {
		cli_locks = plocks;
		if (debug)
			printf("Lock %s removed by server.\n",
				*(msg->argv + 2));
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock removed "
			"by server.", *(msg->argv + 2));
		sprintf(out_msg, "swocserverd,release,ok;");
	}
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server requests client to be blocked.
 * Exactly 1 argument, the client to be blocked.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(cli_blocked, *(msg->argv + 2));

	if (pblocked == NULL) {
		pblocked = add_bst_node(cli_blocked, *(msg->argv + 2),
					strlen(*(msg->argv + 2)) + 1);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node add errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node add errored"
			" with %i.", mge_errno);
			sprintf(out_msg, "swocserverd,block,err,%i;",
				mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s blocked.\n", *(msg->argv + 2));
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s blocked.",
	       *(msg->argv + 2));
	sprintf(out_msg, "swocserverd,block,ok;");

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server requests client to be unblocked.
 * Exactly 1 argument, the client to be unblocked.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(cli_blocked, *(msg->argv + 2));

	if (pblocked != NULL) {
		pblocked = del_bst_node(cli_blocked, *(msg->argv + 2));
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node del errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node del errored"
			" with %i.", mge_errno);
			sprintf(out_msg, "swocserverd,unblock,err,%i;",
				mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s unblocked.\n", *(msg->argv + 2));
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s unblocked.",
	       *(msg->argv + 2));
	sprintf(out_msg, "swocserverd,unblock,ok;");

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server requests server level blocking.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	srv_blocked = true;

	if (debug)
		printf("Server blocked.\n");
	syslog((int) (LOG_USER | LOG_NOTICE), "Server blocked.");

	sprintf(out_msg, "swocserverd,disallow,ok;");
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server requests removal of server level blocking.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	srv_blocked = false;

	if (debug)
		printf("Server unblocked.\n");
	syslog((int) (LOG_USER | LOG_NOTICE), "Server unblocked.");

	sprintf(out_msg, "swocserverd,allow,ok;");

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server requests status of server level blocking.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int srv_block_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	if (debug)
		printf("Server block status.\n");

	sprintf(out_msg, "swocserverd,blockstatus,ok,%i;", srv_blocked);

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Client block further locks request.
 * No parameters allowed.
 * Set a block on this client so that it cannot instantiate any more locks until
 * the client is unblocked. The client can release locks while blocked.
 * If the client is already blocked the function succedes.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(cli_blocked, client);

	if (pblocked == NULL) {
		pblocked = add_bst_node(cli_blocked, client,
					strlen(client) + 1);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node add errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node add errored"
			" with %i.", mge_errno);
			sprintf(out_msg, "swocserverd,block,err,%i;",
				mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s blocked.\n", client);
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s blocked.", client);
	sprintf(out_msg, "swocserverd,block,ok;");

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Client unblock further locks request.
 * No parameters allowed.
 * Remove a block on this client so that it can instantiate locks again.
 * If the client is already unblocked the function succedes.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(cli_blocked, client);

	if (pblocked != NULL) {
		pblocked = del_bst_node(cli_blocked, client);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node del errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node del errored"
			" with %i.", mge_errno);
			sprintf(out_msg, "swocserverd,unblock,err,%i;",
				mge_errno);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		cli_blocked = pblocked;
	}

	if (debug)
		printf("Client %s unblocked.\n", client);
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s unblocked.", client);
	sprintf(out_msg, "swocserverd,unblock,ok;");

	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Client requests status of server level blocking.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_srv_block_status_req(struct mgemessage *msg,
			     enum msg_arguments *msg_args)
{
	return srv_block_status_req(msg, msg_args);
}

/**
 * Client lock request.
 * No parameters allowed, add client lock.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_lock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	pblocked = find_bst_node(cli_blocked, client);
	if (pblocked != NULL || srv_blocked) {
		mge_errno = MGE_CLIENT_BLOCKED;
		if (debug)
			fprintf(stderr, "Client blocked - %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client blocked - %i.",
		       mge_errno);
		sprintf(out_msg, "swocserverd,lock,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	plocks = add_bst_node(cli_locks, client, strlen(client) + 1);
	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node add errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node add errored "
			"with %i.", mge_errno);
		sprintf(out_msg, "swocserverd,lock,err,%i;", mge_errno);
	} else {
		cli_locks = plocks;
		if (debug)
			printf("Lock %s added.\n", client);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock added.",
			client);
		sprintf(out_msg, "swocserverd,lock,ok;");
	}
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Release request from client.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char out_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	plocks = del_bst_node(cli_locks, client);

	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node delete errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node delete errored "
			"with %i.", mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		sprintf(out_msg, "swocserverd,release,err,%i;", mge_errno);
	} else {
		cli_locks = plocks;
		if (debug)
			printf("Lock %s removed.\n", client);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock removed.",
			client);
		sprintf(out_msg, "swocserverd,release,ok;");
	}
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Status request from client.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char out_msg[100];
	int counter, block;

	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	counter = get_counter_bst_node(cli_locks, client);
	if (counter < 0) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node count errored "
			"with %i.", mge_errno);
		sprintf(out_msg, "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	block = get_counter_bst_node(cli_blocked, client);
	if (block < 0) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node count errored "
			"with %i.", mge_errno);
		sprintf(out_msg, "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	if (debug) {
		printf("Client %s has %i locks.\n", client, counter);
		printf("Client has blocked status of %i.\n", block);
	}
	sprintf(out_msg, "swocserverd,status,ok,%i,%i;", counter, block);
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Reset request from client.
 * Remove all locks and any block.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int cli_reset_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char out_msg[100];
	int b = 0;
	int l = 0;
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
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
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s, %i locks removed and "
			"%i blocks removed.", client, l, b);
	sprintf(out_msg, "swocserverd,reset,ok,%i,%i;", l, b);
	send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

