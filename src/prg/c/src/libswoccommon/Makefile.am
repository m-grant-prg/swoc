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
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = -Wall
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSSH_CFLAGS)

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif


lib_LTLIBRARIES = libswoccommon.la

libswoccommon_la_SOURCES = messages.c ssh.c tcp.c
libswoccommon_la_SOURCES += validateconfig.c
libswoccommon_la_SOURCES += version.c


AM_LDFLAGS = $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)
AM_LDFLAGS += $(LIBSSH_LIBS)
AM_LDFLAGS += -version-info 2:0:0


libswoccommon_la_LIBADD = $(tmplibraries)
