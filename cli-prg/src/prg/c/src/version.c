/**
 * @file cli-prg/src/prg/c/src/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 22/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 08/07/2015	MG	1.0.2	Extracted remsyslog.h to a seperate	*
 *				libremsyslog-dev package.		*
 * 10/07/2015	MG	1.0.3	remsyslog extracted to libremsyslog	*
 *				library package.			*
 * 09/05/2016	MG	1.0.4	Move header files to include directory.	*
 * 11/06/2016	MG	1.0.5	Modified following introduction of	*
 *				libswocclient library.			*
 * 13/06/2016	MG	1.0.6	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.7	Move towards kernel coding style.	*
 * 07/05/2017	MG	1.1.0	Migrate to use mge_errno and handling	*
 *				from libmgec. Add error message output	*
 *				to prevent silence on error.		*
 *				Add --wait as a command line argument.	*
 * 12/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 28/03/2018	MG	1.1.2	Add swocclient.h, (fixes sparse		*
 *				warnings).				*
 * 22/05/2018	MG	1.1.3	Simplify src directory structure and	*
 *				header file location.			*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <config.h>

#include "internal.h"


/** The source code version. */
#define SOURCE_VERSION "1.1.1"

/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *swocclient_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *swocclient_get_src_version(void)
{
	return SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void swocclient_print_pkg_version(void)
{
	printf("%s %s %s", "swocclient Package version -", PACKAGE_VERSION,
		"\n");
}

/**
 * Print the source version string to stdout.
 */
void swocclient_print_src_version(void)
{
	printf("%s %s %s", "swocclient Source version -", SOURCE_VERSION, "\n");
}
