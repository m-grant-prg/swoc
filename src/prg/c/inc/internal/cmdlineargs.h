/**
 * @file src/prg/c/inc/internal/cmdlineargs.h
 *
 * Header file for command line processing.
 *
 * @author Copyright (C) 2015-2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.12 ==== 08/09/2022_
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
 * 22/05/2018	MG	1.0.7	Make more generic for copy and paste	*
 *				re-usability by using function with a	*
 *				variable number of arguments.		*
 * 18/05/2019	MG	1.0.8	Merge sub-projects into one.		*
 * 27/03/2020	MG	1.0.9	Move into swocclient sub-directory as	*
 *				the directory hierarchy needs to be the	*
 *				same accross the source tree for	*
 *				temporary libraries to work based on	*
 *				the search in configure.ac.		*
 * 11/10/2021	MG	1.0.10	Move to internal directory.		*
 *				Merge server and client versions.	*
 * 08/12/2021	MG	1.0.11	Tighten SPDX tag.			*
 * 08/09/2022	MG	1.0.12	Rename portability.h			*
 *									*
 ************************************************************************
 */

#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <limits.h>

#include <mge-portability.h>

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
