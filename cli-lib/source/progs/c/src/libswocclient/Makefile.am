#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-lib/source/progs/c/src/libswocclient/Makefile.am	#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocclient library from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 05/06/2016	MG	1.0.1	First release.				#
# 05/01/2017	MG	1.0.2	Changed to support introduction of the	#
#				swocserver daemon and use of TCP stream.#
# 14/01/2017	MG	1.0.3	Set debug or normal compile flags here.	#
# 09/06/2017	MG	1.0.4	Add support for temporary libraries.	#
#				Add support for temporary headers.	#
# 14/10/2017	MG	1.0.5	Remove validateconfig.c as it moved to	#
#				libswoccommon.				#
# 18/10/2017	MG	1.0.6	Change to use _LIBADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.7	Add SPDX license tags to source files.	#
# 13/01/2018	MG	1.0.8	Correct the use of config.h - add the	#
#				header directory to AM_CPPFLAGS.	#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I../../sysgen/inc -I../../sysgen/lib


AM_CFLAGS = -Wall
AM_CFLAGS += $(LIBMGESYSUTILS_CFLAGS) $(LIBSWOCCOMMON_CFLAGS)

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif


lib_LTLIBRARIES = libswocclient.la

libswocclient_la_SOURCES = optionproc.c
libswocclient_la_SOURCES += version.c


AM_LDFLAGS = -version-info 1:0:0
AM_LDFLAGS += $(LIBMGESYSUTILS_LIBS) $(LIBSWOCCOMMON_LIBS)


libswocclient_la_LIBADD = $(tmplibraries)
