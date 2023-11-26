/**
 * @file src/prg/c/inc/swoc/libswocserver.h
 *
 * Header file for Server Wait on Clients server-side library.
 *
 * @author Copyright (C) 2016-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 25/11/2023_
 */

#ifndef LIBSWOCSERVER_H
#define LIBSWOCSERVER_H

#include <libmgec/mge-portability.h>

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
