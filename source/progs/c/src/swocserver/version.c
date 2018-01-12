/**
 * @file srv-prg/source/progs/c/src/swocserver/version.c
 *
 * Source for version functions.
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.2 ==== 17/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 10/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 16/05/2016	MG	1.0.3	Add --force-unlock option.		*
 *				Make --status and --wait ignore		*
 *				directories.				*
 * 28/05/2016	MG	1.0.4	Modified following introduction of	*
 *				libswocserver library.			*
 * 13/06/2016	MG	1.0.5	Use new more unique library function	*
 *				names.					*
 * 17/07/2016	MG	1.0.6	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.7	Further coding style changes.		*
 *				Improve in-source documentation.	*
 *				Enable cmdlineargs support for multiple	*
 *				command line programs in a single	*
 *				project.				*
 *				Simplify cmdlineargs error handling.	*
 *				Use more informative name - sws_err.	*
 * 06/05/2017	MG	1.1.0	Migrate from NFS 'file as a flag' 	*
 *				semaphore to TCP socket stream		*
 *				messaging between applications and	*
 *				daemon.					*
 *				Add end_daemon option to command line	*
 *				program.				*
 * 12/09/2017	MG	1.1.1	Change 'force unlock' to just 'unlock'.	*
 * 17/11/2017	MG	1.1.2	Add Doxygen comments.			*
 *				Add SPDX license tags.			*
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <config.h>

/** The source code version. */
#define SOURCE_VERSION "1.1.2"

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
	return SOURCE_VERSION;
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
	printf("%s %s %s", "swocserver Source version -", SOURCE_VERSION, "\n");
}
