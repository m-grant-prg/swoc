/**
 * @file src/prg/c/inc/srv-prg/signalhandle.h
 *
 * Header file for signal handling.
 *
 * @author Copyright (C) 2015-2017, 2019  Mark Grant
 *
 * Released under the GPLv3 only.\n
 * SPDX-License-Identifier: GPL-3.0
 *
 * @version _v1.0.6 ==== 18/05/2019_
 */

/* **********************************************************************
 *									*
 * Changelog								*
 *									*
 * Date		Author	Version	Description				*
 *									*
 * 22/06/2015	MG	1.0.1	First release.				*
 * 10/05/2016	MG	1.0.2	Move header files to include directory.	*
 * 17/07/2016	MG	1.0.3	Move towards kernel coding style.	*
 * 27/09/2016	MG	1.0.4	Further coding style changes.		*
 *				Improve in-source documentation.	*
 * 17/11/2017	MG	1.0.5	Add Doxygen comments.			*
 *				Add SPDX license tag.			*
 * 18/05/2019	MG	1.0.6	Merge sub-projects into one.		*
 *									*
 ************************************************************************
 */


#ifndef SIGNALHANDLE_H
#define SIGNALHANDLE_H


#include <portability.h>

BEGIN_C_DECLS

void init_sig_handle(void);

void termination_handler(const int signum);

END_C_DECLS

#endif /* ndef SIGNALHANDLE_H */

