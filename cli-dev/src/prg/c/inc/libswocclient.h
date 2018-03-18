/**
 * @file cli-dev/src/prg/c/inc/libswocclient.h
 *
 * Header file for Server Wait on Clients client-side library.
 *
 * @author Copyright (C) 2016-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.8 ==== 18/03/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 16/07/2016	MG	1.0.1	First version release.			*
 * 17/07/2016	MG	1.0.2	Move towards kernel coding style.	*
 * 06/01/2017	MG	1.0.3	Modify to use swocserverd daemon and	*
 *				TCP stream.				*
 * 06/02/2017	MG	1.0.4	Add pollint and wait function.		*
 * 02/10/2017	MG	1.0.5	Move validate config to common.		*
 * 11/11/2017	MG	1.0.6	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 02/02/2018	MG	1.0.7	Add number of locks parameter to wait.	*
 * 18/03/2018	MG	1.0.8	Add locks_held for use during signal	*
 *				handling, primarily in swocclient -w.	*
 *									*
 ************************************************************************
 */


#ifndef LIBSWOCCLIENT_H
#define LIBSWOCCLIENT_H


#include <portability.h>


BEGIN_C_DECLS


extern char locks_held[];


int swc_show_status(void);

int swc_set_lock(void);

int swc_rel_lock(void);

int swc_client_wait(char *cnumlocks);

char *libswocclient_get_pkg_version(void);

char *libswocclient_get_src_version(void);

void libswocclient_print_pkg_version(void);

void libswocclient_print_src_version(void);

END_C_DECLS

#endif /* ndef LIBSWOCCLIENT_H */
