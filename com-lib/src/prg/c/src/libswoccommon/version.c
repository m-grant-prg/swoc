/**
 * @file com-lib/src/prg/c/src/libswoccommon/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2017-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.2 ==== 06/03/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 05/05/2017	MG	1.0.0	First release.				*
 * 15/09/2017	MG	1.0.1	Change force-unlock option to unlock.	*
 * 19/10/2017	MG	1.1.0	Add support for SSH.			*
 * 10/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 06/03/2018	MG	1.1.2	When establishing a TCP connection,	*
 *				another client or server comms session	*
 *				could be taking place. Therefore allow	*
 *				bind and connect 10 tries if the	*
 *				address is in use.			*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <libswoccommon.h>
#include <config.h>

/** The source code version. */
#define SOURCE_VERSION "1.1.2"

/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *libswoccommon_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *libswoccommon_get_src_version(void)
{
	return SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void libswoccommon_print_pkg_version(void)
{
	printf("%s %s %s", "libswoccommon Package version -", PACKAGE_VERSION,
		"\n");
}

/**
 * Print the source version string to stdout.
 */
void libswoccommon_print_src_version(void)
{
	printf("%s %s %s", "libswoccommon Source version -", SOURCE_VERSION,
		"\n");
}
