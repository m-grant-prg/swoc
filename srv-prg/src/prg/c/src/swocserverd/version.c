/**
 * @file srv-prg/src/prg/c/src/swocserverd/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.3 ==== 22/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.0	First release.				*
 * 18/11/2017	MG	1.0.1	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 * 28/03/2018	MG	1.0.2	Include swocserverd.h for function	*
 *				prototypes, (fixes sparse warnings).	*
 * 22/05/2018	MG	1.0.3	Change from swocserverd.h to internal.h	*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <config.h>

#include "internal.h"


/** The source code version. */
#define SOURCE_VERSION "1.0.1"

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
	return SOURCE_VERSION;
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
	printf("%s %s %s", "swocserverd Source version -", SOURCE_VERSION,
		"\n");
}
