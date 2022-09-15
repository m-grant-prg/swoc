#########################################################################
#									#
# Script ID: src/prg/c/src/tests/cli-lib/Makefile.am			#
# Author: Copyright (C) 2016-2019, 2021, 2022  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate testoptionproc test program.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/06/2016	MG	1.0.1	First release.				#
# 14/01/2017	MG	1.0.2	Set debug or normal compile flags here.	#
# 14/10/2017	MG	1.0.3	Add support for temporary headers and	#
#				libraries.				#
# 18/10/2017	MG	1.0.4	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.5	Add SPDX license tags to source files.	#
# 13/01/2018	MG	1.0.6	Correct the use of config.h - add the	#
#				header directory to AM_CPPFLAGS.	#
# 16/01/2018	MG	1.0.7	Change to include headers from relevant	#
#				-dev projects.				#
# 07/07/2018	MG	1.0.8	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 19/05/2019	MG	1.0.9	Merge sub-projects into one.		#
#				Remove shebang.				#
# 08/11/2019	MG	1.0.10	Move under check target.		#
# 09/10/2021	MG	1.0.11	Use CFLAGS from configure.		#
#				Use CPPFLAGS prepared by configure.	#
#				Use LDFLAGS prepared by configure.	#
# 11/10/2021	MG	1.0.12	Use api and internal header directories.#
# 15/10/2021	MG	1.0.13	Change debug optimisation to -O0	#
# 05/11/2021	MG	1.0.14	Use (non-)debug CPPFLAGS from configure	#
#				Use (non-)debug CFLAGS from configure	#
# 08/12/2021	MG	1.0.15	Tighten SPDX tag.			#
# 15/09/2022	MG	1.0.16	Rename tmplibraries.			#
#				Move to tests sub directory.		#
#									#
#########################################################################


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../../inc/api
AM_CPPFLAGS += -I$(srcdir)/../../../inc/internal
AM_CPPFLAGS += -I../../../gen/inc -I../../../gen/lib


if DEBUG
AM_CFLAGS = $(MG_DEBUG_CFLAGS)
else
AM_CFLAGS = $(MG_CFLAGS)
endif


check_PROGRAMS = optionproctest
optionproctest_SOURCES = test-optionproc.c


AM_LDFLAGS = $(MG_LDFLAGS)


LDADD = ../../cli-lib/libswocclient.la
LDADD += $(tmplibraries_depth_3)

