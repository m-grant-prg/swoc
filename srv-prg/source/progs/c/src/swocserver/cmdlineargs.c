/**
 * @file srv-prg/source/progs/c/src/swocserver/cmdlineargs.c
 *
 * Command line argument processing for swocserver using getopt_long.
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
 * 16/05/2016	MG	1.0.2	Add --force-unlock option.		*
 *				Make --status and --wait ignore		*
 *				directories.				*
 * 28/05/2016	MG	1.0.3	Modified following introduction of	*
 *				libswocserver library.			*
 * 13/06/2016	MG	1.0.4	Use new more unique library function	*
 *				names.					*
 * 17/07/2016	MG	1.0.5	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.6	Further coding style changes.		*
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
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <getopt.h>

#include <swocserver.h>
#include <cmdlineargs.h>

static int cpyarg(char *flagarg, char *srcarg);

/**
 * Process command line arguments using getopt_long.
 * Variable length argument list comprising of the flag structs. In this program
 * they are:-
 * @param argc The standard CLA argc.
 * @param argv The standard CLA argv.
 * @param end_flag Struct for the end daemon flag.
 * @param reload_flag Struct for the reload config flag.
 * @param status_flag Struct for the status flag.
 * @param unlock_flag Struct for the unlock flag.
 * @param wait_flag Struct for the wait flag.
 * @return 0 on success, on failure standard EX_USAGE (64) command line  usage
 * error.
 */
int process_cla(int argc, char **argv, ...)
{
	va_list ap;

	/* Command line argument flags. */
	struct cla *end_flag, *reload_flag, *status_flag, *unlock_flag;
	struct cla *wait_flag;

	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	va_start(ap, argv);
	end_flag = va_arg(ap, struct cla *);
	reload_flag = va_arg(ap, struct cla *);
	status_flag = va_arg(ap, struct cla *);
	unlock_flag = va_arg(ap, struct cla *);
	wait_flag = va_arg(ap, struct cla *);
	va_end(ap);

	struct option long_options[] = {
		{"end-daemon",		no_argument,		0,	'e'},
		{"help",		no_argument,		0,	'h'},
		{"reload-config",	no_argument,		0,	'r'},
		{"status",		no_argument,		0,	's'},
		{"unlock",		required_argument,	0,	'u'},
		{"version",		no_argument,		0,	'V'},
		{"wait",		no_argument,		0,	'w'},
		{0,			0,			0,	0}
	};

	while ((c = getopt_long(argc, argv, "ehrsu:Vw",
		long_options, &option_index)) != -1) {

		switch (c) {
		case 'e':
			end_flag->is_set = 1;
			break;

		case 'h':
			printf("%s %s", argv[0], " - Help option.\n");
			printf("\tLong and short options can be mixed on the "
				"command line but if an option\ntakes an "
				"optional argument it is best to enter "
				"-o\"argument\" or\n--option=argument.\n");
			printf("-e | --end-daemon\tEnd the swocserver "
				"daemon.\n");
			printf("-r | --reload-config\tRequest the daemon to "
				"reload it's config file.\n");
			printf("-s | --status\tShow the locking status.\n");
			printf("-u | --unlock 'Lock Name'\tRemove the "
				"lock 'Lock Name'.\n");
			printf("-V | --version\tDisplay version "
				"information.\n");
			printf("-w | --wait\tWait for all locks to clear.\n");
			if (!sws_err)
				sws_err = -1;
			break;

		case 'r':
			reload_flag->is_set = 1;
			break;

		case 's':
			status_flag->is_set = 1;
			break;

		case 'u':
			unlock_flag->is_set = 1;
			if ((sws_err = cpyarg(unlock_flag->argument, optarg)))
				return sws_err;
			break;

		case 'V':
			printf("%s %s %s %s", argv[0], "Source version -",
				swocserver_get_src_version(), "\n");
			printf("%s %s %s %s", argv[0], "Package version -",
				swocserver_get_pkg_version(), "\n");
			if (!sws_err)
				sws_err = -1;
			break;

		case 'w':
			wait_flag->is_set = 1;
			break;

		case '?':
			/* getopt_long already printed a message. */
			sws_err = EX_USAGE;
			return 1;
			break;

		default:
			abort();
		}
	}
	if (sws_err) {
		sws_err = 0;
		return 1;
	}

	/* Non-option arguments are not accepted. */
	if (optind < argc) {
		fprintf(stderr, "Program does not accept other arguments.\n");
		sws_err = EX_USAGE;
		return 1;
	}

	/* Check mutually exclusive options. */
	if (end_flag->is_set && unlock_flag->is_set) {
		fprintf(stderr, "Options e and f are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (end_flag->is_set && reload_flag->is_set) {
		fprintf(stderr, "Options e and r are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (end_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options e and s are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (end_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options e and w are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (unlock_flag->is_set && reload_flag->is_set) {
		fprintf(stderr, "Options f and r are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (unlock_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options f and s are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (unlock_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options f and w are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (reload_flag->is_set && status_flag->is_set) {
		fprintf(stderr, "Options r and s are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (reload_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options r and w are mutually exclusive.\n");
		sws_err = EX_USAGE;
	} else if (status_flag->is_set && wait_flag->is_set) {
		fprintf(stderr, "Options s and w are mutually exclusive.\n");
		sws_err = EX_USAGE;
	}
	/* Check for mandatory options */
	if (!(end_flag->is_set || unlock_flag->is_set || reload_flag->is_set
		|| status_flag->is_set || wait_flag->is_set)) {
		fprintf(stderr, "Either e, f, r, s or w must be specified.\n");
		sws_err = EX_USAGE;
	}
	return sws_err;
}

/*
 * Function to copy optarg to the flag struct member argument.
 */
static int cpyarg(char *flagarg, char *srcarg)
{
	if (ARG_BUF > strlen(srcarg)) {
		strcpy(flagarg, srcarg);
		return 0;
	}
	else
	{
		fprintf(stderr, "Option argument '%s' too long.\n", srcarg);
		return EX_USAGE;
	}
}
