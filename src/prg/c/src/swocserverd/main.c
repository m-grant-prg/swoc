/**
 * @file srv-prg/src/prg/c/src/swocserverd/main.c
 *
 * Server Wait On Clients server daemon.
 * Daemon to enable a server to manage client locks and wait on the removal of
 * those locks prior to further server processing.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.14 ==== 01/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.1	First release.				*
 * 17/12/2016	MG	1.0.2	Re-distribute some functions over	*
 *				libswocserver and new common library	*
 *				libswoccommon.				*
 * 08/01/2017	MG	1.0.3	Add creation / deletion of pid file.	*
 *				(Recommended for a systemd service of	*
 *				type forking.)				*
 * 02/02/2017	MG	1.0.4	Migrate to use of mge_errno in libmgec.	*
 * 13/02/2017	MG	1.0.5	Implement config file reload		*
 *				functionality.				*
 * 				Correct validate config file failure	*
 *				exiting with 0. swsd_err was not	*
 *				assigned an error value on failure.	*
 * 22/04/2017	MG	1.0.6	Change to use new bstree struct.	*
 * 27/05/2017	MG	1.0.7	Give daemon its own config file.	*
 * 				Use new local validateconfig.		*
 * 				Add support for temporary include	*
 *				directory.				*
 * 04/06/2017	MG	1.0.8	Split comms functions out of main into	*
 *				their own source file.			*
 *				Tidy up unnecessary include statements.	*
 * 				Use more meaningful name for client	*
 *				lock bstree.				*
 * 07/06/2017	MG	1.0.9	Implement epoll controlled use of	*
 *				multiple ports.				*
 * 16/09/2017	MG	1.0.10	Add full file path to error message if	*
 *				pid file creation fails.		*
 * 				Change close to re-direct for stdin,	*
 *				stdout & stderr during daemonisation as	*
 *				epoll_wait silently does not work as	*
 *				expected if the standard fd's are 	*
 *				re-used.				*
 * 18/11/2017	MG	1.0.11	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 19/11/2017	MG	1.0.12	Make program exit with EXIT_SUCCESS or	*
 *				EXIT_FAILURE only.			*
 * 22/03/2018	MG	1.0.13	Remove unnecessary libsoccommon.h	*
 * 01/05/2018	MG	1.0.14	Add support for blocked clients list.	*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>

#include <configmake.h>
#include <cmdlineargs.h>
#include <signalhandle.h>
#include <mge-errno.h>
#include <swocserverd.h>


int swsd_err;				/**< swoc daemon error number. */
char client[_POSIX_HOST_NAME_MAX];	/**< Client name. */
int debug;				/**< Debug - 0 false, 1 true. */
int end;				/**< End pending. */
int cursockfd;				/**< Socket file descriptor in use. */
struct comm_spec *port_spec;		/**< Port / socket config mappings. */
struct bstree *cli_locks;		/**< Clients and locks. */
struct bstree *cli_blocked;		/**< Blocked client list. */
struct bstree *port_sock;		/**< Port / socket actual mappings. */


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
	char *pidfile = RUNSTATEDIR"/swocserverd.pid";
	FILE *fp;

	swsd_err = 0;
	debug = 0;
	end = 0;

	/* Initialise signal handling. */
	init_sig_handle();

	/* Check not already running. */
	fp = fopen(pidfile, "r");
	if (fp != NULL) {
		syslog((int) (LOG_USER | LOG_NOTICE), "Daemon already "
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
		syslog((int) (LOG_USER | LOG_NOTICE), "ERROR allocating "
			"port_spec - %s", strerror(swsd_err));
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

	cli_locks = cre_bst(BST_NODES_DUPLICATES,
			(int (*)(const void *, const void *))strcmp);
	if (cli_locks == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "BST creation errored "
			"with %i.", mge_errno);
		goto b4_cli_locks;
	}

	cli_blocked = cre_bst(BST_NODES_UNIQUE,
			(int (*)(const void *, const void *))strcmp);
	if (cli_blocked == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "BST creation errored "
			"with %i.", mge_errno);
		goto b4_cli_blocked;
	}


	/* Prepare sockets. */
	port_sock = cre_bst(BST_NODES_UNIQUE,
			(int (*)(const void *, const void *))csscmp);
	if (port_sock == NULL) {
		if (debug)
			fprintf(stderr, "BST creation errored with %i.\n",
				mge_errno);
		syslog((int) (LOG_USER | LOG_NOTICE), "BST creation errored "
			"with %i.", mge_errno);
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
	pid_t pid, sid;
	char *pidfile = RUNSTATEDIR"/swocserverd.pid";
	char spid[256];
	FILE *fp;

	syslog((int) (LOG_USER | LOG_NOTICE), "Starting daemon.");

	/*
	 * Fork off the parent process.
	 * < 0 Error
	 * == 0 This is the child
	 * > 0 is pid of child, ie this is the parent.
	 */
	pid = fork();
	if (pid < 0) {
		syslog((int) (LOG_USER | LOG_NOTICE),"Error forking.");
		exit(EXIT_FAILURE);
	}
	/* This is the parent process, so exit after creating pid file. */
	if (pid > 0) {
		/* Create pid file. */
		if ((fp = fopen(pidfile, "w")) == NULL) {
			syslog((int) (LOG_USER | LOG_NOTICE), "Cannot create "
					"pid file - %s", pidfile);
			exit(EXIT_FAILURE);
		}
		sprintf(spid, "%i\n", (int) pid);
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
		syslog((int) (LOG_USER | LOG_NOTICE), "setsid error.");
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory. */
	if ((chdir("/")) < 0) {
		syslog((int) (LOG_USER | LOG_NOTICE), "Error on cd /.");
		exit(EXIT_FAILURE);
	}

	/*
	 * Rather than closing the standard file descriptors we shall
	 * re-direct them to /dev/null. This is because epoll_wait silently
	 * does not work as intended if these file descriptors are re-used.
	 */
	freopen ("/dev/null", "r", stdin);
	freopen ("/dev/null", "w", stdout);
	freopen ("/dev/null", "w", stderr);
	syslog((int) (LOG_USER | LOG_NOTICE), "Daemon started successfully.");
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
