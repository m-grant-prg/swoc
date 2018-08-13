#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/src/prg/c/src/swocserverd/Makefile.am		#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserverd daemon from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 13/01/2017	MG	1.0.1	First release.				#
# 19/05/2017	MG	1.0.2	Change for new directory structure.	#
#				Add validateconfig.c now swocserverd	#
#				has it's own config file.		#
# 29/05/2017	MG	1.0.3	Add support for temporary header files.	#
#				Add support for temporary libraries.	#
# 04/06/2017	MG	1.0.4	Add comms.c after source re-structure.	#
# 20/10/2017	MG	1.0.5	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.6	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.0.7	Correct usage of config.h		#
#				Change to include headers from relevant	#
#				-dev projects.				#
# 24/03/2018	MG	1.0.8	Add support for sparse.			#
# 22/05/2018	MG	1.0.9	Change from swocserverd.h to internal.h	#
# 23/06/2018	MG	1.0.10	Disable sparse variable length array	#
#				warnings.				#
# 08/07/2018	MG	1.0.11	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 13/08/2018	MG	1.0.12	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I$(top_srcdir)/../com-dev/src/prg/c/inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = @mg_cflags@

AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSWOCCOMMON_CFLAGS)

if DEBUG
AM_CFLAGS += -ggdb3 -Og
else
AM_CFLAGS += -O2
endif

if HEADERCHECK
AM_CFLAGS += -H
endif

if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif


noinst_HEADERS = internal.h


sbin_PROGRAMS = swocserverd
swocserverd_SOURCES = cmdlineargs.c signalhandle.c validateconfig.c version.c
swocserverd_SOURCES += main.c comms.c request.c


AM_LDFLAGS = $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)
AM_LDFLAGS += $(LIBSWOCCOMMON_LIBS)


LDADD = $(tmplibraries)
