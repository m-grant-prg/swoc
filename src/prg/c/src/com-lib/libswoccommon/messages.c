/**
 * @file src/prg/c/src/com-lib/libswoccommon/messages.c
 *
 * Message processing functions common to swoc programs.
 *
 * @author Copyright (C) 2017-2021  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.14 ==== 08/12/2021_
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
 * 07/03/2018	MG	1.1.2	Preserve config file global variables	*
 *				unchanged. Use local variables when	*
 *				values need changing.			*
 * 01/05/2018	MG	1.1.3	Add support for client blocking list.	*
 * 10/05/2018	MG	1.1.4	Improve function name consistency,	*
 *				unlock -> release.			*
 *				Add support for server listing blocked	*
 *				clients.				*
 *				Add support for server blocking.	*
 * 17/06/2018	MG	1.1.5	libmgec/pull_msg now allows for the 	*
 *				extraction of partial messages from the	*
 *				buffer struct to the message struct, so	*
 *				eliminate the clear_msg call if the	*
 *				message after pull is incomplete.	*
 * 05/08/2018	MG	1.1.6	Correct mgebuffer struct initialisation	*
 * 17/08/2018	MG	1.1.7	Correct sizing of local serv variable.	*
 * 07/09/2018	MG	1.1.8	Use new mgebuffer struct initialiser.	*
 * 23/09/2018	MG	1.1.9	Replace use of deprecated bzero() with	*
 *				memset().				*
 * 26/05/2019	MG	1.1.10	Merge sub-projects into one.		*
 *				Cast ssize_t to size_t to avoid sign	*
 *				warning.				*
 * 01/06/2019	MG	1.1.11	Trivial improvements to type safety.	*
 * 21/03/2020	MG	1.1.12	Add support for sending an id message	*
 *				before the requested message.		*
 * 10/10/2021	MG	1.1.13	Use newly internal common header.	*
 * 08/12/2021	MG	1.1.14	Tighten SPDX tag.			*
 *									*
 ************************************************************************
 */

#include <errno.h>
#include <ifaddrs.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>

#include <libswoccommon.h>
#include <mge-errno.h>
#include <mgebuffer.h>
#include <mgemessage.h>

static int get_reply_msg(int sockfd, struct mgemessage *recv_msg);
static int host_id(int sockfd, char *orig_outgoing_msg);

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
	if (!strcmp(*(msg->argv + 1), "allow"))
		*msg_req = swocallow;
	else if (!strcmp(*(msg->argv + 1), "block"))
		*msg_req = swocblock;
	else if (!strcmp(*(msg->argv + 1), "blocklist"))
		*msg_req = swocblocklist;
	else if (!strcmp(*(msg->argv + 1), "blockstatus"))
		*msg_req = swocblockstatus;
	else if (!strcmp(*(msg->argv + 1), "disallow"))
		*msg_req = swocdisallow;
	else if (!strcmp(*(msg->argv + 1), "end"))
		*msg_req = swocend;
	else if (!strcmp(*(msg->argv + 1), "id"))
		*msg_req = swocid;
	else if (!strcmp(*(msg->argv + 1), "lock"))
		*msg_req = swoclock;
	else if (!strcmp(*(msg->argv + 1), "release"))
		*msg_req = swocrelease;
	else if (!strcmp(*(msg->argv + 1), "reload"))
		*msg_req = swocreload;
	else if (!strcmp(*(msg->argv + 1), "reset"))
		*msg_req = swocreset;
	else if (!strcmp(*(msg->argv + 1), "status"))
		*msg_req = swocstatus;
	else if (!strcmp(*(msg->argv + 1), "unblock"))
		*msg_req = swocunblock;
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
	ssize_t n;
	mge_errno = 0;

	n = send(*newsockfd, outgoing_msg, outgoing_msg_length, 0);
	if (n < 0) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "ERROR writing to socket "
		       "- %s",
		       mge_strerror(mge_errno));
	}
	return mge_errno;
}

/**
 * Exchange messages.
 * Send and receive 1 requested message after sending ID message.
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
	/* serv must be large enough to hold either server or localhost. */
	char serv[strlen(server) > 9 ? strlen(server) + 1 : 10];

	strcpy(serv, server);

	if (ssh) {
		portno = sshportno;
		res = open_ssh_tunnel();
		if (res)
			return res;
		strcpy(serv, "localhost");
	} else {
		portno = srvportno;
	}

	res = init_conn(&sockfd, &portno, serv);
	if (res)
		goto err_exit_0;

	res = host_id(sockfd, outgoing_msg);
	if (res)
		goto err_exit_1;

	res = send_outgoing_msg(outgoing_msg, om_length, &sockfd);
	if (res)
		goto err_exit_1;

	res = get_reply_msg(sockfd, msg);
	if (res)
		goto err_exit_1;

	res = close_sock(&sockfd);
	if (res)
		goto err_exit_0;
	if (ssh)
		res = close_ssh_tunnel();
	return res;

err_exit_1:
	close_sock(&sockfd);
err_exit_0:
	if (ssh)
		close_ssh_tunnel();
	return -1;
}

/*
 * Get daemon reply message.
 * Send and receive 1 message.
 * Discard anything in the buffer after the 1st message terminator.
 * On error mge_errno will be set.
 * @param sockfd The socket to use.
 * @param msg The received message.
 * @return 0 on success, -1 on error.
 */
static int get_reply_msg(int sockfd, struct mgemessage *recv_msg)
{
	ssize_t n;
	char sock_buf[SOCK_BUF_SIZE];
	struct mgebuffer msg_buf1 = MGEBUFFER_INIT;
	struct mgebuffer *msg_buf = &msg_buf1;

	/* Get server daemon reply. */
	memset(sock_buf, '\0', sizeof(sock_buf));

	while ((n = recv(sockfd, sock_buf, sizeof(sock_buf), 0)) != 0) {
		if (n < 0) {
			sav_errno = errno;
			mge_errno = MGE_ERRNO;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "ERROR reading "
			       "from socket - %s",
			       mge_strerror(mge_errno));
			return -1;
		}
		msg_buf = concat_buf(sock_buf, (size_t)n, msg_buf);
		if (msg_buf1.buffer == NULL)
			return -1;

		/*
		 * Only 1 message can be valid, anything else is erroneous,
		 * so don't loop emptying the buffer of messages.
		 */
		recv_msg = pull_msg(msg_buf, recv_msg);

		if (recv_msg->complete || mge_errno)
			break;

		memset(sock_buf, '\0', sizeof(sock_buf));
	}
	free(msg_buf1.buffer);
	return 0;
}

/*
 * Server or client submits id.
 * The host name and IP address are submitted for the daemon to know the
 * machine identity even if it is coming over SSH (which always appears as
 * localhost to the daemon).
 * Daemon is unaffected by errors which are relayed back to the sender.
 * Exactly 2 arguments, the host name and IP address.
 * @param sockfd The socket in use.
 * @param orig_outgoing_msg The original (main) outgoing message.
 */
static int host_id(int sockfd, char *orig_outgoing_msg)
{
	struct ifaddrs *result, *rp;
	socklen_t sockaddr_size;
	char host_name[_POSIX_HOST_NAME_MAX];
	char host_ip[INET6_ADDRSTRLEN];
	char om[13 + _POSIX_HOST_NAME_MAX + INET6_ADDRSTRLEN];
	char oom[strlen(orig_outgoing_msg) + 1];
	char *end;
	struct mgemessage ret_msg = MGEMESSAGE_INIT(';', ',');
	long int x;
	int s;

	s = getifaddrs(&result);
	if (s) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		syslog((int)(LOG_USER | LOG_NOTICE), "getifaddrs error - %s",
		       mge_strerror(mge_errno));
		return -1;
	}

	/* getifaddrs() returns a list of address structures. */
	for (rp = result; rp != NULL; rp = rp->ifa_next) {
		if (!strcmp(server, "localhost")) {
			if (strcmp(rp->ifa_name, "lo"))
				continue;
		} else if (!strcmp(rp->ifa_name, "lo")) {
			continue;
		}
		switch (rp->ifa_addr->sa_family) {
		case AF_INET:
			sockaddr_size = sizeof(struct sockaddr_in);
			break;
		case AF_INET6:
			sockaddr_size = sizeof(struct sockaddr_in6);
			break;
		default:
			continue;
		}
		s = getnameinfo(rp->ifa_addr, sockaddr_size, host_ip,
				INET6_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST);
		if (s) {
			sav_errno = s;
			mge_errno = MGE_GAI;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "getnameinfo error - %s",
			       mge_strerror(mge_errno));
			goto free_exit;
		}
		s = getnameinfo(rp->ifa_addr, sockaddr_size, host_name,
				_POSIX_HOST_NAME_MAX, NULL, 0, NI_NAMEREQD);
		if (s) {
			sav_errno = s;
			mge_errno = MGE_GAI;
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "getnameinfo error - %s",
			       mge_strerror(mge_errno));
			goto free_exit;
		}
		break;
	}
	if (rp == NULL) { /* No address succeeded */
		mge_errno = MGE_ID;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Host ID not established - %s", mge_strerror(mge_errno));
		goto free_exit;
	}
	strcpy(oom, orig_outgoing_msg);
	strcpy(om, strtok(oom, ","));
	strcat(om, ",id,");
	strcat(om, host_name);
	strcat(om, ",");
	strcat(om, host_ip);
	strcat(om, ";");
	send_outgoing_msg(om, strlen(om), &sockfd);

	s = get_reply_msg(sockfd, &ret_msg);
	if (s)
		goto msg_free_exit;

	if (strncmp(ret_msg.message, "swocserverd,id,ok", 17)) {
		mge_errno = MGE_INVAL_MSG;
		if (ret_msg.argc == 4) {
			if (!(strcmp(ret_msg.argv[0], "swocserverd"))
			    && !(strcmp(ret_msg.argv[1], "id"))
			    && !(strcmp(ret_msg.argv[2], "err"))) {
				x = strtol(ret_msg.argv[3], &end, 10);
				if ((*end == '\0') && x <= INT_MAX
				    && x >= INT_MIN)
					mge_errno = (int)x;
			}
		}
		syslog((int)(LOG_USER | LOG_NOTICE), "Invalid message - %s",
		       mge_strerror(mge_errno));
		goto msg_free_exit;
	}

	clear_msg(&ret_msg, ';', ',');
	freeifaddrs(result);
	return 0;

msg_free_exit:
	clear_msg(&ret_msg, ';', ',');

free_exit:
	freeifaddrs(result);
	return -1;
}

