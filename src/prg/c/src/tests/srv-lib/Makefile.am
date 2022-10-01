#########################################################################
#									#
# Script ID: src/prg/c/src/tests/srv-lib/Makefile.am			#
# Author: Copyright (C) 2016-2019, 2021, 2022 Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate optionproctest test program.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 25/05/2016	MG	1.0.1	First release.				#
# 14/01/2017	MG	1.0.2	Set debug or normal compile flags here.	#
# 20/10/2017	MG	1.0.3	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.4	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.5	Correct consumption of config.h		#
# 16/01/2018	MG	1.0.6	Change to include headers from relevant	#
#				-dev projects.				#
# 08/07/2018	MG	1.0.7	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 19/05/2019	MG	1.0.8	Merge sub-projects into one.		#
#				Remove shebang.				#
# 08/11/2019	MG	1.0.9	Move under check target.		#
# 09/10/2021	MG	1.0.10	Use CFLAGS from configure.		#
#				Use CPPFLAGS prepared by configure.	#
#				Use LDFLAGS prepared by configure.	#
# 11/10/2021	MG	1.0.11	Use api and internal header directories.#
# 15/10/2021	MG	1.0.12	Change debug optimisation to -O0	#
# 05/11/2021	MG	1.0.13	Use (non-)debug CPPFLAGS from configure	#
#				Use (non-)debug CFLAGS from configure	#
# 08/12/2021	MG	1.0.14	Tighten SPDX tag.			#
# 29/09/2022	MG	1.0.15	Move tests under tests.			#
#				Flatten inc directory to pkgincludedir.	#
#									#
#########################################################################


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../../inc
AM_CPPFLAGS += -I../../../gen/inc -I../../../gen/lib


if DEBUG
AM_CFLAGS = $(MG_DEBUG_CFLAGS)
else
AM_CFLAGS = $(MG_CFLAGS)
endif


check_PROGRAMS = optionproctest
optionproctest_SOURCES = test-optionproc.c


AM_LDFLAGS = $(MG_LDFLAGS)


LDADD = $(tmplibraries)
LDADD += ../../srv-lib/libswocserver.la

