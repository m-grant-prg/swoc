/**
 * @file src/prg/c/src/cli-prg/swocclient/cmdlineargs.c
 *
 * Command line argument processing for swocclient using getopt_long.
 *
 * @author Copyright (C) 2015-2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.11 ==== 01/04/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 13/06/2016	MG	1.0.2	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 07/05/2017	MG	1.1.0	Migrate to use mge_errno and handling	*
 *				from libmgec. Add error message output	*
 *				to prevent silence on error.		*
 *				Add --wait as a command line argument.	*
 * 12/11/2017	MG	1.1.1	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 30/01/2018	MG	1.1.2	Use fprintf for error messages.		*
 * 02/02/2018	MG	1.1.3	Allow wait to take an optional number	*
 *				of locks argument (default 0).		*
 * 28/03/2018	MG	1.1.4	For pointers assigned zero in option	*
 *				struct, assign NULL instead.		*
 * 02/05/2018	MG	1.1.5	Add support for client block list.	*
 * 22/05/2018	MG	1.1.6	Make more generic for copy and paste	*
 *				re-usability by using function with a	*
 *				variable number of arguments.		*
 *				Simplify src directory structure and	*
 *				header file location.			*
 * 18/05/2019	MG	1.1.7	Merge sub-projects into one.		*
 * 27/03/2020	MG	1.1.8	Move into swocclient sub-directory as	*
 *				the directory hierarchy needs to be the	*
 *				same accross the source tree for	*
 *				temporary libraries to work based on	*
 *				the search in configure.ac.		*
 * 11/10/2021	MG	1.1.9	Move cmdlineargs.h to inc directory.	*
 * 08/12/2021	MG	1.1.10	Tighten SPDX tag.			*
 * 01/04/2022	MG	1.1.11	Improve error handling consistency.	*
 *									*
 ************************************************************************
 */

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include <cmdlineargs.h>
#include <mge-errno.h>

/**
 * Process command line arguments using getopt_long.
 * On error mge_errno will be set.
 * @param argc The standard CLA argc.
 * @param argv The standard CLA argv.
 * @param ... Variable number of flag structs.
 * @return 0 on success, -mge_errno on failure.
 */
int process_cla(int argc, char **argv, ...)
{
	va_list ap;

	/* Command line argument flags. */
	struct cla *block_flag, *lock_flag, *release_flag, *reset_flag;
	struct cla *status_flag, *unblock_flag, *wait_flag;

	//* getopt_long stores the option index here. */
	int option_index = 0;
	int c;
	int x;

	struct option long_options[]
		= { { "block", no_argument, NULL, 'b' },
		    { "help", no_argument, NULL, 'h' },
		    { "lock", no_argument, NULL, 'l' },
		    { "release", no_argument, NULL, 'r' },
		    { "reset", no_argument, NULL, 'i' },
		    { "status", no_argument, NULL, 's' },
		    { "unblock", no_argument, NULL, 'u' },
		    { "version", no_argument, NULL, 'V' },
		    { "wait", optional_argument, NULL, 'w' },
		    { NULL, 0, NULL, 0 } };

	va_start(ap, argv);
	block_flag = va_arg(ap, struct cla *);
	lock_flag = va_arg(ap, struct cla *);
	release_flag = va_arg(ap, struct cla *);
	reset_flag = va_arg(ap, struct cla *);
	status_flag = va_arg(ap, struct cla *);
	unblock_flag = va_arg(ap, struct cla *);
	wait_flag = va_arg(ap, struct cla *);
	va_end(ap);

	while ((c = getopt_long(argc, argv, "bhilrsuVw::", long_options,
				&option_index))
	       != -1) {
		switch (c) {
		case 'b':
			if (lock_flag->is_set || release_flag->is_set
			    || reset_flag->is_set || status_flag->is_set
			    || unblock_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			block_flag->is_set = 1;
			break;
		case 'i':
			if (block_flag->is_set || lock_flag->is_set
			    || release_flag->is_set || status_flag->is_set
			    || unblock_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			reset_flag->is_set = 1;
			break;
		case 'h':
			printf("%s %s", argv[0], " - Help option.\n");
			printf("\tLong and short options can be mixed on the "
			       "command line but if an option takes an "
			       "optional argument it is best to enter "
			       "-o\"argument\" or --option=argument.\n");
			printf("-b | --block\tBlock client on server.\n");
			printf("-i | --reset\tSet locks to 0 and unblock "
			       "client on server.\n");
			printf("-l | --lock\tSet client lock on server.\n");
			printf("-r | --release\tRelease client lock on "
			       "server.\n");
			printf("-u | --unblock\tUnblock client on server.\n");
			printf("-s | --status\tShow the locking status.\n");
			printf("-V | --version\tDisplay version "
			       "information.\n");
			printf("-w[NumLocks] | --wait[=NumLocks]\tWait until "
			       "this client has NumLocks or fewer locks.\n");
			exit(0);
			break;
		case 'l':
			if (block_flag->is_set || release_flag->is_set
			    || reset_flag->is_set || status_flag->is_set
			    || unblock_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			lock_flag->is_set = 1;
			break;

		case 'r':
			if (block_flag->is_set || lock_flag->is_set
			    || reset_flag->is_set || status_flag->is_set
			    || unblock_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			release_flag->is_set = 1;
			break;
		case 'u':
			if (block_flag->is_set || lock_flag->is_set
			    || release_flag->is_set || reset_flag->is_set
			    || status_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			unblock_flag->is_set = 1;
			break;
		case 's':
			if (block_flag->is_set || lock_flag->is_set
			    || release_flag->is_set || reset_flag->is_set
			    || unblock_flag->is_set || wait_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			status_flag->is_set = 1;
			break;
		case 'V':
			printf("%s %s %s %s", argv[0], "Source version -",
			       swocclient_get_src_version(), "\n");
			printf("%s %s %s %s", argv[0], "Package version -",
			       swocclient_get_pkg_version(), "\n");
			exit(0);
			break;
		case 'w':
			if (block_flag->is_set || lock_flag->is_set
			    || release_flag->is_set || reset_flag->is_set
			    || status_flag->is_set || unblock_flag->is_set) {
				fprintf(stderr, "Options b, i, l, r, s, u and "
						"w are mutually exclusive.\n");
				mge_errno = MGE_PARAM;
				return -mge_errno;
			}
			wait_flag->is_set = 1;
			strcpy(wait_flag->argument, "0");
			if (!optarg)
				break;
			x = cpyarg(wait_flag->argument, optarg);
			if (x)
				return x;
			break;
		case '?':
			/* getopt_long already printed an error message. */
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

	/* Check for mandatory options */
	if (!(block_flag->is_set || lock_flag->is_set || release_flag->is_set
	      || reset_flag->is_set || status_flag->is_set
	      || unblock_flag->is_set || wait_flag->is_set)) {
		fprintf(stderr, "Either b, i, l, r, s, u or w must be "
				"specified.\n");
		mge_errno = MGE_PARAM;
		return -mge_errno;
	}
	return 0;
}

/*
 * Function to copy optarg to the flag struct member argument.
 * On error mge_errno will be set.
 */
int cpyarg(char *flagarg, char *srcarg)
{
	if (ARG_BUF > strlen(srcarg)) {
		strcpy(flagarg, srcarg);
		return 0;
	} else {
		fprintf(stderr, "Option argument '%s' too long.\n", srcarg);
		mge_errno = MGE_PARAM;
		return -mge_errno;
	}
}
