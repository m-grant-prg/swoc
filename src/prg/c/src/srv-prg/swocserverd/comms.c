/**
 * @file src/prg/c/src/srv-prg/swocserverd/comms.c
 *
 * Comms functions associated with the swocserverd daemon.
 *
 * @author Copyright (C) 2017-2024  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.1 ==== 27/05/2024_
 */

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
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

#include "internal.h"
#include <libmgec/libmgec.h>
#include <libmgec/mge-bstree.h>
#include <libmgec/mge-buffer.h>
#include <libmgec/mge-errno.h>
#include <libmgec/mge-message.h>
#include <swoc/libswoccommon.h>

static int bind_ports(int *sfd, int *portno, struct addrinfo *hints);
static int init_epoll(int *pepfd, struct epoll_event *pevent,
		      struct comm_spec *pt_ps);
static int proc_events(int n_events, struct epoll_event *pevents);
static int proc_msg(struct mgemessage *message);

/**
 * Prepare all sockets.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int prepare_sockets(void)
{
	int i, ret;
	struct addrinfo hints;
	struct bstree *tmp_p_sock;

	/*
	 * BSD now allows that the PF_ prefix always has same value as AF_. It
	 * is now accepted that AF_ is the standard. Ref NOTES in man 2 socket.
	 * Old plan was PF_ prefix described as "TCP/IP" and AF_ prefix as
	 * "Internet".
	 */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;	 /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* TCP stream socket */
	hints.ai_flags = AI_PASSIVE;	 /* For wildcard IP address */
	hints.ai_protocol = IPPROTO_TCP; /* Only TCP protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	for (i = 0; i < 10; i++) {
		if ((port_spec + i)->portno == 0)
			continue;
		ret = bind_ports(&((port_spec + i)->socketfd),
				 &((port_spec + i)->portno), &hints);
		if (ret)
			return ret;

		tmp_p_sock = add_bst_node(port_sock, (port_spec + i),
					  sizeof(*port_spec));
		if (tmp_p_sock == NULL)
			return -mge_errno;
		port_sock = tmp_p_sock;

		ret = listen_sock(&((port_spec + i)->socketfd));
		if (ret)
			return ret;
	}
	return 0;
}

/*
 * Establish send or receive connection.
 */
static int bind_ports(int *sfd, int *portno, struct addrinfo *hints)
{
	struct addrinfo *result, *rp;
	int i, r, s;
	char port[6];

	snprintf(port, ARRAY_SIZE(port), "%i", *portno);
	s = getaddrinfo(NULL, port, hints, &result);
	if (s) {
		sav_errno = s;
		mge_errno = MGE_GAI;
		syslog((int)(LOG_USER | LOG_NOTICE), "getaddrinfo error - %s",
		       mge_strerror(mge_errno));
		return -mge_errno;
	}

	/* getaddrinfo() returns a list of address structures. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		*sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (*sfd == -1)
			continue;

		r = 1;
		/*
		 * If the connection is being restarted within a, say,
		 * 60 second timeframe, a bind error may occur. To stop
		 * this happening set the flag to say re-use the socket.
		 */
		i = setsockopt(*sfd, SOL_SOCKET, SO_REUSEADDR, &r, sizeof r);
		if (!i)
			r = bind(*sfd, rp->ai_addr, rp->ai_addrlen);

		if (!r)
			break;

		close(*sfd);
	}
	r = 0;
	if (rp == NULL) { /* No address succeeded */
		mge_errno = MGE_GAI_BIND;
		syslog((int)(LOG_USER | LOG_NOTICE), "%s",
		       mge_strerror(mge_errno));
		r = -mge_errno;
	}
	freeaddrinfo(result);
	return r;
}

/**
 * Wait and then process communications.
 * On error mge_errno will be set.
 * @return 0 on success, < zero on failure.
 */
int process_comms(void)
{
	int e, epfd, nr_events, ret;
	struct comm_spec tmp_ps = { 0, 0 };
	struct comm_spec *ptmp_ps = &tmp_ps;
	struct epoll_event event, *events;

	e = init_epoll(&epfd, &event, ptmp_ps);
	if (e)
		return e;

	events = malloc(sizeof(*events) * MAX_EPOLL_EVENTS);
	if (events == NULL) {
		mge_errno = MGE_ERRNO;
		if (debug)
			perror("ERROR allocating events");
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "ERROR allocating "
		       "events - %s",
		       mge_strerror(mge_errno));
		return -mge_errno;
	}

	while (1) {
		nr_events = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, -1);
		ret = proc_events(nr_events, events);
		if (ret)
			goto free_exit;
		if (end)
			break;
	}
	ptmp_ps = &tmp_ps;
	while ((ptmp_ps = find_next_bst_node(port_sock, ptmp_ps)) != NULL) {
		ret = close_sock(&(ptmp_ps->socketfd));
		if (ret)
			goto free_exit;
	}

free_exit:
	free(events);
	return ret;
}

/*
 * Initialise the epoll instance.
 */
static int init_epoll(int *pepfd, struct epoll_event *pevent,
		      struct comm_spec *pt_ps)
{
	int ret;

	*pepfd = epoll_create1(0);
	if (*pepfd < 0) {
		mge_errno = MGE_ERRNO;
		ret = -mge_errno;
		if (debug)
			perror("ERROR on epoll_create1");
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "ERROR on epoll_create1 "
		       "- %s",
		       mge_strerror(mge_errno));
		return ret;
	}
	memset(pevent, '\0', sizeof(*pevent));
	pevent->events = EPOLLIN;
	while ((pt_ps = find_next_bst_node(port_sock, pt_ps)) != NULL) {
		pevent->data.fd = pt_ps->socketfd;
		ret = epoll_ctl(*pepfd, EPOLL_CTL_ADD, pt_ps->socketfd, pevent);
		if (ret) {
			mge_errno = MGE_ERRNO;
			ret = -mge_errno;
			if (debug)
				perror("ERROR on epoll_ctl");
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "ERROR on "
			       "epoll_ctl - %s",
			       mge_strerror(mge_errno));
			return ret;
		}
	}
	return 0;
}

/*
 * Process epoll events.
 */
static int proc_events(int n_events, struct epoll_event *pevents)
{
	int i, tmp_comp;
	int err = 0;
	ssize_t n;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	char sock_buf[SOCK_BUF_SIZE];
	struct mgebuffer msg_buf1;
	struct mgebuffer *msg_buf;
	struct mgemessage msg1 = MGEMESSAGE_INIT(';', ',');
	struct mgemessage *msg = &msg1;

	clilen = sizeof(cli_addr);

	for (i = 0; i < n_events; i++) {
		msg_buf1 = (struct mgebuffer){ NULL, 0, 0, 0 };
		msg_buf = &msg_buf1;

		cursockfd = accept(pevents[i].data.fd,
				   (struct sockaddr *)&cli_addr, &clilen);
		if (cursockfd < 0) {
			mge_errno = MGE_ERRNO;
			if (debug)
				perror("ERROR on accept");
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "ERROR on accept "
			       "- %s",
			       mge_strerror(mge_errno));
			return -mge_errno;
		}

		client[0] = '\0';

		memset(sock_buf, '\0', sizeof(sock_buf));
		while ((n = recv(cursockfd, sock_buf, sizeof(sock_buf), 0))
		       != 0) {
			if (n < 0) {
				mge_errno = MGE_ERRNO;
				if (debug)
					perror("ERROR reading from socket");
				syslog((int)(LOG_USER | LOG_NOTICE),
				       "ERROR "
				       "reading from socket - %s",
				       mge_strerror(mge_errno));
				return -mge_errno;
			}
			msg_buf = concat_buf(sock_buf, (size_t)n, msg_buf);
			if (msg_buf1.buffer == NULL)
				return -mge_errno;
			if (debug)
				print_buf(msg_buf);

			do {
				tmp_comp = 0;
				msg = pull_msg(msg_buf, msg);
				if (msg == NULL)
					err = mge_errno;
				if (debug)
					print_msg(msg);
				tmp_comp = msg1.complete;
				if (tmp_comp && !err) {
					err = proc_msg(msg);
					clear_msg(msg, ';', ',');
				}
			} while (tmp_comp && !err);

			if (debug)
				print_buf(msg_buf);

			if (end || err)
				break;

			memset(sock_buf, '\0', sizeof(sock_buf));
		}
		clear_msg(msg, ';', ',');
		free(msg_buf1.buffer);
		if (close_sock(&cursockfd))
			return err;
		if (end)
			break;
	}
	return err;
}

/*
 * Control function to process a message.
 */
static int proc_msg(struct mgemessage *msg)
{
	int ret;
	enum msg_source msg_src;
	enum msg_request msg_req;
	enum msg_arguments msg_args;
	char out_msg[100];

	parse_msg(msg, &msg_args, &msg_src, &msg_req);

	if (msg_args == args_err) {
		if (debug)
			fprintf(stderr,
				"Invalid arguments from %s in "
				"message %s\n",
				client, msg->message);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Invalid arguments "
		       "from %s in message %s",
		       client, msg->message);
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd, ,err,%i;",
			 MGE_INVAL_MSG);
		ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return ret;
	}

	/* Must identify before any other message. */
	if (!*client && msg_req != swocid) {
		if (debug)
			fprintf(stderr, "Host not identified for message %s\n",
				msg->message);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Host not identified for message %s\n", msg->message);
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd,%s,err,%i;",
			 msg->argv[1], MGE_ID);
		ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return ret;
	}
	switch (msg_src) {
	case swocserver:
		if (debug)
			printf("source = server\n");
		switch (msg_req) {
		case swocallow:
			if (debug)
				printf("request = allow\n");
			ret = srv_unblock_req(msg, &msg_args);
			break;
		case swocblock:
			if (debug)
				printf("request = block\n");
			ret = srv_cli_block_req(msg, &msg_args);
			break;
		case swocblocklist:
			if (debug)
				printf("request = blocklist\n");
			ret = srv_cli_blocklist_req(msg, &msg_args);
			break;
		case swocblockstatus:
			if (debug)
				printf("request = blockstatus\n");
			ret = srv_block_status_req(msg, &msg_args);
			break;
		case swocdisallow:
			if (debug)
				printf("request = disallow\n");
			ret = srv_block_req(msg, &msg_args);
			break;
		case swocend:
			if (debug)
				printf("request = end\n");
			ret = srv_end_req(msg, &msg_args);
			break;
		case swocid:
			if (debug)
				printf("request = id\n");
			ret = 0;
			id_req(msg, &msg_args);
			break;
		case swocrelease:
			if (debug)
				printf("request = release\n");
			ret = srv_cli_rel_req(msg, &msg_args);
			break;
		case swocreload:
			if (debug)
				printf("request = reload\n");
			ret = srv_reload_req(msg, &msg_args);
			break;
		case swocstatus:
			if (debug)
				printf("request = status\n");
			ret = srv_status_req(msg, &msg_args);
			break;
		case swocunblock:
			if (debug)
				printf("request = unblock\n");
			ret = srv_cli_unblock_req(msg, &msg_args);
			break;
		default:
			if (debug)
				fprintf(stderr,
					"Invalid request from %s in "
					"message %s\n",
					client, msg->message);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Invalid request "
			       "from %s in message %s",
			       client, msg->message);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd, ,err,%i;", MGE_INVAL_MSG);
			ret = send_outgoing_msg(out_msg, strlen(out_msg),
						&cursockfd);
			return ret;
		}
		break;
	case swocclient:
		if (debug)
			printf("source = client\n");
		switch (msg_req) {
		case swocblock:
			if (debug)
				printf("request = block\n");
			ret = cli_block_req(msg, &msg_args);
			break;
		case swocblockstatus:
			if (debug)
				printf("request = blockstatus\n");
			ret = cli_srv_block_status_req(msg, &msg_args);
			break;
		case swocid:
			if (debug)
				printf("request = id\n");
			ret = 0;
			id_req(msg, &msg_args);
			break;
		case swoclock:
			if (debug)
				printf("request = lock\n");
			ret = cli_lock_req(msg, &msg_args);
			break;
		case swocrelease:
			if (debug)
				printf("request = release\n");
			ret = cli_rel_req(msg, &msg_args);
			break;
		case swocreset:
			if (debug)
				printf("request = reset\n");
			ret = cli_reset_req(msg, &msg_args);
			break;
		case swocstatus:
			if (debug)
				printf("request = status\n");
			ret = cli_status_req(msg, &msg_args);
			break;
		case swocunblock:
			if (debug)
				printf("request = unblock\n");
			ret = cli_unblock_req(msg, &msg_args);
			break;
		default:
			if (debug)
				fprintf(stderr,
					"Invalid request from %s in "
					"message %s\n",
					client, msg->message);
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Invalid request "
			       "from %s in message %s",
			       client, msg->message);
			snprintf(out_msg, ARRAY_SIZE(out_msg),
				 "swocserverd, ,err,%i;", MGE_INVAL_MSG);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			mge_errno = MGE_INVAL_MSG;
			return -mge_errno;
		}
		break;
	default:
		if (debug)
			fprintf(stderr,
				"Invalid message source from %s in "
				"message %s\n",
				client, msg->message);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Invalid message source "
		       "from %s in message %s",
		       client, msg->message);
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd, ,err,%i;",
			 MGE_INVAL_MSG);
		ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return ret;
	}

	if (msg_args == args_err) {
		if (debug)
			fprintf(stderr,
				"Invalid arguments from %s in "
				"message %s\n",
				client, msg->message);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Invalid arguments "
		       "from %s in message %s",
		       client, msg->message);
		snprintf(out_msg, ARRAY_SIZE(out_msg), "swocserverd, ,err,%i;",
			 MGE_INVAL_MSG);
		ret = send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		return ret;
	}
	return ret;
}
