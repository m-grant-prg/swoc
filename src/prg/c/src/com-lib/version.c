/**
 * @file src/prg/c/src/com-lib/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2017-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.8 ==== 17/09/2022_
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
 * 18/05/2019	MG	1.0.5	Merge sub-projects into one.		*
 * 10/10/2021	MG	1.0.6	Use newly internalised common header.	*
 * 08/12/2021	MG	1.0.7	Tighten SPDX tag.			*
 * 17/09/2022	MG	1.0.8	Correct included headers.		*
 *				Flatten directory structure.		*
 *									*
 ************************************************************************
 */

#include <stdio.h>

#include <config.h>

#include <swoc/libswoccommon.h>

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
	return COMMON_SOURCE_VERSION;
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
	printf("%s %s %s", "libswoccommon Source version -",
	       COMMON_SOURCE_VERSION, "\n");
}
