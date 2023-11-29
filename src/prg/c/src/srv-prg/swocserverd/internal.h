/**
 * @file src/prg/c/src/srv-prg/swocserverd/internal.h
 *
 * Header file for Server Wait on Clients server-side daemon.
 *
 * @author Copyright (C) 2016-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#ifndef SWOCSERVERD_INTERNAL_H
#define SWOCSERVERD_INTERNAL_H

#include <limits.h>

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

#include <libmgec/mge-bstree.h>
#include <libmgec/mge-message.h>
#include <libmgec/mge-portability.h>
#include <swoc/libswoccommon.h>

BEGIN_C_DECLS

#define MAX_LISTEN_PORTS 10 /**< Max number of listening ports. */
#define MAX_EPOLL_EVENTS 64 /**< Max num events for EPOLL. */

/**
 * Map sockets to ports.
 */
struct comm_spec {
	int portno;   /**< Port number. */
	int socketfd; /**< Socket file descriptor. */
};

extern int swsd_err;
extern char client[_POSIX_HOST_NAME_MAX];
extern int debug;
extern int end;
extern int cursockfd;
extern struct comm_spec *port_spec;
extern bool srv_blocked;
extern struct bstree *cli_locks, *cli_blocked, *port_sock;

int swsd_validate_config(void);

int prepare_sockets(void);

int process_comms(void);

int srv_end_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_status_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_cli_blocklist_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_block_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_block_status_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_block_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_unblock_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_srv_block_status_req(struct mgemessage *msg,
			     enum msg_arguments *msg_args);

int srv_cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_lock_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_rel_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int srv_reload_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int swsd_reload_config(void);

int cli_status_req(struct mgemessage *msg, enum msg_arguments *msg_args);

int cli_reset_req(struct mgemessage *msg, enum msg_arguments *msg_args);

void id_req(struct mgemessage *msg, enum msg_arguments *msg_args);

__attribute__((const)) const char *swocserverd_get_pkg_version(void);

__attribute__((const)) const char *swocserverd_get_src_version(void);

void swocserverd_print_pkg_version(void);

void swocserverd_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCSERVERD_INTERNAL_H */
