/**
 * @file src/prg/c/src/srv-prg/swocserverd/cmdlineargs.c
 *
 * Command line argument processing for swocserverd using getopt_long.
 *
 * @author Copyright (C) 2016-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include <libmgec/mge-errno.h>
#include <swoc/cmdlineargs.h>

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
	char *argv_copy, *base_name;

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
			argv_copy = strdup(argv[0]);
			base_name = basename(argv_copy);
			printf("Usage is:-\n");
			printf("%s %s", base_name, "[-D]\n");
			printf("%s %s", base_name, "{-h|-V}\n");
			printf("\nUsage is:-\n");
			printf("%s %s", base_name, "[OPTIONS]\n");
			printf("\t-D | --debug\tDon't daemonise. Output "
			       "information to stdout and \n\t\t\tstderr.\n");
			printf("\t-V | --version\tDisplay version "
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
