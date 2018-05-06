/**
 * @file com-dev/src/prg/c/inc/libswoccommon.h
 *
 * Header file for Server Wait on Client common library.
 *
 * @author Copyright (C) 2017-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 05/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 24/10/2017	MG	1.0.1	This ChangeLog introduced.		*
 * 29/10/2017	MG	1.0.2	Move port / socket mapping struct to	*
 *				swocserverd.h.				*
 * 10/11/2017	MG	1.0.3	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 07/03/2018	MG	1.0.4	Remove redundant global variable portno	*
 * 01/05/2018	MG	1.0.5	Add support for blocked clients list.	*
 * 05/05/2018	MG	1.0.6	Add support for server list of blocked	*
 *				clients.				*
 *				Improve function name consistency,	*
 *				unlock -> release.			*
 *									*
 ************************************************************************
 */


#ifndef LIBSWOCCOMMON_H
#define LIBSWOCCOMMON_H


#include <portability.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mgemessage.h>


BEGIN_C_DECLS

extern int pollint;
extern int ssh;
extern char server[];
extern int srvportno;
extern int sshportno;
extern char sshuser[];


/** Socket buffer size. */
#define SOCK_BUF_SIZE 256

/** Queue length for listen backlog. */
#define SOCK_Q_LEN 10

/** Timeout for SSH channel read (in ms). */
#define SSH_CHAN_POLL_TIMEOUT 10000

/** enum identifying the source of a message. */
enum msg_source {swocclient, swocserver, swocserverd, src_err};

/** enum identifying the message request. */
enum msg_request {swocblock, swocblocklist, swocend, swoclock, swocrelease,
	swocreload, swocreset, swocstatus, swocunblock, req_err};

/** enum specifying error status of arguments. */
enum msg_arguments {args_ok, args_err};

/** enum indentify send or receive mode. */
enum comms_mode {recv_mode, send_mode};


int swcom_validate_config(void);

int prep_recv_sock(int *sockfd, int *portno);

int init_conn(int *sockfd, int *portno, char *server);

int est_connect(int *sfd, char *serv, int *portno, struct addrinfo *hints,
	enum comms_mode *mode);

int listen_sock(const int *sfd);

int close_sock(const int *sockfd);

void parse_msg(struct mgemessage *msg, enum msg_arguments *msg_args,
	enum msg_source *msg_src, enum msg_request *msg_req);

int send_outgoing_msg(char *outgoing_msg, size_t outgoing_msg_length,
			int *newsockfd);

int exch_msg(char *outgoing_msg, size_t om_length, struct mgemessage *msg);

int open_ssh_tunnel(void);

int close_ssh_tunnel(void);

char *libswoccommon_get_pkg_version(void);

char *libswoccommon_get_src_version(void);

void libswoccommon_print_pkg_version(void);

void libswoccommon_print_src_version(void);


END_C_DECLS

#endif /* ndef LIBSWOCCOMMON_H */
