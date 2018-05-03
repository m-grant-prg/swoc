/**
 * @file cli-prg/src/prg/c/inc/cmdlineargs.h
 *
 * Header file for command line processing.
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 12/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 09/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 13/06/2016	MG	1.0.3	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.4	Move towards kernel coding style.	*
 * 07/05/2017	MG	1.0.5	Add --wait as a command line argument.	*
 * 12/11/2017	MG	1.0.6	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H


#include <limits.h>

#include <portability.h>

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
	char argument[ARG_BUF];	/**< A possible argument to the flag. */
};

int process_cla(int argc, char **argv, struct cla *block_flag,
		struct cla *lock_flag, struct cla *release_flag,
		struct cla *reset_flag,struct cla *status_flag,
		struct cla *unblock_flag, struct cla *wait_flag);

int cpyarg(char *flagarg, char *srcarg);

END_C_DECLS

#endif /* ndef CMDLINEARGS_H */
