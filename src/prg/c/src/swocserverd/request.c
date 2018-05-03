/**
 * @file srv-prg/src/prg/c/src/swocserverd/request.c
 *
 * Request processing functions.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.13 ==== 01/05/2018_
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
 * 01/05/2018	MG	1.0.13	Add support for blocked clients list.	*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include <errno.h>

#include <mgememory.h>
#include <mge-errno.h>
#include <mgemessage.h>
#include <swocserverd.h>
#include <bstree.h>


/**
 * swocserver requesting the daemon to terminate.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int end_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char outgoing_msg[100];
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

	sprintf(outgoing_msg, "swocserverd,end,ok;");
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * swocserver request to reload config file.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int reload_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	swsd_err = swsd_validate_config();
	if (swsd_err) {
		if (debug)
			fprintf(stderr, "Validate config errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Validate config errored "
			"with %i.", mge_errno);
		sprintf(outgoing_msg, "swocserverd,reload,err,%i;", mge_errno);
		send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
			&cursockfd);
		exit(swsd_err);
	}
	if (debug)
		printf("Config file reloaded.\n");
	syslog((int) (LOG_USER | LOG_NOTICE), "Config file reloaded.");
	sprintf(outgoing_msg, "swocserverd,reload,ok;");
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Server status request.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int server_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char *outgoing_msg, *t_outgoing_msg = NULL;
	size_t outgoing_msg_size = DEF_MSG_SIZE;
	char tmp_msg[_POSIX_HOST_NAME_MAX + 100] = "";
	char *client_lu = "";
	int counter;
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	if ((int) outgoing_msg_size < 23)
		outgoing_msg_size = 23;
	outgoing_msg = mg_realloc(NULL, outgoing_msg_size);
	if (outgoing_msg == NULL) {
		swsd_err = errno;
		return swsd_err;
	}
	*outgoing_msg = '\0';
	strcat(outgoing_msg, "swocserverd,status,ok");

	while((client_lu = (char *) find_next_bst_node(locks, client_lu))
		!= NULL) {

		counter = get_counter_bst_node(locks, client_lu);

		sprintf(tmp_msg, ",%s,%i", client_lu, counter);
		if (((int) outgoing_msg_size - (int) strlen(outgoing_msg))
				< ((int) strlen(tmp_msg) + 2)) {
			outgoing_msg_size += (int) strlen(tmp_msg) + 2;
			t_outgoing_msg = outgoing_msg;
			outgoing_msg = mg_realloc(t_outgoing_msg,
							outgoing_msg_size);
			if (outgoing_msg == NULL) {
				swsd_err = errno;
				free(t_outgoing_msg);
				return swsd_err;
			}
		}

		if (debug)
			printf("Client %s has %i locks.\n", client_lu, counter);
		strcat(outgoing_msg, tmp_msg);
	}
	strcat(outgoing_msg, ";");
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;

	free(outgoing_msg);
	return swsd_err;
}

/**
 * Server lock removal.
 * Exactly 1 argument, the client lock to be removed.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int unlock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc != 3) {
		*msg_args = args_err;
		return swsd_err;
	}
	plocks = del_bst_node(locks, *(msg->argv + 2));

	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node removal errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node removal errored "
			"with %i.", mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		sprintf(outgoing_msg, "swocserverd,unlock,err,%i;", mge_errno);
	} else {
		locks = plocks;
		if (debug)
			printf("Lock %s removed by server.\n",
				*(msg->argv + 2));
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock removed "
			"by server.", *(msg->argv + 2));
		sprintf(outgoing_msg, "swocserverd,unlock,ok,%s;",
			*(msg->argv + 2));
	}
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
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
int block_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(blocked, client);

	if (pblocked == NULL) {
		pblocked = add_bst_node(blocked, client, strlen(client) + 1);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node add errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node add errored"
			" with %i.", mge_errno);
			sprintf(outgoing_msg, "swocserverd,block,err,%i;",
				mge_errno);
			send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
					  &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		blocked = pblocked;
	}

	if (debug)
		printf("Client %s blocked.\n", client);
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s blocked.", client);
	sprintf(outgoing_msg, "swocserverd,block,ok;");

	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
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
int unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *pblocked;
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	pblocked = find_bst_node(blocked, client);

	if (pblocked != NULL) {
		pblocked = del_bst_node(blocked, client);
		if (pblocked == NULL) {
			if (debug)
				fprintf(stderr, "Node del errored with %i.\n",
					mge_errno);
			syslog((int) (LOG_USER | LOG_NOTICE), "Node del errored"
			" with %i.", mge_errno);
			sprintf(outgoing_msg, "swocserverd,unblock,err,%i;",
				mge_errno);
			send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
					  &cursockfd);
			swsd_err = mge_errno;
			return swsd_err;
		}
		blocked = pblocked;
	}

	if (debug)
		printf("Client %s unblocked.\n", client);
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s unblocked.", client);
	sprintf(outgoing_msg, "swocserverd,unblock,ok;");

	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Client lock request.
 * No parameters allowed, add client lock.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int lock_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	pblocked = find_bst_node(blocked, client);
	if (pblocked != NULL) {
		mge_errno = MGE_CLIENT_BLOCKED;
		if (debug)
			fprintf(stderr, "Client blocked - %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client blocked - %i.",
		       mge_errno);
		sprintf(outgoing_msg, "swocserverd,lock,err,%i;", mge_errno);
		send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
				  &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	plocks = add_bst_node(locks, client, strlen(client) + 1);
	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node add errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node add errored "
			"with %i.", mge_errno);
		sprintf(outgoing_msg, "swocserverd,lock,err,%i;", mge_errno);
	} else {
		locks = plocks;
		if (debug)
			printf("Lock %s added.\n", client);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock added.",
			client);
		sprintf(outgoing_msg, "swocserverd,lock,ok;");
	}
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Release request from client.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int release_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks;
	char outgoing_msg[100];
	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	plocks = del_bst_node(locks, client);

	if (plocks == NULL) {
		if (debug)
			fprintf(stderr, "Node delete errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node delete errored "
			"with %i.", mge_errno);
		/* Change to more user informational error. */
		if (mge_errno == MGE_NODE_NOT_FOUND)
			mge_errno = MGE_LOCK_NOT_FOUND;
		sprintf(outgoing_msg, "swocserverd,release,err,%i;", mge_errno);
	} else {
		locks = plocks;
		if (debug)
			printf("Lock %s removed.\n", client);
		syslog((int) (LOG_USER | LOG_NOTICE), "Client %s lock removed.",
			client);
		sprintf(outgoing_msg, "swocserverd,release,ok;");
	}
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}

/**
 * Status request from client.
 * @param mgemessage The message being processed.
 * @param msg_args The message arguments.
 * @return 0 on success, non-zero on failure.
 */
int client_status_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	char outgoing_msg[100];
	int counter, block;

	swsd_err = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}

	counter = get_counter_bst_node(locks, client);
	if (counter == -1) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node count errored "
			"with %i.", mge_errno);
		sprintf(outgoing_msg, "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
				  &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	block = get_counter_bst_node(blocked, client);
	if (block == -1) {
		if (debug)
			fprintf(stderr, "Node count errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "Node count errored "
			"with %i.", mge_errno);
		sprintf(outgoing_msg, "swocserverd,status,err,%i;", mge_errno);
		send_outgoing_msg(outgoing_msg, strlen(outgoing_msg),
				  &cursockfd);
		swsd_err = mge_errno;
		return swsd_err;
	}

	if (debug) {
		printf("Client %s has %i locks.\n", client, counter);
		printf("Client has blocked status of %i.\n", block);
	}
	sprintf(outgoing_msg, "swocserverd,status,ok,%i,%i;", counter, block);
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
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
int reset_req(struct mgemessage *msg, enum msg_arguments *msg_args)
{
	struct bstree *plocks, *pblocked;
	char outgoing_msg[100];
	swsd_err = 0;
	int b = 0;
	int l = 0;

	if (msg->argc > 2) {
		*msg_args = args_err;
		return swsd_err;
	}
	while ((plocks = del_bst_node(locks, client)) != NULL) {
		l++;
		locks = plocks;
	}

	pblocked = del_bst_node(blocked, client);
	if (pblocked != NULL) {
		b = 1;
		blocked = pblocked;
	}

	if (debug) {
		printf("%i locks removed.\n", l);
		printf("%i blocks removed.\n", b);
	}
	syslog((int) (LOG_USER | LOG_NOTICE), "Client %s, %i locks removed and "
			"%i blocks removed.", client, l, b);
	sprintf(outgoing_msg, "swocserverd,reset,ok,%i,%i;", l, b);
	send_outgoing_msg(outgoing_msg, strlen(outgoing_msg), &cursockfd);
	swsd_err = mge_errno;
	return swsd_err;
}
