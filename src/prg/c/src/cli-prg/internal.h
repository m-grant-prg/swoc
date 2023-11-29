/**
 * @file src/prg/c/src/cli-prg/internal.h
 *
 * Header file for Server Wait on Clients client-side program.
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#ifndef SWOCCLIENT_INTERNAL_H
#define SWOCCLIENT_INTERNAL_H

#include <libmgec/mge-portability.h>

BEGIN_C_DECLS

__attribute__((const)) const char *swocclient_get_pkg_version(void);

__attribute__((const)) const char *swocclient_get_src_version(void);

void swocclient_print_pkg_version(void);

void swocclient_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCCLIENT_INTERNAL_H */
