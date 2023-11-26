/**
 * @file src/prg/c/src/srv-prg/swocserver/cmdlineargs.c
 *
 * Command line argument processing for swocserver using getopt_long.
 *
 * @author Copyright (C) 2015-2019, 2021-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.2.0 ==== 26/11/2023_
 */

#include <getopt.h>
#include <stdarg.h>
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
int process_cla(int argc, char **argv, ...)
{
	va_list ap;

	/* Command line argument flags. */
	struct cla *allow_flag, *block_flag, *disallow_flag, *end_flag;
	struct cla *release_flag, *reload_flag, *status_flag, *unblock_flag;
	struct cla *wait_flag;

	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	struct option long_options[]
		= { { "allow", no_argument, NULL, 'a' },
		    { "block", required_argument, NULL, 'b' },
		    { "disallow", no_argument, NULL, 'd' },
		    { "end-daemon", no_argument, NULL, 'e' },
		    { "help", no_argument, NULL, 'h' },
		    { "release", required_argument, NULL, 'r' },
		    { "reload-config", no_argument, NULL, 'c' },
		    { "status", no_argument, NULL, 's' },
		    { "unblock", required_argument, NULL, 'u' },
		    { "version", no_argument, NULL, 'V' },
		    { "wait", no_argument, NULL, 'w' },
		    { NULL, 0, NULL, 0 } };

	va_start(ap, argv);
	allow_flag = va_arg(ap, struct cla *);
	block_flag = va_arg(ap, struct cla *);
	disallow_flag = va_arg(ap, struct cla *);
	end_flag = va_arg(ap, struct cla *);
	release_flag = va_arg(ap, struct cla *);
	reload_flag = va_arg(ap, struct cla *);
	status_flag = va_arg(ap, struct cla *);
	unblock_flag = va_arg(ap, struct cla *);
	wait_flag = va_arg(ap, struct cla *);
	va_end(ap);

	while ((c = getopt_long(argc, argv, "ab:cdehr:su:Vw", long_options,
				&option_index))
	       != -1) {
		switch (c) {
		case 'a':
			allow_flag->is_set = 1;
			break;
		case 'b':
			block_flag->is_set = 1;
			if ((sws_err = cpyarg(block_flag->argument, optarg)))
				return sws_err;
			break;
		case 'c':
			reload_flag->is_set = 1;
			break;
		case 'd':
			disallow_flag->is_set = 1;
			break;
		case 'e':
			end_flag->is_set = 1;
			break;
		case 'h':
			printf("%s %s", argv[0], " - Help option.\n");
			printf("\tLong and short options can be mixed on the "
			       "command line but if an option\ntakes an "
			       "optional argument it is best to enter "
			       "-o\"argument\" or\n--option=argument.\n");
			printf("-a | --allow\t\tUnblock the server.\n");
			printf("-b | --block 'Client'\tBlock 'Client'.\n");
			printf("-c | --reload-config\tRequest the daemon to "
			       "reload it's config file.\n");
			printf("-d | --disallow\t\tBlock the server.\n");
			printf("-e | --end-daemon\tEnd the swocserver "
			       "daemon.\n");
			printf("-r | --release 'Client'\tRemove the lock for "
			       "'Client'.\n");
			printf("-s | --status\t\tShow the locking status.\n");
			printf("-u | --unblock 'Client'\tUnblock 'Client'.\n");
			printf("-V | --version\t\tDisplay version "
			       "information.\n");
			printf("-w | --wait\t\tWait for all locks to clear.\n");
			exit(0);
			break;
		case 'r':
			release_flag->is_set = 1;
			if ((sws_err = cpyarg(release_flag->argument, optarg)))
				return sws_err;
			break;
		case 's':
			status_flag->is_set = 1;
			break;
		case 'u':
			unblock_flag->is_set = 1;
			if ((sws_err = cpyarg(unblock_flag->argument, optarg)))
				return sws_err;
			break;
		case 'V':
			printf("%s %s %s %s", argv[0], "Source version -",
			       swocserver_get_src_version(), "\n");
			printf("%s %s %s %s", argv[0], "Package version -",
			       swocserver_get_pkg_version(), "\n");
			exit(0);
			break;
		case 'w':
			wait_flag->is_set = 1;
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

	/* Check mutually exclusive options. */
	sws_err = 0;
	if (allow_flag->is_set && block_flag->is_set) {
		fprintf(stderr, "Options a and b are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && reload_flag->is_set) {
		fprintf(stderr, "Options a and c are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && disallow_flag->is_set) {
		fprintf(stderr, "Options a and d are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && end_flag->is_set) {
		fprintf(stderr, "Options a and e are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && release_flag->is_set) {
		fprintf(stderr, "Options a and r are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options a and s are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options a and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (allow_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options a and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && reload_flag->is_set) {
		fprintf(stderr, "Options b and c are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && disallow_flag->is_set) {
		fprintf(stderr, "Options b and d are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && end_flag->is_set) {
		fprintf(stderr, "Options b and e are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && release_flag->is_set) {
		fprintf(stderr, "Options b and r are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options b and s are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options b and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (block_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options b and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (disallow_flag->is_set && end_flag->is_set) {
		fprintf(stderr, "Options d and e are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (disallow_flag->is_set && release_flag->is_set) {
		fprintf(stderr, "Options d and r are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (disallow_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options d and s are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (disallow_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options d and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (disallow_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options d and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (end_flag->is_set && release_flag->is_set) {
		fprintf(stderr, "Options e and r are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (end_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options e and s are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (end_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options e and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (end_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options e and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (release_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options r and s are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (release_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options r and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (release_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options r and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (status_flag->is_set && unblock_flag->is_set) {
		fprintf(stderr, "Options s and u are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (status_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options s and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	} else if (unblock_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options u and w are mutually exclusive.\n");
		sws_err = -MGE_PARAM;
	}
	/* Check for mandatory options */
	if (!(allow_flag->is_set || block_flag->is_set || reload_flag->is_set
	      || disallow_flag->is_set || end_flag->is_set
	      || release_flag->is_set || status_flag->is_set
	      || unblock_flag->is_set || wait_flag->is_set)) {
		fprintf(stderr, "Either a, b, c, d, e, r, s, u or w "
				"must be specified.\n");
		sws_err = -MGE_PARAM;
	}

	if (sws_err)
		mge_errno = MGE_PARAM;
	return sws_err;
}

/*
 * Function to copy optarg to the flag struct member argument.
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
