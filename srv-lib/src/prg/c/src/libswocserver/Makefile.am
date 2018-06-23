#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-lib/src/prg/c/src/libswocserver/Makefile.am		#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver library from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 25/05/2016	MG	1.0.1	First release.				#
# 13/06/2016	MG	1.0.2	Increment library interface version as	#
#				changes to make function names more	#
#				unique breaks backwards compatibility.	#
# 20/11/2016	MG	1.0.3	Change to new source directory		#
#				structure. Add swoccommon library info.	#
# 14/01/2017	MG	1.0.4	Set debug or normal compile flags here.	#
# 15/05/2017	MG	1.0.5	Change validate config to swocserver-	#
#				only, aot common with swocserverd.	#
# 09/06/2017	MG	1.0.6	Add support for temporary libraries.	#
# 20/10/2017	MG	1.0.7	Change to use _LIBADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.0.8	Add SPDX license tags to source files.	#
# 15/01/2018	MG	1.0.9	Correct consumption of config.h		#
# 16/01/2018	MG	1.0.10	Change to include headers from relevant	#
#				-dev projects.				#
# 26/03/2018	MG	1.0.11	Add support for sparse.			#
# 23/06/2018	MG	1.0.12	Disable sparse variable length array	#
#				warnings.				#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I$(top_srcdir)/../com-dev/src/prg/c/inc
AM_CPPFLAGS += -I$(top_srcdir)/../srv-dev/src/prg/c/inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = -Wall
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBSWOCCOMMON_CFLAGS)

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif

if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif


lib_LTLIBRARIES = libswocserver.la

libswocserver_la_SOURCES = optionproc.c
libswocserver_la_SOURCES += version.c


AM_LDFLAGS = -version-info 2:0:0
AM_LDFLAGS += $(LIBMGEC_LIBS) $(LIBSWOCCOMMON_LIBS)


libswocserver_la_LIBADD = $(tmplibraries)
