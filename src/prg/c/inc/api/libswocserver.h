/**
 * @file src/prg/c/inc/api/libswocserver.h
 *
 * Header file for Server Wait on Clients server-side library.
 *
 * @author Copyright (C) 2016-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.15 ==== 08/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 16/07/2016	MG	1.0.1	First versioned release.		*
 * 17/07/2016	MG	1.0.2	Further coding style changes.		*
 * 14/12/2016	MG	1.0.3	Changes for move from NFS share 'file	*
 *				as flag' to TCP socket implementation.	*
 *				Add server extern.			*
 *				Add common server-side validate		*
 *				prototype.				*
 *				Add new end daemon function prototype.	*
 * 13/02/2017	MG	1.0.4	Add new reload config function.		*
 * 15/05/2017	MG	1.0.5	Change validate config to swocserver-	*
 *				only, aot common with swocserverd.	*
 * 12/09/2017	MG	1.0.6	Change sws_force_unlock() to		*
 *				sws_unlock().				*
 * 15/09/2017	MG	1.0.7	Change references to ssl to tls.	*
 * 02/10/2017	MG	1.0.8	Move validate config to common.		*
 * 12/11/2017	MG	1.0.9	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 29/03/2018	MG	1.0.10	Add locks_held for use during signal	*
 *				handling, primarily in swocserver -w.	*
 * 10/05/2018	MG	1.0.11	Improve function name consistency,	*
 *				unlock -> release.			*
 *				Add support for server listing blocked	*
 *				clients.				*
 *				Add client block and unblock.		*
 *				Add server block and unblock.		*
 * 18/05/2019	MG	1.0.12	Merge sub-projects into one.		*
 * 11/10/2021	MG	1.0.13	Move to api directory.			*
 * 08/12/2021	MG	1.0.14	Tighten SPDX tag.			*
 * 08/09/2022	MG	1.0.15	Rename portability.h			*
 *									*
 ************************************************************************
 */

#ifndef LIBSWOCSERVER_H
#define LIBSWOCSERVER_H

#include <mge-portability.h>

BEGIN_C_DECLS

extern char locks_held[];

int sws_show_status(void);

int sws_show_block_status(void);

int sws_srv_block(void);

int sws_srv_unblock(void);

int sws_show_cli_blocklist(void);

int sws_server_wait(void);

int sws_release(char *lockname);

int sws_cli_block(char *blockname);

int sws_cli_unblock(char *blockname);

int sws_end_daemon(void);

int sws_reload_config(void);

char *libswocserver_get_pkg_version(void);

char *libswocserver_get_src_version(void);

void libswocserver_print_pkg_version(void);

void libswocserver_print_src_version(void);

END_C_DECLS

#endif /* ndef LIBSWOCSERVER_H */
