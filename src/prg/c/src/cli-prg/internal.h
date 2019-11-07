/**
 * @file src/prg/c/src/cli-prg/internal.h
 *
 * Header file for Server Wait on Clients client-side program.
 *
 * @author Copyright (C) 2015-2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.8 ==== 18/05/2019_
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
 * 22/05/2018	MG	1.0.7	Change name to internal.h and place in	*
 *				src directory.				*
 * 18/05/2019	MG	1.0.8	Merge sub-projects into one.		*
 *									*
 ************************************************************************
 */

#ifndef SWOCCLIENT_INTERNAL_H
#define SWOCCLIENT_INTERNAL_H

#include <portability.h>

BEGIN_C_DECLS

char *swocclient_get_pkg_version(void);

char *swocclient_get_src_version(void);

void swocclient_print_pkg_version(void);

void swocclient_print_src_version(void);

END_C_DECLS

#endif /* ndef SWOCCLIENT_INTERNAL_H */

