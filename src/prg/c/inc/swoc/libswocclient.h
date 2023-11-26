/**
 * @file src/prg/c/inc/swoc/libswocclient.h
 *
 * Header file for Server Wait on Clients client-side library.
 *
 * @author Copyright (C) 2016-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 25/11/2023_
 */

#ifndef LIBSWOCCLIENT_H
#define LIBSWOCCLIENT_H

#include <libmgec/mge-portability.h>

BEGIN_C_DECLS

extern char locks_held[];

int swc_show_status(void);

int swc_show_srv_block_status(void);

int swc_block(void);

int swc_unblock(void);

int swc_set_lock(void);

int swc_rel_lock(void);

int swc_client_wait(char *cnumlocks);

int swc_reset(void);

char *libswocclient_get_pkg_version(void);

char *libswocclient_get_src_version(void);

void libswocclient_print_pkg_version(void);

void libswocclient_print_src_version(void);

END_C_DECLS

#endif /* ndef LIBSWOCCLIENT_H */
