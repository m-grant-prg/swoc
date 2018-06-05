/**
 * @file cli-lib/src/prg/c/src/libswocclient/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 05/06/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 11/06/2016	MG	1.0.1	First release.Separated out from	*
 *				swocclient-c package.			*
 * 12/06/2016	MG	1.0.2	Adopt convention of function definition	*
 *				with empty parameter list to use void,	*
 *				just as with prototypes.		*
 * 16/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 17/07/2016	MG	1.0.4	Further coding style changes.		*
 * 11/11/2017	MG	1.0.5	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 05/06/2018	MG	1.0.6	Make this ChangeLog for this file only.	*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <libswocclient.h>
#include <config.h>

/** The source code version. */
#define SOURCE_VERSION "1.1.1"

/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *libswocclient_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *libswocclient_get_src_version(void)
{
	return SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void libswocclient_print_pkg_version(void)
{
	printf("%s %s %s", "libswocclient Package version -", PACKAGE_VERSION,
		"\n");
}

/**
 * Print the source version string to stdout.
 */
void libswocclient_print_src_version(void)
{
	printf("%s %s %s", "libswocclient Source version -", SOURCE_VERSION,
		"\n");
}
