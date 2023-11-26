/**
 * @file src/prg/c/inc/swoc/libswoccommon.h
 *
 * Internal header file for Server Wait on Client common library.
 *
 * @author Copyright (C) 2017-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 25/11/2023_
 */

#ifndef LIBSWOCCOMMON_H
#define LIBSWOCCOMMON_H

#include <netdb.h>
#include <sys/types.h>

#include <libmgec/mge-message.h>
#include <libmgec/mge-portability.h>

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
enum msg_source { swocclient, swocserver, swocserverd, src_err };

/** enum identifying the message request. */
enum msg_request {
	swocallow,
	swocblock,
	swocblocklist,
	swocblockstatus,
	swocdisallow,
	swocend,
	swocid,
	swoclock,
	swocrelease,
	swocreload,
	swocreset,
	swocstatus,
	swocunblock,
	req_err
};

/** enum specifying error status of arguments. */
enum msg_arguments { args_ok, args_err };

/** enum indentify send or receive mode. */
enum comms_mode { recv_mode, send_mode };

int swcom_validate_config(void);

int prep_recv_sock(int *sockfd, int *portno);

int init_conn(int *sockfd, int *portno, char *srv);

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
