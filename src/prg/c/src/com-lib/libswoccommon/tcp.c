/**
 * @file src/prg/c/src/com-lib/libswoccommon/tcp.c
 *
 * TCP connection processing functions.
 *
 * @author Copyright (C) 2017-2021  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.7 ==== 08/12/2021_
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
 * 06/03/2018	MG	1.1.2	Another client or server comms session	*
 *				could be taking place. Therefore allow	*
 *				bind and connect 10 tries if the	*
 *				address is in use.			*
 * 15/08/2018	MG	1.1.3	Change parameter name to prevent shadow.*
 * 18/05/2019	MG	1.1.4	Merge sub-projects into one.		*
 * 16/06/2020	MG	1.1.5	listen() on a socket may race with	*
 *				other swoc invocations, so allow some	*
 *				retries if the address is in use.	*
 * 10/10/2021	MG	1.1.6	Use newly internalised common header.	*
 * 08/12/2021	MG	1.1.7	Tighten SPDX tag.			*
 *									*
 ************************************************************************
 */

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <libswoccommon.h>
#include <mge-errno.h>

/**
 * Prepare TCP socket to receive connections.
 * On error mge_errno is set.
 * @param sockfd The socket file descriptor.
 * @param portno The port number.
 * @return 0 on success, non-zero on failure.
 */
int prep_recv_sock(int *sockfd, int *portno)
{
	struct addrinfo hints;
	enum comms_mode mode = recv_mode;

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

	/* Host set to NULL for localhost receive. */
	mge_errno = est_connect(sockfd, NULL, portno, &hints, &mode);

	if (mge_errno)
		return mge_errno;

	mge_errno = listen_sock(sockfd);

	return mge_errno;
}

/**
 * Initiate TCP stream socket connection.
 * On error mge_errno is set.
 * @param sockfd The socket file descriptor.
 * @param portno The port number.
 * @param srv The server name.
 * @return 0 on success, non-zero on failure.
 */
int init_conn(int *sockfd, int *portno, char *srv)
{
	struct addrinfo hints;
	enum comms_mode mode = send_mode;

	/* PF_ prefix vs AF_ - see comment in prep_recv_sock(). */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;	 /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* TCP stream socket */
	hints.ai_protocol = IPPROTO_TCP; /* Only TCP protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	mge_errno = est_connect(sockfd, srv, portno, &hints, &mode);

	return mge_errno;
}

/**
 * Establish send or receive connection.
 * Bind or connect depending on mode - listen or send.
 * On error mge_errno is set.
 * @param sfd The socket file descriptor.
 * @param serv The server name.
 * @param portno The port number.
 * @param hints The hints for getaddrinfo().
 * @param mode send_mode or recv_mode.
 * @return 0 on success, non-zero on failure.
 */
int est_connect(int *sfd, char *serv, int *portno, struct addrinfo *hints,
		enum comms_mode *mode)
{
	struct addrinfo *result, *rp;
	int i, r, s;
	int x = 0;
	char port[6];
	mge_errno = 0;

	sprintf(port, "%i", *portno);
	s = getaddrinfo(serv, port, hints, &result);
	if (s) {
		sav_errno = s;
		mge_errno = MGE_GAI;
		syslog((int)(LOG_USER | LOG_NOTICE), "getaddrinfo error - %s",
		       mge_strerror(mge_errno));
		return mge_errno;
	}

	/* getaddrinfo() returns a list of address structures. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		*sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (*sfd == -1)
			continue;

		if (*mode == recv_mode) {
			r = 1;
			/*
			 * If the connection is being restarted within a, say,
			 * 60 second timeframe, a bind error may occur. To stop
			 * this happening set the flag to say re-use the socket.
			 */
			i = setsockopt(*sfd, SOL_SOCKET, SO_REUSEADDR, &r,
				       sizeof r);
			if (!i) {
				do {
					/*
					 * If the address is in use, AOT any
					 * other error, allow 10 tries.
					 */
					i = bind(*sfd, rp->ai_addr,
						 rp->ai_addrlen);
					if (i == -1 && errno == EADDRINUSE)
						sleep(1);
				} while (x++ < 10 && i == -1
					 && errno == EADDRINUSE);
			}
		} else {
			do {
				/*
				 * If the address is in use, AOT any other
				 * error, allow 10 tries.
				 */
				i = connect(*sfd, rp->ai_addr, rp->ai_addrlen);
				if (i == -1 && errno == EADDRINUSE)
					sleep(1);
			} while (x++ < 10 && i == -1 && errno == EADDRINUSE);
		}
		if (!i)
			break;

		close(*sfd);
	}
	if (rp == NULL) { /* No address succeeded */
		mge_errno = MGE_GAI_BIND;
		syslog((int)(LOG_USER | LOG_NOTICE), "%s",
		       mge_strerror(mge_errno));
	}

	freeaddrinfo(result);

	return mge_errno;
}

/**
 * Set TCP socket to listen.
 * Equivalent to listen() with error handling.
 * A race is possible with other swoc invocations to listen on that socket, so
 * if it is in use do a few retries.
 * On error mge_errno is set.
 * @param sfd The socket file descriptor.
 * @return 0 on success, non-zero on failure.
 */
int listen_sock(const int *sfd)
{
	int i = 0;
	int status;

	mge_errno = 0;

	do {
		if (i)
			sleep(1);
		status = listen(*sfd, SOCK_Q_LEN);
	} while ((status < 0) && (errno == EADDRINUSE) && (i++ < 10));

	if (status < 0) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		syslog((int)(LOG_USER | LOG_NOTICE), "ERROR on listen - %s",
		       mge_strerror(mge_errno));
	}
	return mge_errno;
}

/**
 * Close TCP socket.
 * Equivalent to close() with error handling.
 * On error mge_errno is set.
 * @param sockfd The socket file descriptor.
 * @return 0 on success, non-zero on failure.
 */
int close_sock(const int *sockfd)
{
	mge_errno = 0;

	if (close(*sockfd) < 0) {
		sav_errno = errno;
		mge_errno = MGE_ERRNO;
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "ERROR closing socket "
		       "- %s",
		       mge_strerror(mge_errno));
	}
	return mge_errno;
}

