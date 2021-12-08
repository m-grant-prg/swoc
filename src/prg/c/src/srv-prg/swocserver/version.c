/**
 * @file src/prg/c/src/srv-prg/swocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2019, 2021  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.7 ==== 08/12/2021_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 17/07/2016	MG	1.0.2	Move towards kernel coding style.	*
 * 17/11/2017	MG	1.0.3	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 05/06/2018	MG	1.0.4	Make this ChangeLog for this file only.	*
 * 24/06/2018	MG	1.0.5	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 * 18/05/2019	MG	1.0.6	Merge sub-projects into one.		*
 * 08/12/2021	MG	1.0.7	Tighten SPDX tag.			*
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
char *swocserver_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *swocserver_get_src_version(void)
{
	return SWOCSERVER_SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void swocserver_print_pkg_version(void)
{
	printf("%s %s %s", "swocserver Package version -", PACKAGE_VERSION,
	       "\n");
}

/**
 * Print the source version string to stdout.
 */
void swocserver_print_src_version(void)
{
	printf("%s %s %s", "swocserver Source version -",
	       SWOCSERVER_SOURCE_VERSION, "\n");
}

