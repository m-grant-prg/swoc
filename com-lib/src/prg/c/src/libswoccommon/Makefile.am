#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/src/prg/c/src/libswoccommon/Makefile.am		#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon library from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 11/12/2016	MG	1.0.1	First release.				#
# 13/01/2017	MG	1.0.2	Set debug or normal compile flags here.	#
# 09/06/2017	MG	1.0.3	Add support for temporary header files.	#
#				Add support for temporary libraries.	#
# 03/10/2017	MG	1.0.4	Add ssh support.			#
# 19/10/2017	MG	1.0.5	Change to use _LIBADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
#				Re-order into logical steps.		#
# 12/01/2018	MG	1.0.6	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.7	Correct usage of config.h		#
# 16/01/2018	MG	1.0.8	Change to include headers from relevant	#
#				-dev projects.				#
# 26/01/2018	MG	1.0.9	Add correct pthread handling.		#
# 26/03/2018	MG	1.0.10	Add support for sparse.			#
# 23/06/2018	MG	1.0.11	Disable sparse variable length array	#
#				warnings.				#
#				Use lib_version_info AC_SUBST variable	#
#				passed via AC_CONFIG_FILES to enable	#
#				all top level versioning to be done in	#
#				configure.ac				#
# 07/07/2018	MG	1.0.12	Improve debug flags used. Use gdb	#
#				orientated level 3 (includes		#
#				pre-processor macros).			#
# 12/08/2018	MG	1.0.13	Support configure time otion to add -H	#
#				compiler flag to show included files	#
#				and their depth in the include stack.	#
#				Use compiler version dependent CFLAGS	#
#				assembled in configure.			#
#									#
#########################################################################


CC = $(PTHREAD_CC)


AM_CPPFLAGS = $(PTHREAD_CFLAGS)
AM_CPPFLAGS += -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I$(top_srcdir)/../com-dev/src/prg/c/inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = @mg_cflags@
AM_CFLAGS += $(PTHREAD_CFLAGS)
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSSH_CFLAGS)

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


lib_LTLIBRARIES = libswoccommon.la

libswoccommon_la_SOURCES = messages.c ssh.c tcp.c
libswoccommon_la_SOURCES += validateconfig.c
libswoccommon_la_SOURCES += version.c

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LDFLAGS) $(PTHREAD_LIBS) $(LIBS)
AM_LDFLAGS += $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)
AM_LDFLAGS += $(LIBSSH_LIBS)
AM_LDFLAGS += -version-info @lib_version_info@


libswoccommon_la_LIBADD = $(tmplibraries)
