/**
 * @file srv-lib/src/prg/c/src/libswocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.5 ==== 05/06/2018_
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
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <libswocserver.h>
#include <config.h>

/** The source code version. */
#define SOURCE_VERSION "1.1.4"

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
	return SOURCE_VERSION;
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
	printf("%s %s %s", "libswocserver Source version -", SOURCE_VERSION,
		"\n");
}
