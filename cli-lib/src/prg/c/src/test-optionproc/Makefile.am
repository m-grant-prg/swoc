#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-lib/src/prg/c/src/test-optionproc/Makefile.am		#
# Author: Copyright (C) 2016-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
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
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I$(top_srcdir)/../com-dev/src/prg/c/inc
AM_CPPFLAGS += -I$(top_srcdir)/../cli-dev/src/prg/c/inc
AM_CPPFLAGS += -I../../sysgen/inc -I../../sysgen/lib


AM_CFLAGS = -Wall

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif


noinst_PROGRAMS = testoptionproc
testoptionproc_SOURCES = testoptionproc.c


LDADD = ../libswocclient/libswocclient.la
LDADD += $(tmplibraries)
