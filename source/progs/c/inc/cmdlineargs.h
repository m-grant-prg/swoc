/**
 * @file srv-prg/source/progs/c/inc/cmdlineargs.h
 *
 * Header file for command line processing.
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.5 ==== 17/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 10/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 17/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.4	Further coding style changes.		*
 *				Improve in-source documentation.	*
 *				Enable cmdlineargs support for multiple	*
 *				command line programs in a single	*
 *				project.				*
 * 17/11/2017	MG	1.0.5	Add Doxygen comments.			*
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
 * Use maximum path length as size of argument for want of a more meaningful
 * value.
 */
#define ARG_BUF PATH_MAX

/** Command line argument. */
struct cla {
	int is_set;			/**< Is this option set on the CL. */
	char argument[ARG_BUF];		/**< Argument to the option. */
};

int process_cla(int argc, char **argv, ...);

END_C_DECLS

#endif /* ndef CMDLINEARGS_H */
