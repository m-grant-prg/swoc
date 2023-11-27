/**
 * @file src/prg/c/src/cli-prg/cmdlineargs.c
 *
 * Command line argument processing for swocclient using getopt_long.
 *
 * @author Copyright (C) 2015-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 26/11/2023_
 */

#include <getopt.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include <libmgec/mge-errno.h>
#include <swoc/cmdlineargs.h>

static void usage(char **argv);

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
			usage(argv);
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
	if (ARG_BUF <= strlen(srcarg)) {
		fprintf(stderr, "Option argument '%s' too long.\n", srcarg);
		mge_errno = MGE_PARAM;
		return -mge_errno;
	} else {
		strcpy(flagarg, srcarg);
		return 0;
	}
}

/*
 * Display help text.
 */
static void usage(char **argv)
{
	char *argv_copy, *base_name;

	argv_copy = strdup(argv[0]);
	base_name = basename(argv_copy);
	printf("Usage is:-\n");
	printf("%s %s", base_name, "{-b|-i|-l|-r|-u|-s|-w[NUMLOCKS]}\n");
	printf("%s %s", base_name, "{-h|-V}\n");
	printf("\nUsage is:-\n");
	printf("%s %s", base_name, "[OPTIONS]\n");
	printf("\t-b | --block\tBlock client on server.\n");
	printf("\t-i | --reset\tSet locks to 0 and unblock client on"
	       " server.\n");
	printf("\t-l | --lock\tSet client lock on server.\n");
	printf("\t-r | --release\tRelease client lock on server.\n");
	printf("\t-u | --unblock\tUnblock client on server.\n");
	printf("\t-s | --status\tShow the locking status.\n");
	printf("\t-V | --version\tDisplay version information.\n");
	printf("\t-w[NUMLOCKS] | --wait[{ |=}NUMLOCKS]\n"
	       "\t\t\tWait until this client has NUMLOCKS or fewer locks.\n"
	       "\t\t\tIf not specified, NUMLOCKS defaults to 0.\n");
}
