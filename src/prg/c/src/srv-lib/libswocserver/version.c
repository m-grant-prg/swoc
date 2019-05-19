/**
 * @file src/prg/c/src/srv-lib/libswocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.7 ==== 18/05/2019_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 25/05/2016	MG	1.0.1	First release.				*
 * 16/07/2016	MG	1.0.2	Move towards kernel coding style.	*
 * 17/07/2016	MG	1.0.3	Further coding style changes.		*
 * 12/11/2017	MG	1.0.4	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 05/06/2018	MG	1.0.5	Make this ChangeLog for this file only.	*
 * 23/06/2018	MG	1.0.6	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 * 18/05/2019	MG	1.0.7	Merge sub-projects into one.		*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <libswocserver.h>
#include <config.h>


/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *libswocserver_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *libswocserver_get_src_version(void)
{
	return SERVER_SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void libswocserver_print_pkg_version(void)
{
	printf("%s %s %s", "libswocserver Package version -", PACKAGE_VERSION,
		"\n");
}

/**
 * Print the source version string to stdout.
 */
void libswocserver_print_src_version(void)
{
	printf("%s %s %s", "libswocserver Source version -",
		SERVER_SOURCE_VERSION, "\n");
}

