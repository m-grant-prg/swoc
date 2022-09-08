/**
 * @file src/prg/c/src/srv-prg/swocserverd/internal.h
 *
 * Header file for Server Wait on Clients server-side daemon.
 *
 * @author Copyright (C) 2016-2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.19 ==== 08/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.0	First release.				*
 * 17/12/2016	MG	1.0.1	Re-distribute over libswoccommon and	*
 *				libswocserver.				*
 * 13/02/2017	MG	1.0.2	Implement config file reload		*
 *				functionality in daemon.		*
 * 22/04/2017	MG	1.0.3	Change to use new bstree struct.	*
 * 04/06/2017	MG	1.0.4	Use more meaningful name for client	*
 *				lock bstree.				*
 * 07/06/2017	MG	1.0.5	Implement epoll controlled use of	*
 *				multiple ports.				*
 * 14/09/2017	MG	1.0.6	Change 'force unlock' to just 'unlock'.	*
 * 29/10/2017	MG	1.0.7	Remove references to TLS. Security now	*
 * 				implemented from client side via SSH	*
 * 				tunnelling.				*
 * 17/11/2017	MG	1.0.8	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 10/05/2018	MG	1.0.9	Add support for blocked clients list.	*
 *				Add server client block and unblock.	*
 *				Add server block and unblock.		*
 * 18/05/2018	MG	1.0.10	Add client show server block status.	*
 * 22/05/2018	MG	1.0.11	Change from swocserverd.h to internal.h	*
 * 05/08/2018	MG	1.0.12	Remove spurious include of sys/types.h	*
 * 25/08/2018	MG	1.0.13	Add prototype for swsd_reload_config.	*
 * 18/05/2019	MG	1.0.14	Merge sub-projects into one.		*
 * 08/11/2019	MG	1.0.15	Use standard GNU ifdeffery around use	*
 *				of AC_HEADER_STDBOOL.			*
 * 14/03/2020	MG	1.0.16	Add support for id_req.			*
 * 10/10/2021	MG	1.0.17	Use newly internalised common header.	*
 * 08/12/2021	MG	1.0.18	Tighten SPDX tag.			*
 * 08/09/2022	MG	1.0.19	Rename portability.h			*
 *				Rename mgemessage.h			*
 *				Rename bstree.h				*
 *									*
 ************************************************************************
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

#include <libswoccommon.h>
#include <mge-bstree.h>
#include <mge-message.h>
#include <mge-portability.h>

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

char *swocserverd_get_pkg_version(void);

char *swocserverd_get_src_version(void);

void swocserverd_print_pkg_version(void);

void swocserverd_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCSERVERD_INTERNAL_H */
