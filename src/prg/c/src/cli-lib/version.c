/**
 * @file src/prg/c/src/cli-lib/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.10 ==== 17/09/2022_
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
 * 22/06/2018	MG	1.0.7	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 * 18/05/2019	MG	1.0.8	Merge sub-projects into one.		*
 * 08/12/2021	MG	1.0.9	Tighten SPDX tag.			*
 * 17/09/2022	MG	1.0.10	Correct included headers.		*
 *				Flatten directory structure.		*
 *									*
 ************************************************************************
 */

#include <stdio.h>

#include <config.h>

#include <swoc/libswocclient.h>

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
	return CLIENT_SOURCE_VERSION;
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
	printf("%s %s %s", "libswocclient Source version -",
	       CLIENT_SOURCE_VERSION, "\n");
}
