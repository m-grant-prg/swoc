/**
 * @file srv-prg/src/prg/c/src/swocserverd/comms.c
 *
 * Comms functions associated with the swocserverd daemon.
 *
 * @author Copyright (C) 2017-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.9 ==== 10/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 04/06/2017	MG	1.0.1	First release.				*
 *				Split comms functions out of main into	*
 *				their own source file.			*
 *				Tidy up unnecessary include statements.	*
 * 07/06/2017	MG	1.0.2	Implement epoll controlled use of	*
 *				multiple ports.				*
 *				Ensure error path frees memory		*
 *				allocation.				*
 * 14/09/2017	MG	1.0.3	Change 'force unlock' to just 'unlock'.	*
 * 29/10/2017	MG	1.0.4	Remove references to TLS. Security now	*
 *				implemented from client side via SSH	*
 *				tunnelling.				*
 * 18/11/2017	MG	1.0.5	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 31/01/2018	MG	1.0.6	Standardise on error message daemon	*
 *				returns, now, swocserverd, ,err,9;	*
 *				where 9 is a valid mge-errno.h error	*
 *				number.					*
 * 28/03/2018	MG	1.0.7	Enforce ANSI function declarations.	*
 *				Declare variables before code, (fixes	*
 *				sparse warning).			*
 * 01/05/2018	MG	1.0.8	Add support for blocked clients list.	*
 * 10/05/2018	MG	1.0.9	Improve function name consistency,	*
 *				unlock -> release.			*
 *				Add server client block and unblock.	*
 *				Add server block and unblock.		*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <mge-errno.h>
#include <swocserverd.h>


static int bind_ports(int *sfd, int *portno, struct addrinfo *hints);
static int init_epoll(int *pepfd, struct epoll_event *pevent,
		struct comm_spec *pt_ps);
static int proc_events(int n_events, struct epoll_event *pevents);
static void proc_msg(struct mgemessage *message);


/**
 * Prepare all sockets.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int prepare_sockets(void)
{
	int i;
	struct addrinfo hints;
	struct bstree *tmp_p_sock;

	/*
	 * BSD now allows that the PF_ prefix always has same value as AF_. It
	 * is now accepted that AF_ is the standard. Ref NOTES in man 2 socket.
	 * Old plan was PF_ prefix described as "TCP/IP" and AF_ prefix as
	 * "Internet".
	 */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;		/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* TCP stream socket */
	hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */
	hints.ai_protocol = IPPROTO_TCP;	/* Only TCP protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	for (i = 0; i < 10; i++) {
		if ((port_spec + i)->portno == 0)
			continue;
		mge_errno = bind_ports(&((port_spec + i)->socketfd),
			&((port_spec + i)->portno), &hints);
		if (mge_errno)
			return mge_errno;

		tmp_p_sock = add_bst_node(port_sock, (port_spec + i),
					sizeof(*port_spec));
		if (tmp_p_sock == NULL)
			return mge_errno;
		port_sock = tmp_p_sock;

		mge_errno = listen_sock(&((port_spec + i)->socketfd));
		if (mge_errno)
			return mge_errno;
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
	mge_errno = 0;

	sprintf(port, "%i", *portno);
	s = getaddrinfo(NULL, port, hints, &result);
	if (s) {
		sav_errno = s;
		mge_errno = MGE_GAI;
		syslog((int) (LOG_USER | LOG_NOTICE), "getaddrinfo error - %s",
			mge_strerror(mge_errno));
		return mge_errno;
	}

	/* getaddrinfo() returns a list of address structures. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		*sfd = socket(rp->ai_family, rp->ai_socktype,
			rp->ai_protocol);
		if (*sfd == -1)
			continue;

		r = 1;
		/*
		 * If the connection is being restarted within a, say,
		 * 60 second timeframe, a bind error may occur. To stop
		 * this happening set the flag to say re-use the socket.
		 */
		i = setsockopt(*sfd, SOL_SOCKET, SO_REUSEADDR, &r,
			sizeof r);
		if (!i)
			i = bind(*sfd, rp->ai_addr, rp->ai_addrlen);

		if (!i)
			break;

		close(*sfd);
	}
	if (rp == NULL) {	/* No address succeeded */
		mge_errno = MGE_GAI_BIND;
		syslog((int) (LOG_USER | LOG_NOTICE), "%s",
				mge_strerror(mge_errno));
	}
	freeaddrinfo(result);
	return mge_errno;
}

/**
 * Wait and then process communications.
 * On error mge_errno will be set.
 * @return 0 on success, non-zero on failure.
 */
int process_comms(void)
{
	int epfd, nr_events;
	struct comm_spec tmp_ps = { 0, 0 };
	struct comm_spec *ptmp_ps = &tmp_ps;
	struct epoll_event event, *events;

	swsd_err = init_epoll(&epfd, &event, ptmp_ps);
	if (swsd_err)
		return swsd_err;

	events = malloc(sizeof(*events) * MAX_EPOLL_EVENTS);
	if (events == NULL) {
		swsd_err = errno;
		if (debug)
			perror("ERROR allocating events");
		syslog((int) (LOG_USER | LOG_NOTICE), "ERROR allocating "
			"events - %s", strerror(swsd_err));
		return swsd_err;
	}

	while (1) {
		nr_events = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, -1);
		swsd_err = proc_events(nr_events, events);
		if (swsd_err)
			goto err_exit;
		if (end)
			break;
	}
	ptmp_ps = &tmp_ps;
	while ((ptmp_ps = find_next_bst_node(port_sock, ptmp_ps)) != NULL) {
		swsd_err = close_sock(&(ptmp_ps->socketfd));
		if (swsd_err)
			goto err_exit;
	}

err_exit:
	free(events);
	return swsd_err;
}

/*
 * Initialise the epoll instance.
 */
static int init_epoll(int *pepfd, struct epoll_event *pevent,
		struct comm_spec *pt_ps)
{
	*pepfd = epoll_create1(0);
	if (*pepfd < 0) {
		swsd_err = errno;
		if (debug)
			perror("ERROR on epoll_create1");
		syslog((int) (LOG_USER | LOG_NOTICE), "ERROR on epoll_create1 "
			"- %s", strerror(swsd_err));
		return swsd_err;
	}
	bzero(pevent, sizeof(*pevent));
	pevent->events = EPOLLIN;
	while ((pt_ps = find_next_bst_node(port_sock, pt_ps)) != NULL) {
		pevent->data.fd = pt_ps->socketfd;
		swsd_err = epoll_ctl(*pepfd, EPOLL_CTL_ADD, pt_ps->socketfd,
				pevent);
		if (swsd_err) {
			swsd_err = errno;
			if (debug)
				perror("ERROR on epoll_ctl");
			syslog((int) (LOG_USER | LOG_NOTICE), "ERROR on "
				"epoll_ctl - %s", strerror(swsd_err));
			return swsd_err;
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
	ssize_t n;
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	char sock_buf[SOCK_BUF_SIZE];
	struct mgebuffer msg_buf1;
	struct mgebuffer *msg_buf;
	struct mgemessage msg1 = { NULL, 0, 0, 0, ';', ',', 0, NULL };
	struct mgemessage *msg = &msg1;

	clilen = sizeof(cli_addr);

	for (i = 0; i < n_events; i++) {
		swsd_err = 0;
		msg_buf1 = (struct mgebuffer) { NULL, DEF_BUF_SIZE , 0 };
		msg_buf = &msg_buf1;

		cursockfd = accept(pevents[i].data.fd,
				(struct sockaddr *) &cli_addr, &clilen);
		if (cursockfd < 0) {
			swsd_err = errno;
			if (debug)
				perror("ERROR on accept");
			syslog((int) (LOG_USER | LOG_NOTICE), "ERROR on accept "
				"- %s", strerror(swsd_err));
			return swsd_err;
		}
		/* Get client hostname. */
		getnameinfo((struct sockaddr *) &cli_addr,
			(socklen_t) sizeof(cli_addr), (char * restrict) &client,
			(socklen_t) sizeof(client), NULL, (socklen_t) 0, 0);
		if (debug)
			printf("Client hostname %s\n", client);

		bzero(sock_buf, sizeof(sock_buf));

		while ((n = recv(cursockfd, sock_buf, sizeof(sock_buf), 0))
			!= 0) {
			if (n < 0) {
				swsd_err = errno;
				if (debug)
					perror("ERROR reading from socket");
				syslog((int) (LOG_USER | LOG_NOTICE), "ERROR "
					"reading from socket - %s",
					strerror(swsd_err));
				return swsd_err;
			}
			msg_buf = concat_buf(sock_buf, n, msg_buf);
			if (msg_buf1.buffer == NULL) {
				swsd_err = mge_errno;
				return swsd_err;
			}
			if (debug)
				print_buf(msg_buf);

			do {
				tmp_comp = 0;
				msg = pull_msg(msg_buf, msg);
				swsd_err = mge_errno;
				if (debug)
					print_msg(msg);
				tmp_comp = msg1.complete;
				if (tmp_comp && !swsd_err)
					proc_msg(msg);
				clear_msg(msg, ';', ',');
			} while (tmp_comp && !swsd_err);

			if (debug)
				print_buf(msg_buf);

			if (end || swsd_err)
				break;

			bzero(sock_buf, sizeof(sock_buf));
		}
		free(msg_buf1.buffer);
		if (close_sock(&cursockfd))
			return swsd_err;
		if (end)
			break;
	}
	return swsd_err;
}

/*
 * Control function to process a message.
 */
static void proc_msg(struct mgemessage *msg)
{
	enum msg_source msg_src;
	enum msg_request msg_req;
	enum msg_arguments msg_args;
	char out_msg[100];

	parse_msg(msg, &msg_args, &msg_src, &msg_req);

	if (msg_args == args_err) {
		if (debug)
			fprintf(stderr, "Invalid arguments from %s in "
				"message %s\n", client, msg->message);
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid arguments "
			"from %s in message %s", client, msg->message);
		sprintf(out_msg, "swocserverd, ,err,%i;", MGE_INVAL_MSG);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return;
	}

	switch (msg_src) {
	case swocserver:
		if (debug)
			printf("source = server\n");
		switch (msg_req) {
		case swocallow:
			if (debug)
				printf("request = allow\n");
			srv_unblock_req(msg, &msg_args);
			break;
		case swocblock:
			if (debug)
				printf("request = block\n");
			srv_cli_block_req(msg, &msg_args);
			break;
		case swocblocklist:
			if (debug)
				printf("request = blocklist\n");
			srv_cli_blocklist_req(msg, &msg_args);
			break;
		case swocblockstatus:
			if (debug)
				printf("request = blockstatus\n");
			srv_block_status_req(msg, &msg_args);
			break;
		case swocdisallow:
			if (debug)
				printf("request = disallow\n");
			srv_block_req(msg, &msg_args);
			break;
		case swocend:
			if (debug)
				printf("request = end\n");
			srv_end_req(msg, &msg_args);
			break;
		case swocrelease:
			if (debug)
				printf("request = release\n");
			srv_cli_rel_req(msg, &msg_args);
			break;
		case swocreload:
			if (debug)
				printf("request = reload\n");
			srv_reload_req(msg, &msg_args);
			break;
		case swocstatus:
			if (debug)
				printf("request = status\n");
			srv_status_req(msg, &msg_args);
			break;
		case swocunblock:
			if (debug)
				printf("request = unblock\n");
			srv_cli_unblock_req(msg, &msg_args);
			break;
		default:
			if (debug)
				fprintf(stderr, "Invalid request from %s in "
					"message %s\n", client, msg->message);
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid request "
				"from %s in message %s", client, msg->message);
			sprintf(out_msg, "swocserverd, ,err,%i;",
				MGE_INVAL_MSG);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
		return;
		}
		break;
	case swocclient:
		if (debug)
			printf("source = client\n");
		switch (msg_req) {
		case swocblock:
			if (debug)
				printf("request = block\n");
			cli_block_req(msg, &msg_args);
			break;
		case swoclock:
			if (debug)
				printf("request = lock\n");
			cli_lock_req(msg, &msg_args);
			break;
		case swocrelease:
			if (debug)
				printf("request = release\n");
			cli_rel_req(msg, &msg_args);
			break;
		case swocreset:
			if (debug)
				printf("request = reset\n");
			cli_reset_req(msg, &msg_args);
			break;
		case swocstatus:
			if (debug)
				printf("request = status\n");
			cli_status_req(msg, &msg_args);
			break;
		case swocunblock:
			if (debug)
				printf("request = unblock\n");
			cli_unblock_req(msg, &msg_args);
			break;
		default:
			if (debug)
				fprintf(stderr, "Invalid request from %s in "
					"message %s\n", client, msg->message);
			syslog((int) (LOG_USER | LOG_NOTICE), "Invalid request "
				"from %s in message %s", client, msg->message);
			sprintf(out_msg, "swocserverd, ,err,%i;",
				MGE_INVAL_MSG);
			send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
			swsd_err = mge_errno;
		return;
		}
		break;
	default:
		if (debug)
			fprintf(stderr, "Invalid message source from %s in "
				"message %s\n", client, msg->message);
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid message source "
			"from %s in message %s", client, msg->message);
		sprintf(out_msg, "swocserverd, ,err,%i;", MGE_INVAL_MSG);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return;
	}

	if (msg_args == args_err) {
		if (debug)
			fprintf(stderr, "Invalid arguments from %s in "
				"message %s\n", client, msg->message);
		syslog((int) (LOG_USER | LOG_NOTICE), "Invalid arguments "
			"from %s in message %s", client, msg->message);
		sprintf(out_msg, "swocserverd, ,err,%i;", MGE_INVAL_MSG);
		send_outgoing_msg(out_msg, strlen(out_msg), &cursockfd);
		swsd_err = mge_errno;
		return;
	}
}
