/**
 * @file cli-prg/src/prg/c/src/swocclient/cmdlineargs.c
 *
 * Command line argument processing for swocclient using getopt_long.
 *
 * @author Copyright (C) 2015-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.1.3 ==== 02/02/2018_
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
 *									*
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <swocclient.h>
#include <cmdlineargs.h>


/**
 * Process command line arguments using getopt_long.
 * @param argc The standard CLA argc.
 * @param argv The standard CLA argv.
 * @param lock_flag Struct for the lock flag.
 * @param release_flag Struct for the release flag.
 * @param status_flag Struct for the status flag.
 * @param wait_flag Struct for the wait flag.
 * @return 0 on success, on failure standard EX_USAGE (64) command line  usage
 * error.
 */
int process_cla(int argc, char **argv, struct cla *lock_flag,
		struct cla *release_flag, struct cla *status_flag,
		struct cla *wait_flag)
{
	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;
	int x;

	struct option long_options[] = {
		{"help",	no_argument,		0,	'h'},
		{"lock",	no_argument,		0,	'l'},
		{"release",	no_argument,		0,	'r'},
		{"status",	no_argument,		0,	's'},
		{"version",	no_argument,		0,	'V'},
		{"wait",	optional_argument,	0,	'w'},
		{0,		0,			0,	0}
	};

	while ((c = getopt_long(argc, argv, "hlrsVw::",
		long_options, &option_index)) != -1) {

		switch (c) {
		case 'h':
			printf("%s %s", argv[0], " - Help option.\n");
			printf("\tLong and short options can be mixed on the "
				"command line but if an option takes an "
				"optional argument it is best to enter "
				"-o\"argument\" or --option=argument.\n");
			printf("-l | --lock\tSet client lock on server.\n");
			printf("-r | --release\tRelease client lock on "
				"server.\n");
			printf("-s | --status\tShow the locking status.\n");
			printf("-V | --version\tDisplay version "
				"information.\n");
			printf("-w[NumLocks] | --wait[=NumLocks]\tWait until "
				"this client has NumLocks or fewer locks.\n");
			exit(0);
			break;

		case 'l':
			if (release_flag->is_set
				|| status_flag->is_set
				|| wait_flag->is_set) {
				fprintf(stderr, "Options l, r, s and w are "
				"mutually exclusive.\n");
				return 64;
			}
			lock_flag->is_set = 1;
			break;

		case 'r':
			if (lock_flag->is_set || status_flag->is_set
				|| wait_flag->is_set) {
				fprintf(stderr, "Options l, r, s and w are "
				"mutually exclusive.\n");
				return 64;
			}
			release_flag->is_set = 1;
			break;

		case 's':
			if (release_flag->is_set || lock_flag->is_set
				|| wait_flag->is_set) {
				fprintf(stderr, "Options l, r, s and w are "
				"mutually exclusive.\n");
				return 64;
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
			if (release_flag->is_set || lock_flag->is_set
				|| status_flag->is_set) {
				fprintf(stderr, "Options l, r, s and w are "
				"mutually exclusive.\n");
				return 64;
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
			return 64;
			break;

		default:
			abort();
		}
	}

	/* Non-option arguments are not accepted. */
	if (optind < argc) {
		fprintf(stderr, "Program does not accept other arguments.\n");
		return 64;
	}

	/* Check for mandatory options */
	if (!(lock_flag->is_set || release_flag->is_set
		|| status_flag->is_set || wait_flag->is_set)) {
		fprintf(stderr, "Either l, r, s or w must be specified.\n");
		return 64;
	}
	return 0;
}

/*
 * Function to copy optarg to the flag struct member argument.
 */
int cpyarg(char *flagarg, char *srcarg)
{
	int e = 0;
	if (ARG_BUF > strlen(srcarg))
		strcpy(flagarg, srcarg);
	else
	{
		fprintf(stderr, "Option argument '%s' too long.\n", srcarg);
		e = 64;
	}
	return e;
}
