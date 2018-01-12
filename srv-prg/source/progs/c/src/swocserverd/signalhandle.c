/**
 * @file srv-prg/source/progs/c/src/swocserverd/signalhandle.c
 *
 * Signal handling functions.
 *
 * > Likely terminate signals to handle.
 * >
 * > SIGTERM\n
 * > SIGINT	(Ctrl-c)\n
 * > SIGQUIT	(Ctrl-\)\n
 * > SIGHUP\n
 * >
 * > Might want to handle these non-terminate signals.
 * >
 * > SIGCONT just print program continued.\n
 * > SIGTSTP (Ctrl-z) just print stopped.
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.3 ==== 18/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 19/11/2016	MG	1.0.1	First release.				*
 * 17/02/2017	MG	1.0.2	Initialise signal mask before use.	*
 * 18/11/2017	MG	1.0.3	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <signalhandle.h>

/** Caught signal is already being processed flag. */
volatile sig_atomic_t processing_caught_signal = 0;

/**
 * Initialise signal handler.
 */
void init_sig_handle(void)
{
	sigset_t block_mask;
	struct sigaction new_action, old_action;
	memset(&new_action, 0, sizeof(new_action));
	new_action.sa_handler = termination_handler;

	/* Block other terminate signals whilst this is processed. */
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sigaddset(&block_mask, SIGTERM);
	sigaddset(&block_mask, SIGHUP);
	new_action.sa_mask = block_mask;
	new_action.sa_flags = SA_RESTART;

	/*
	 * If the action for these signals is set to ignore, then some parent
	 * process explicitly did this and we should not change it.
	 */
	sigaction(SIGINT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN)
		sigaction(SIGINT, &new_action, NULL);
	sigaction(SIGQUIT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN)
		sigaction(SIGQUIT, &new_action, NULL);
	sigaction(SIGTERM, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN)
		sigaction(SIGTERM, &new_action, NULL);
	sigaction(SIGHUP, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN)
		sigaction(SIGHUP, &new_action, NULL);
}

/**
 * Handler for caught terminating signals.
 * @param signum The signal number.
 */
void termination_handler(const int signum)
{
	/*
	 * In general it is possible that since this handler is established for
	 * more than one kind of signal, it might get invoked recursively by
	 * delivery of another kind of signal, (if that signal is not blocked
	 * above, or cannot be blocked). Use a static variable to keep track of
	 * that.
	 */
	if (processing_caught_signal)
		raise(signum);
	processing_caught_signal = 1;

	/*
	 * Now do the clean up actions:
	 * - reset terminal modes
	 * - kill child processes
	 * - remove lock files
	 */
	psignal(signum, "Received");

	/*
	 * Now reraise the signal using the signal’s default handling, which is
	 * to terminate the process.
	 * We could just call exit or abort, but reraising the signal sets the
	 * return status from the process correctly.
	 */
	signal(signum, SIG_DFL);
	raise(signum);
}
