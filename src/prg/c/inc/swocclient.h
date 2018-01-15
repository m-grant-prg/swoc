/**
 * @file cli-prg/src/prg/c/inc/swocclient.h
 *
 * Header file for Server Wait on Clients client-side program.
 *
 * @author Copyright (C) 2015-2017  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 12/11/2017_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 09/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 11/06/2016	MG	1.0.3	Modified following introduction of	*
 *				libswocclient library.			*
 * 13/06/2016	MG	1.0.4	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.5	Move towards kernel coding style.	*
 * 12/11/2017	MG	1.0.6	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 *									*
 ************************************************************************
 */


#ifndef SWOCCLIENT_H
#define SWOCCLIENT_H


#include <portability.h>

BEGIN_C_DECLS

char *swocclient_get_pkg_version(void);

char *swocclient_get_src_version(void);

void swocclient_print_pkg_version(void);

void swocclient_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCCLIENT_H */
