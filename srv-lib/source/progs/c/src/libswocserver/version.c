/**
 * @file srv-lib/source/progs/c/src/libswocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2016-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 12/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 25/05/2016	MG	1.0.1	First release.Separated out from	*
 *				swocserver-c package.			*
 * 12/06/2016	MG	1.0.2	Make publicly visible function names	*
 *				more unique.				*
 * 16/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 17/07/2016	MG	1.0.4	Further coding style changes.		*
 * 06/05/2017	MG	1.1.0	Convert from NFS 'file as a flag'	*
 *				semaphore to TCP socket messaging to	*
 *				new server daemon.			*
 *				Migrate to use of mge_errno error	*
 *				handling from libmgec.			*
 *				Print the number of clients with locks	*
 *				in sws_show_status().			*
 * 12/09/2017	MG	1.1.1	Change sws_force_unlock() to		*
 *				sws_unlock().				*
 * 15/09/2017	MG	1.1.2	Change refernces to ssl to tls.		*
 * 12/11/2017	MG	1.1.3	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <libswocserver.h>
#include <config.h>

/** The source code version. */
#define SOURCE_VERSION "1.1.3"

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
