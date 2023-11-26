/**
 * @file src/prg/c/src/cli-prg/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <config.h>

#include <stdio.h>

#include "internal.h"

/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *swocclient_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *swocclient_get_src_version(void)
{
	return SWOCCLIENT_SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void swocclient_print_pkg_version(void)
{
	printf("%s %s %s", "swocclient Package version -", PACKAGE_VERSION,
	       "\n");
}

/**
 * Print the source version string to stdout.
 */
void swocclient_print_src_version(void)
{
	printf("%s %s %s", "swocclient Source version -",
	       SWOCCLIENT_SOURCE_VERSION, "\n");
}
