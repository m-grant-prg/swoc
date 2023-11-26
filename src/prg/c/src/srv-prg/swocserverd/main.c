/**
 * @file src/prg/c/src/srv-prg/swocserverd/main.c
 *
 * Server Wait On Clients server daemon.
 * Daemon to enable a server to manage client locks and wait on the removal of
 * those locks prior to further server processing.
 *
 * @author Copyright (C) 2016-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

/* Standard GNU AC_HEADER_STDBOOL ifdeffery. */
#ifdef HAVE_STDBOOL_H
	#include <stdbool.h>
#else
	#ifndef HAVE__BOOL
		#ifdef __cplusplus
typedef bool _Bool;
		#else
			#define _Bool signed char
		#endif
	#endif
	#define bool _Bool
	#define false 0
	#define true 1
	#define __bool_true_false_are_defined 1
#endif

#include <configmake.h>

#include "internal.h"
#include <libmgec/libmgec.h>
#include <libmgec/mge-bstree.h>
#include <libmgec/mge-errno.h>
#include <swoc/cmdlineargs.h>
#include <swoc/signalhandle.h>

int swsd_err;			   /**< swoc daemon error number. */
char client[_POSIX_HOST_NAME_MAX]; /**< Client name. */
int debug;			   /**< Debug - 0 false, 1 true. */
int end;			   /**< End pending. */
int cursockfd;			   /**< Socket file descriptor in use. */
struct comm_spec *port_spec;	   /**< Port / socket config mappings. */
bool srv_blocked;		   /**< Server is blocked? */
struct bstree *cli_locks;	   /**< Clients and locks. */
struct bstree *cli_blocked;	   /**< Blocked client list. */
struct bstree *port_sock;	   /**< Port / socket actual mappings. */

static void daemonise(void);
static int csscmp(const struct comm_spec *first, const struct comm_spec *last);

/**
 * Program entry point.
 * @param argc Standard CLA argc.
 * @param argv Standard CLA argv.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv)
{
	char *pidfile = RUNSTATEDIR "/swocserverd.pid";
	FILE *fp;

	client[0] = '\0';
	swsd_err = 0;
	debug = 0;
	end = 0;

	/* Initialise signal handling. */
	init_sig_handle();

	/* Check not already running. */
	fp = fopen(pidfile, "r");
	if (fp != NULL) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Daemon already "
						     "running.");
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	/* Validate the config file. */
	port_spec = calloc(MAX_LISTEN_PORTS, sizeof(*port_spec));
	if (port_spec == NULL) {
		swsd_err = errno;
		if (debug)
			perror("ERROR allocating port_spec");
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "ERROR allocating "
		       "port_spec - %s",
		       strerror(swsd_err));
		exit(EXIT_FAILURE);
	}
	swsd_err = swsd_validate_config();
	if (swsd_err)
		goto b4_cli_locks;

	/* Process command line. */
	swsd_err = process_cla(argc, argv);
	if (swsd_err)
		goto b4_cli_locks;

	/* Daemonise if not in debug mode. */
	if (!debug)
		daemonise();

	srv_blocked = false;

	cli_locks = cre_bst(BST_NODES_DUPLICATES,
			    (int (*)(const void *, const void *))strcmp);
	if (cli_locks == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "BST creation errored "
		       "with %i.",
		       mge_errno);
		goto b4_cli_locks;
	}

	cli_blocked = cre_bst(BST_NODES_UNIQUE,
			      (int (*)(const void *, const void *))strcmp);
	if (cli_blocked == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "BST creation errored "
		       "with %i.",
		       mge_errno);
		goto b4_cli_blocked;
	}

	/* Prepare sockets. */
	port_sock = cre_bst(BST_NODES_UNIQUE,
			    (int (*)(const void *, const void *))csscmp);
	if (port_sock == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "BST creation errored "
		       "with %i.",
		       mge_errno);
		goto b4_port_sock;
	}
	swsd_err = prepare_sockets();
	if (swsd_err)
		goto comms_fail;

	/* Wait and perform comms. */
	swsd_err = process_comms();

comms_fail:
	port_sock = del_bst(port_sock);

b4_port_sock:
	cli_blocked = del_bst(cli_blocked);

b4_cli_blocked:
	cli_locks = del_bst(cli_locks);

b4_cli_locks:
	free(port_spec);
	if (!debug)
		remove(pidfile);
	if (swsd_err)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

/*
 * Daemonise the process.
 */
static void daemonise(void)
{
	char *pidfile = RUNSTATEDIR "/swocserverd.pid";
	pid_t pid, sid;
	char spid[256];
	FILE *fp;

	syslog((int)(LOG_USER | LOG_NOTICE), "Starting daemon.");

	/*
	 * Fork off the parent process.
	 * < 0 Error
	 * == 0 This is the child
	 * > 0 is pid of child, ie this is the parent.
	 */
	pid = fork();
	if (pid < 0) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Error forking.");
		exit(EXIT_FAILURE);
	}
	/* This is the parent process, so exit after creating pid file. */
	if (pid > 0) {
		/* Create pid file. */
		if ((fp = fopen(pidfile, "w")) == NULL) {
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Cannot create "
			       "pid file - %s",
			       pidfile);
			exit(EXIT_FAILURE);
		}
		snprintf(spid, ARRAY_SIZE(spid), "%i\n", (int)pid);
		fputs(spid, fp);
		fclose(fp);
		exit(EXIT_SUCCESS);
	}

	/* Set the file mode creation mask for this new process. */
	umask(0);

	/*
	 * Create a new session and sets this process as process group leader
	 * in new process group.
	 */
	if ((sid = setsid()) < 0) {
		syslog((int)(LOG_USER | LOG_NOTICE), "setsid error.");
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory. */
	if ((chdir("/")) < 0) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Error on cd /.");
		exit(EXIT_FAILURE);
	}

	/*
	 * Rather than closing the standard file descriptors we shall
	 * re-direct them to /dev/null. This is because epoll_wait silently
	 * does not work as intended if these file descriptors are re-used.
	 */
	if (freopen("/dev/null", "r", stdin) == NULL)
		syslog((int)(LOG_USER | LOG_NOTICE), "stdin freopen error.");
	if (freopen("/dev/null", "w", stdout) == NULL)
		syslog((int)(LOG_USER | LOG_NOTICE), "stdout freopen error.");
	if (freopen("/dev/null", "w", stderr) == NULL)
		syslog((int)(LOG_USER | LOG_NOTICE), "stderr freopen error.");
	syslog((int)(LOG_USER | LOG_NOTICE), "Daemon started successfully.");
}

/*
 * comm_spec struct (css) comparison function.
 * Tree should be indexed by socket file descriptor so compare the socket value.
 */
static int csscmp(const struct comm_spec *first, const struct comm_spec *last)
{
	if (first->socketfd == last->socketfd)
		return 0;
	return first->socketfd < last->socketfd ? -1 : 1;
}
