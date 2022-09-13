/**
 * @file src/prg/c/src/srv-prg/swocserverd/cmdlineargs.c
 *
 * Command line argument processing for swocserverd using getopt_long.
 *
 * @author Copyright (C) 2016-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.10 ==== 12/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.1	First release.				*
 * 04/06/2017	MG	1.0.2	Tidy up unnecessary include statements.	*
 * 18/11/2017	MG	1.0.3	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 28/03/2018	MG	1.0.4	Replace initialising pointers to zero	*
 *				with NULL. (fixes sparse warnings).	*
 * 22/05/2018	MG	1.0.5	Change from swocserverd.h to internal.h	*
 * 18/05/2019	MG	1.0.6	Merge sub-projects into one.		*
 * 13/10/2021	MG	1.0.7	Eliminate a Doxygen warning.		*
 * 08/12/2021	MG	1.0.8	Tighten SPDX tag.			*
 * 05/04/2022	MG	1.0.9	Improve error handling consistency.	*
 * 12/09/2022	MG	1.0.10	Use pkginclude location.		*
 *									*
 ************************************************************************
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "internal.h"
#include <cmdlineargs.h>
#include <libmgec/mge-errno.h>

/**
 * Process command line arguments using getopt_long.
 * On error mge_errno will be set.
 * @param argc The standard CLA argc.
 * @param argv The standard CLA argv.
 * @param ... Variable number of flag structs.
 * @return 0 on success, -mge_errno on failure.
 */
/*
 * The variable number of flag structs parameter above is not used in this
 * implementation. The Doxygen comment must be present to avoid Doxygen
 * warnings.
 */
int process_cla(int argc, char **argv, ...)
{
	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	struct option long_options[] = { { "debug", no_argument, NULL, 'D' },
					 { "help", no_argument, NULL, 'h' },
					 { "version", no_argument, NULL, 'V' },
					 { NULL, 0, NULL, 0 } };

	while ((c = getopt_long(argc, argv, "DhV", long_options, &option_index))
	       != -1) {
		switch (c) {
		case 'D':
			debug = 1;
			break;

		case 'h':
			printf("%s %s", argv[0], " - Help option.\n");
			printf("\tLong and short options can be mixed on the "
			       "command line but if an option\ntakes an "
			       "optional argument it is best to enter "
			       "-o\"argument\" or\n--option=argument.\n");
			printf("-D | --debug\tDon't daemonise. Output "
			       "information to stdout and stderr.\n");
			printf("-V | --version\tDisplay version "
			       "information.\n");
			exit(0);
			break;

		case 'V':
			printf("%s %s %s %s", argv[0], "Source version -",
			       swocserverd_get_src_version(), "\n");
			printf("%s %s %s %s", argv[0], "Package version -",
			       swocserverd_get_pkg_version(), "\n");
			exit(0);
			break;

		case '?':
			/* getopt_long already printed a message. */
			mge_errno = MGE_PARAM;
			return -mge_errno;
			break;

		default:
			abort();
		}
	}

	/* Non-option arguments are not accepted. */
	if (optind < argc) {
		fprintf(stderr, "Program does not accept other arguments.\n");
		mge_errno = MGE_PARAM;
		return -mge_errno;
	}
	return 0;
}
