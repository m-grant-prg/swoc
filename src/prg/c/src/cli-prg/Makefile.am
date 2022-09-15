#########################################################################
#									#
# Script ID: src/prg/c/src/cli-prg/Makefile.am				#
# Author: Copyright (C) 2015-2022  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate SWOCClient-C programs from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 08/07/2015	MG	1.0.1	First release.				#
# 08/07/2015	MG	1.0.2	Extracted remsyslog.h to a separate	#
#				libremsyslog-dev package.		#
# 10/07/2015	MG	1.0.3	remsyslog library extracted, add 	#
#				pkg-config LDFLAGS for that library.	#
# 12/07/2015	MG	1.0.4	Change to use libmgesysutils.		#
# 24/10/2015	MG	1.0.5	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 09/05/2016	MG	1.0.6	Move header files to include directory.	#
# 11/06/2016	MG	1.0.7	Add flags for libswocclient following	#
#				it's introduction.			#
# 14/01/2017	MG	1.0.8	Set debug or normal compile flags here.	#
# 11/02/2017	MG	1.0.9	Add libmgec.				#
# 31/05/2017	MG	1.0.10	Add support for temporary libraries.	#
# 14/10/2017	MG	1.0.11	Add support for temporary header files.	#
# 16/10/2017	MG	1.0.12	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.13	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.14	Correct usage of config.h		#
# 16/01/2018	MG	1.0.15	Change to include headers from relevant	#
#				-dev projects.				#
# 26/03/2018	MG	1.0.16	Add support for sparse.			#
# 22/05/2018	MG	1.0.17	Simplify src directory structure and	#
#				header file location.			#
# 23/06/2018	MG	1.0.18	Disable sparse variable length array	#
#				warnings.				#
# 08/07/2018	MG	1.0.19	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 13/08/2018	MG	1.0.20	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
# 19/05/2019	MG	1.0.21	Merge sub-projects into one.		#
#				Remove shebang.				#
# 15/03/2020	MG	1.0.22	Add libswoccommon.la to LDADD for	#
#				testing and debugging with libtool	#
#				mode=execute.				#
# 27/03/2020	MG	1.0.23	Move into swocclient sub-directory as	#
#				the directory hierarchy needs to be the	#
#				same accross the source tree for	#
#				temporary libraries to work based on	#
#				the search in configure.ac.		#
# 09/10/2021	MG	1.0.24	Capitalise MG_CFLAGS variable.		#
#				Use CPPFLAGS prepared by configure.	#
#				Use LDFLAGS prepared by configure.	#
# 11/10/2021	MG	1.0.25	Move cmdlineargs.h to inc directory.	#
#				Move signalhandle.h to inc directorry.	#
# 15/10/2021	MG	1.0.26	Change debug optimisation to -O0	#
# 05/11/2021	MG	1.0.27	Use (non-)debug CPPFLAGS from configure	#
#				Use (non-)debug CFLAGS from configure	#
# 08/12/2021	MG	1.0.28	Tighten SPDX tag.			#
# 06/06/2022	MG	1.0.29	Add compiler analyzer option.		#
# 15/09/2022	MG	1.0.30	Rename tmplibraries.			#
#				Flatten directory structure.		#
#									#
#########################################################################


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../inc/api
AM_CPPFLAGS += -I$(srcdir)/../../inc/internal
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


if DEBUG
AM_CFLAGS = $(MG_DEBUG_CFLAGS)
else
AM_CFLAGS = $(MG_CFLAGS)
endif
if HEADERCHECK
AM_CFLAGS += -H
endif
if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif
if ANALYZER
AM_CFLAGS += $(MG_ANALYZER_CFLAGS)
endif
AM_CFLAGS += $(LIBSWOCCLIENT_CFLAGS) $(LIBMGEC_CFLAGS)


noinst_HEADERS = internal.h


sbin_PROGRAMS = swocclient
swocclient_SOURCES = main.c version.c cmdlineargs.c signalhandle.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += $(LIBMGEC_LIBS)


LDADD = $(tmplibraries_depth_2)
LDADD += ../com-lib/libswoccommon/libswoccommon.la
LDADD += ../cli-lib/libswocclient.la
