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
 * @version _v1.0.4 ==== 22/06/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 05/05/2017	MG	1.0.1	First release.				*
 * 10/11/2017	MG	1.0.2	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 05/06/2018	MG	1.0.3	Make this ChangeLog for this file only.	*
 * 22/06/2018	MG	1.0.4	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <libswoccommon.h>
#include <config.h>


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
