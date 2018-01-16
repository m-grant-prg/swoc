/**
 * @file srv-prg/src/prg/c/src/swocserverd/cmdlineargs.c
 *
 * Command line argument processing for swocserverd using getopt_long.
 *
 * @author Copyright (C) 2016-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.3 ==== 18/11/2017_
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
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <getopt.h>

#include <swocserverd.h>
#include <cmdlineargs.h>


/**
 * Process command line arguments using getopt_long.
 * Variable length argument list comprising of the flag structs. In this program
 * there are none.
 * @param argc The standard CLA argc.
 * @param argv The standard CLA argv.
 * @return 0 on success, on failure standard EX_USAGE (64) command line  usage
 * error.
 */
int process_cla(int argc, char **argv, ...)
{
	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	struct option long_options[] = {
		{"debug",		no_argument,		0,	'D'},
		{"help",		no_argument,		0,	'h'},
		{"version",		no_argument,		0,	'V'},
		{0,			0,			0,	0}
	};

	while ((c = getopt_long(argc, argv, "DhV",
		long_options, &option_index)) != -1) {

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
			if (!swsd_err)
				swsd_err = -1;
			break;

		case 'V':
			printf("%s %s %s %s", argv[0], "Source version -",
				swocserverd_get_src_version(), "\n");
			printf("%s %s %s %s", argv[0], "Package version -",
				swocserverd_get_pkg_version(), "\n");
			if (!swsd_err)
				swsd_err = -1;
			break;

		case '?':
			/* getopt_long already printed a message. */
			swsd_err = EX_USAGE;
			break;

		default:
			abort();
		}
	}

	/* Non-option arguments are not accepted. */
	if (optind < argc) {
		fprintf(stderr, "Program does not accept other arguments.\n");
		swsd_err = EX_USAGE;
	}
	if (swsd_err == -1) {
		swsd_err = 0;
		return 1;
	}
	return swsd_err;
}
