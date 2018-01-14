/**
 * @file com-lib/src/prg/c/src/libswoccommon/messages.c
 *
 * Message processing functions common to swoc programs.
 *
 * @author Copyright (C) 2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.1 ==== 10/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 05/05/2017	MG	1.0.0	First release.				*
 * 15/09/2017	MG	1.0.1	Change force-unlock option to unlock.	*
 * 19/10/2017	MG	1.1.0	Add support for SSH.			*
 * 10/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <syslog.h>

#include <mgemessage.h>
#include <mgebuffer.h>
#include <mge-errno.h>
#include <libswoccommon.h>


/**
 * Parse a message.
 * Identify message source and request.
 * @param msg The message to process.
 * @param msg_args The arguments to the message.
 * @param msg_src The source of the message.
 * @param msg_req The request contained in the message.
 */
void parse_msg(struct mgemessage *msg, enum msg_arguments *msg_args,
	enum msg_source *msg_src, enum msg_request *msg_req)
{
	*msg_args = args_ok;

	/* Must have at least source and request. */
	if (msg->argc < 2) {
		*msg_args = args_err;
		return;
	}

	/* Get message source. */
	if (!strcmp(*(msg->argv), "swocserver"))
		*msg_src = swocserver;
	else if (!strcmp(*(msg->argv), "swocclient"))
		*msg_src = swocclient;
	else if (!strcmp(*(msg->argv), "swocserverd"))
		*msg_src = swocserverd;
	else
		*msg_src = src_err;

	/* Get request. */
	if (!strcmp(*(msg->argv + 1), "end"))
		*msg_req = swocend;
	else if (!strcmp(*(msg->argv + 1), "lock"))
		*msg_req = swoclock;
	else if (!strcmp(*(msg->argv + 1), "release"))
		*msg_req = swocrelease;
	else if (!strcmp(*(msg->argv + 1), "reload"))
		*msg_req = swocreload;
	else if (!strcmp(*(msg->argv + 1), "status"))
		*msg_req = swocstatus;
	else if (!strcmp(*(msg->argv + 1), "unlock"))
		*msg_req = swocunlock;
	else
		*msg_req = req_err;
}

/**
 * Send a message.
 * On error mge_errno will be set.
 * @param outgoing_msg The message to send.
 * @param outgoing_msg_length The length of the message.
 * @param newsockfd The socket file descriptor.
 * @return 0 on success, non-zero on error.
 */
int send_outgoing_msg(char *outgoing_msg, size_t outgoing_msg_length,
			int *newsockfd)
{
	int n;
	mge_errno = 0;

	n = send(*newsockfd, outgoing_msg, outgoing_msg_length, 0);
	if (n < 0) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		syslog((int) (LOG_USER | LOG_NOTICE), "ERROR writing to socket "
			"- %s", mge_strerror(mge_errno));
	}
	return mge_errno;
}

/**
 * Exchange messages.
 * Send and receive 1 message.
 * Discard anything in the buffer after the 1st message terminator.
 * On error mge_errno will be set.
 * @param outgoing_msg The message to send.
 * @param om_length The length of the outgoing message.
 * @param msg The received message.
 * @return 0 on success, -1 on error.
 */
int exch_msg(char *outgoing_msg, size_t om_length, struct mgemessage *msg)
{
	int res;
	int sockfd;
	int portno;
	ssize_t n;
	char sock_buf[SOCK_BUF_SIZE];
	struct mgebuffer msg_buf1 = { NULL, DEF_BUF_SIZE , 0 };
	struct mgebuffer *msg_buf = &msg_buf1;

	if (ssh) {
		portno = sshportno;
		res = open_ssh_tunnel();
		if (res)
			return res;
		strcpy(server, "localhost");
	} else {
		portno = srvportno;
	}

	res = init_conn(&sockfd, &portno, server);
	if (res)
		goto err_exit_0;

	res = send_outgoing_msg(outgoing_msg, om_length, &sockfd);
	if (res)
		goto err_exit_1;

	/* Get server daemon reply. */
	bzero(sock_buf, sizeof(sock_buf));

	while ((n = recv(sockfd, sock_buf, sizeof(sock_buf), 0)) != 0) {
		if (n < 0) {
			sav_errno = errno;
			mge_errno = MGE_ERRNO;
			syslog((int) (LOG_USER | LOG_NOTICE), "ERROR reading "
				"from socket - %s", mge_strerror(mge_errno));
			goto err_exit_1;
		}
		msg_buf = concat_buf(sock_buf, n, msg_buf);
		if (msg_buf1.buffer == NULL)
			goto err_exit_1;

		/*
		 * Only 1 message can be valid, anything else is erroneous,
		 * so don't loop emptying the buffer of messages.
		 */
		msg = pull_msg(msg_buf, msg);

		if (msg->complete || mge_errno)
			break;

		clear_msg(msg, ';', ',');

		bzero(sock_buf, sizeof(sock_buf));
	}
	res = close_sock(&sockfd);
	if (res)
		goto err_exit_0;
	free(msg_buf1.buffer);
	if (ssh)
		res = close_ssh_tunnel();
	return res;

err_exit_1:
	close_sock(&sockfd);
err_exit_0:
	free(msg_buf1.buffer);
	if (ssh)
		close_ssh_tunnel();
	return -1;
}
