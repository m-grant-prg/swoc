/**
 * @file src/prg/c/src/srv-prg/swocserverd/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.5 ==== 18/05/2019_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.1	First release.				*
 * 18/11/2017	MG	1.0.2	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 05/06/2018	MG	1.0.3	Make this ChangeLog for this file only.	*
 * 24/06/2018	MG	1.0.4	Remove SOURCE_CODE define, now uses the	*
 *				AC_DEFINE'd value in config.h		*
 * 18/05/2019	MG	1.0.5	Merge sub-projects into one.		*
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
char *swocserverd_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *swocserverd_get_src_version(void)
{
	return SWOCSERVERD_SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void swocserverd_print_pkg_version(void)
{
	printf("%s %s %s", "swocserverd Package version -", PACKAGE_VERSION,
	       "\n");
}

/**
 * Print the source version string to stdout.
 */
void swocserverd_print_src_version(void)
{
	printf("%s %s %s", "swocserverd Source version -",
	       SWOCSERVERD_SOURCE_VERSION, "\n");
}

