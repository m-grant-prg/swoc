#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: com-lib/src/prg/c/src/test-validate/Makefile.am		#
# Author: Copyright (C) 2017-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate testvalidate test program.	#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 03/10/2017	MG	1.0.0	First release.				#
# 19/10/2017	MG	1.0.1	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
#				Re-order into logical steps.		#
# 12/01/2018	MG	1.0.2	Add SPDX license tags to source files.	#
# 14/01/2018	MG	1.0.3	Correct usage of config.h		#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = -Wall

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif


noinst_PROGRAMS = testvalidate
testvalidate_SOURCES = testvalidate.c


LDADD = ../libswoccommon/libswoccommon.la
LDADD += $(tmplibraries)
