/**
 * @file src/prg/c/src/cli-prg/swocclient/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2020  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.9 ==== 27/03/2020_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 13/06/2016	MG	1.0.2	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 12/11/2017	MG	1.0.4	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 22/05/2018	MG	1.0.5	Simplify src directory structure.	*
 * 05/06/2018	MG	1.0.6	Make this ChangeLog for this file only.	*
 * 23/06/2018	MG	1.0.7	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 * 18/05/2019	MG	1.0.8	Merge sub-projects into one.		*
 * 27/03/2020	MG	1.0.9	Move into swocclient sub-directory as	*
 *				the directory hierarchy needs to be the	*
 *				same accross the source tree for	*
 *				temporary libraries to work based on	*
 *				the search in configure.ac.		*
 *									*
 ************************************************************************
 */

#include <config.h>
#include <stdio.h>

#include "internal.h"

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
	return SWOCCLIENT_SOURCE_VERSION;
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
	printf("%s %s %s", "swocclient Source version -",
	       SWOCCLIENT_SOURCE_VERSION, "\n");
}

