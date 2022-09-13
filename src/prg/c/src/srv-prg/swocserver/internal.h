/**
 * @file src/prg/c/src/srv-prg/swocserver/internal.h
 *
 * Header file for Server Wait on Clients server-side program.
 *
 * @author Copyright (C) 2015-2019, 2021, 2022  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * @version _v1.0.10 ==== 12/09/2022_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 10/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 13/06/2016	MG	1.0.3	Use new more unique library function	*
 *				names.					*
 * 17/07/2016	MG	1.0.4	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.5	Further coding style changes.		*
 *				Improve in-source documentation.	*
 *				Use more informative name - sws_err.	*
 * 12/11/2017	MG	1.0.6	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 22/05/2018	MG	1.0.7	Change from swocserver.h to internal.h	*
 * 18/05/2019	MG	1.0.8	Merge sub-projects into one.		*
 * 08/12/2021	MG	1.0.9	Tighten SPDX tag.			*
 * 12/09/2022	MG	1.0.10	Rename portability.h			*
 *				Use pkginclude location.		*
 *									*
 ************************************************************************
 */

#ifndef SWOCSERVER_INTERNAL_H
#define SWOCSERVER_INTERNAL_H

#include <libmgec/mge-portability.h>
#include <libswocserver.h>

BEGIN_C_DECLS

extern int sws_err;

char *swocserver_get_pkg_version(void);

char *swocserver_get_src_version(void);

void swocserver_print_pkg_version(void);

void swocserver_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCSERVER_INTERNAL_H */
