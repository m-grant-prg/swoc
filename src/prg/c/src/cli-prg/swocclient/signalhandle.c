/**
 * @file src/prg/c/src/cli-prg/swocclient/signalhandle.c
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
 * @author Copyright (C) 2015-2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.12 ==== 14/09/2022_
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
 * 12/11/2017	MG	1.0.4	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 19/03/2018	MG	1.0.5	Print locks held warning.		*
 *				Code for re-raise and return exit paths.*
 *				Improve error handling.			*
 *				Improve comments.			*
 * 22/05/2018	MG	1.0.6	Simplify src directory structure and	*
 *				header file location.			*
 * 18/05/2019	MG	1.0.7	Merge sub-projects into one.		*
 * 27/03/2020	MG	1.0.8	Move into swocclient sub-directory as	*
 *				the directory hierarchy needs to be the	*
 *				same accross the source tree for	*
 *				temporary libraries to work based on	*
 *				the search in configure.ac.		*
 * 11/10/2021	MG	1.0.9	Move signalhandle.h to internal inc.	*
 * 14/10/2021	MG	1.0.10	Eliminate -Wunused-result warnings.	*
 * 08/12/2021	MG	1.0.11	Tighten SPDX tag.			*
 * 14/09/2022	MG	1.0.12	Correct included headers.		*
 *									*
 ************************************************************************
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libswocclient.h>
#include <signalhandle.h>

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
		perror("Cannot retrieve old SIGINT action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGINT, &new_action, NULL)) {
			perror("Cannot set new SIGINT action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGQUIT, NULL, &old_action)) {
		perror("Cannot retrieve old SIGQUIT action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGQUIT, &new_action, NULL)) {
			perror("Cannot set new SIGQUIT action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGTERM, NULL, &old_action)) {
		perror("Cannot retrieve old SIGTERM action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGTERM, &new_action, NULL)) {
			perror("Cannot set new SIGTERM action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGHUP, NULL, &old_action)) {
		perror("Cannot retrieve old SIGHUP action");
		exit(EXIT_FAILURE);
	}
	if (old_action.sa_handler != SIG_IGN) {
		if (sigaction(SIGHUP, &new_action, NULL)) {
			perror("Cannot set new SIGHUP action");
			exit(EXIT_FAILURE);
		}
	}

	if (sigaction(SIGCONT, &new_action, NULL)) {
		perror("Cannot set new SIGCONT action");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTSTP, &new_action, NULL)) {
		perror("Cannot set new SIGTSTP action");
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
	struct sigaction cur_action;
	char msg[40] = "Currently ";

	/*
	 * Reset signal handlers for any non-terminating but default re-raising
	 * signals which may have fired.
	 */
	if (sigaction(SIGTSTP, NULL, &cur_action)) {
		psignal(SIGTSTP, "Cannot retrieve old action");
		raise(SIGKILL);
	}
	if (cur_action.sa_handler != termination_handler) {
		cur_action.sa_handler = termination_handler;
		if (sigaction(SIGTSTP, &cur_action, NULL)) {
			psignal(SIGTSTP, "Cannot set new action");
			raise(SIGKILL);
		}
	}

	if (sigaction(signum, NULL, &cur_action)) {
		psignal(signum, "Cannot retrieve old action");
		raise(SIGKILL);
	}

	/*
	 * Print number of locks held warning.
	 * The value could be out of date by as much as the polling interval.
	 */
	psignal(signum, "Signal Received");

	strcat(msg, locks_held);
	strcat(msg, " locks held.\n");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
	write(STDERR_FILENO, msg, strlen(msg));
#pragma GCC diagnostic pop

	/*
	 * If the default signal should not be raised as the handler exits then
	 * return.
	 */
	if (signum == SIGCONT)
		return;

	/*
	 * Now re-raise the signal using the signal’s default handling. If the
	 * default handling is to terminate we could just call exit or abort,
	 * but re-raising the signal sets the return status from the process
	 * correctly.
	 */
	cur_action.sa_handler = SIG_DFL;
	if (sigaction(signum, &cur_action, NULL)) {
		psignal(signum, "Cannot run default signal handler");
		raise(SIGKILL);
	}
	raise(signum);
}
