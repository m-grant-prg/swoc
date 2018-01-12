/**
 * @file srv-dev/source/progs/c/inc/libswocserver.h
 *
 * Header file for Server Wait on Clients server-side library.
 *
 * @author Copyright (C) 2016-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.9 ==== 12/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 16/07/2016	MG	1.0.1	First versioned release.		*
 * 17/07/2016	MG	1.0.2	Further coding style changes.		*
 * 14/12/2016	MG	1.0.3	Changes for move from NFS share 'file	*
 *				as flag' to TCP socket implementation.	*
 *				Add server extern.			*
 *				Add common server-side validate		*
 *				prototype.				*
 *				Add new end daemon function prototype.	*
 * 13/02/2017	MG	1.0.4	Add new reload config function.		*
 * 15/05/2017	MG	1.0.5	Change validate config to swocserver-	*
 *				only, aot common with swocserverd.	*
 * 12/09/2017	MG	1.0.6	Change sws_force_unlock() to		*
 *				sws_unlock().				*
 * 15/09/2017	MG	1.0.7	Change references to ssl to tls.	*
 * 02/10/2017	MG	1.0.8	Move validate config to common.		*
 * 12/11/2017	MG	1.0.9	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#ifndef LIBSWOCSERVER_H
#define LIBSWOCSERVER_H


#include <portability.h>


BEGIN_C_DECLS

int sws_show_status(void);

int sws_server_wait(void);

int sws_unlock(char *lockname);

int sws_end_daemon(void);

int sws_reload_config(void);

char *libswocserver_get_pkg_version(void);

char *libswocserver_get_src_version(void);

void libswocserver_print_pkg_version(void);

void libswocserver_print_src_version(void);

END_C_DECLS

#endif /* ndef LIBSWOCSERVER_H */
