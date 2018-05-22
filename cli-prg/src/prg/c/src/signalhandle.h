/**
 * @file cli-prg/src/prg/c/src/signalhandle.h
 *
 * Header file for signal handling.
 *
 * @author Copyright (C) 2015-2018  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 22/05/2018_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 08/07/2015	MG	1.0.1	First release.				*
 * 09/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 13/06/2016	MG	1.0.3	Adopt convention of using void in empty	*
 *				function parameter lists.		*
 * 17/07/2016	MG	1.0.4	Move towards kernel coding style.	*
 * 12/11/2017	MG	1.0.5	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 22/05/2018	MG	1.0.6	Simplify src directory structure and	*
 *				header file location.			*
 *									*
 ************************************************************************
 */


#ifndef SIGNALHANDLE_H
#define SIGNALHANDLE_H


#include <portability.h>

BEGIN_C_DECLS

void init_sig_handle(void);

void termination_handler(int signum);

END_C_DECLS

#endif /* ndef SIGNALHANDLE_H */
