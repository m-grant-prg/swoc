/**
 * @file src/prg/c/inc/swoc/signalhandle.h
 *
 * Header file for signal handling.
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 25/11/2023_
 */

#ifndef SIGNALHANDLE_H
#define SIGNALHANDLE_H

#include <libmgec/mge-portability.h>

BEGIN_C_DECLS

void init_sig_handle(void);

void termination_handler(int signum);

END_C_DECLS

#endif /* ndef SIGNALHANDLE_H */
