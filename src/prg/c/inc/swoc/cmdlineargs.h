/**
 * @file src/prg/c/inc/swoc/cmdlineargs.h
 *
 * Header file for command line processing.
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 25/11/2023_
 */

#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <limits.h>

#include <libmgec/mge-portability.h>

BEGIN_C_DECLS

/**
 * Argument buffer size.
 * Use maximum path length as size of argument for want of a more meaningful
 * value.
 */
#define ARG_BUF PATH_MAX

/** Command line argument. */
struct cla {
	int is_set;		/**< Flag is set, 1 is true, 0 is false. */
	char argument[ARG_BUF]; /**< A possible argument to the flag. */
};

int process_cla(int argc, char **argv, ...);

int cpyarg(char *flagarg, char *srcarg);

END_C_DECLS

#endif /* ndef CMDLINEARGS_H */
