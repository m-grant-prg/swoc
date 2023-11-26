/**
 * @file src/prg/c/src/srv-prg/swocserverd/signalhandle.c
 *
 * Signal handling functions.
 *
 * > Likely terminate signals to handle.
 * >
 * > SIGTERM\n
 * > SIGINT	(Ctrl-C)\n
 * > SIGQUIT	(Ctrl-\)\n
 * > SIGHUP\n
 * >
 * > Might want to handle these non-terminate signals.
 * >
 * > SIGCONT\n
 * > SIGTSTP (Ctrl-Z)
 *
 * @author Copyright (C) 2016-2023  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.1.0 ==== 26/11/2023_
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "internal.h"
#include <libmgec/mge-bstree.h>
#include <swoc/signalhandle.h>

/**
 * Initialise signal handler.
 * Block all other caught signals whilst the handler processes.
 */
void init_sig_handle(void)
{
	sigset_t block_mask;
	struct sigaction new_action, old_action;

	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_handler = termination_handler;

	/* Block other caught signals whilst one is processed. */
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sigaddset(&block_mask, SIGTERM);
	sigaddset(&block_mask, SIGHUP);
	sigaddset(&block_mask, SIGCONT);
	sigaddset(&block_mask, SIGTSTP);
	new_action.sa_mask = block_mask;
	new_action.sa_flags = SA_RESTART;

	/*
	 * If the action for these signals is set to ignore, then some parent
	 * process explicitly did this and we should not change it.
	 */
	if (sigaction(SIGINT, NULL, &old_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot retrieve old "
						     "SIGINT action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGINT, &new_action, NULL)) {
			syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new "
							     "SIGINT action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGQUIT, NULL, &old_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot retrieve old "
						     "SIGQUIT action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGQUIT, &new_action, NULL)) {
			syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new "
							     "SIGQUIT action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGTERM, NULL, &old_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot retrieve old "
						     "SIGTERM action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGTERM, &new_action, NULL)) {
			syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new "
							     "SIGTERM action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGHUP, NULL, &old_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot retrieve old "
						     "SIGHUP action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGHUP, &new_action, NULL)) {
			syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new "
							     "SIGHUP action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGCONT, &new_action, NULL)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new SIGCONT "
						     "action");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTSTP, &new_action, NULL)) {
		syslog((int)(LOG_USER | LOG_NOTICE), "Cannot set new SIGTSTP "
						     "action");
		exit(EXIT_FAILURE);
	}
}

/**
 * Handler for caught signals.
 * > There are 2 possibilities here:-\n
 * > Catch, process and terminate\n
 * > or\n
 * > Catch, process and continue.\n
 *
 * Catch, process and terminate is straight forward as the easiest way to
 * terminate is to re-raise the signal using the default hander.
 *
 * > Catch, process and continue has 2 forms:-\n
 * > Continue via simple return\n
 * > or\n
 * > Continue via re-raise the signal using the default handler.\n
 *
 * The simple return is just that.\n
 * \n
 * Re-raise the signal using the default handler is required for instance with
 * SIGTSTP (Ctrl-Z). This is because in a terminal the default handler ensures
 * the stopped process is in the background and control is passed back to the
 * shell, (fg can then be used to resume the process). This means that SIGTSTP
 * is now set to use the default handler, hence this relies on SIGCONT to
 * explicitly reset the SIGTSTP handler.
 * @param signum The signal number.
 */
void termination_handler(int signum)
{
	int err;
	struct sigaction cur_action;

	/*
	 * Reset signal handlers for any non-terminating but default re-raising
	 * signals which may have fired.
	 */
	if (sigaction(SIGTSTP, NULL, &cur_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Cannot retrieve old "
		       "action : %s",
		       strsignal(SIGTSTP));
		raise(SIGKILL);
	}
	if (cur_action.sa_handler != termination_handler) {
		cur_action.sa_handler = termination_handler;
		if (sigaction(SIGTSTP, &cur_action, NULL)) {
			syslog((int)(LOG_USER | LOG_NOTICE),
			       "Cannot set new "
			       "action : %s",
			       strsignal(SIGTSTP));
			raise(SIGKILL);
		}
	}

	if (sigaction(signum, NULL, &cur_action)) {
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Cannot retrieve old "
		       "action : %s",
		       strsignal(signum));
		raise(SIGKILL);
	}

	/* Log the signal and the number of locks being held. */
	syslog((int)(LOG_USER | LOG_NOTICE), "Signal Received : %s",
	       strsignal(signum));
	syslog((int)(LOG_USER | LOG_NOTICE),
	       "%i clients have %i locks on "
	       "this server.",
	       cli_locks->node_total, cli_locks->count_total);

	/*
	 * If the default signal should not be raised as the handler exits then
	 * return.
	 */
	if (signum == SIGCONT)
		return;

	/*
	 * As a daemon, SIGHUP means reload the configuration file which, if
	 * successful just requires a return but on failure exit.
	 */
	if (signum == SIGHUP) {
		err = swsd_reload_config();
		if (err)
			exit(err);
		return;
	}

	/*
	 * Now re-raise the signal using the signal’s default handling. If the
	 * default handling is to terminate we could just call exit or abort,
	 * but re-raising the signal sets the return status from the process
	 * correctly.
	 */
	cur_action.sa_handler = SIG_DFL;
	if (sigaction(signum, &cur_action, NULL)) {
		syslog((int)(LOG_USER | LOG_NOTICE),
		       "Cannot run default "
		       "signal handler : %s",
		       strsignal(signum));
		raise(SIGKILL);
	}
	raise(signum);
}
