/**
 * @file src/prg/c/src/srv-prg/swocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2019, 2021, 2023  Mark Grant
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
__attribute__((const)) const char *swocserver_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
__attribute__((const)) const char *swocserver_get_src_version(void)
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
