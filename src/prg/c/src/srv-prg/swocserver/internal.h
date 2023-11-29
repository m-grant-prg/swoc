/**
 * @file src/prg/c/src/srv-prg/swocserver/internal.h
 *
 * Header file for Server Wait on Clients server-side program.
 *
 * @author Copyright (C) 2015-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#ifndef SWOCSERVER_INTERNAL_H
#define SWOCSERVER_INTERNAL_H

#include <libmgec/mge-portability.h>
#include <swoc/libswocserver.h>

BEGIN_C_DECLS

extern int sws_err;

__attribute__((const)) const char *swocserver_get_pkg_version(void);

__attribute__((const)) const char *swocserver_get_src_version(void);

void swocserver_print_pkg_version(void);

void swocserver_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCSERVER_INTERNAL_H */
