/**
 * @file src/prg/c/src/cli-lib/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <stdio.h>

#include <config.h>

#include <swoc/libswocclient.h>

/**
 * Get the git-describe based package version.
 * @return The package version string.
 */
char *libswocclient_get_pkg_version(void)
{
	return PACKAGE_VERSION;
}

/**
 * Get the source version.
 * @return The source version string.
 */
char *libswocclient_get_src_version(void)
{
	return CLIENT_SOURCE_VERSION;
}

/**
 * Print the package version string to stdout.
 */
void libswocclient_print_pkg_version(void)
{
	printf("%s %s %s", "libswocclient Package version -", PACKAGE_VERSION,
	       "\n");
}

/**
 * Print the source version string to stdout.
 */
void libswocclient_print_src_version(void)
{
	printf("%s %s %s", "libswocclient Source version -",
	       CLIENT_SOURCE_VERSION, "\n");
}
