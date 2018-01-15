#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: cli-prg/src/prg/c/src/swocclient/Makefile.am		#
# Author: Copyright (C) 2015-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
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
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I../../sysgen/lib


AM_CFLAGS = -Wall
AM_CFLAGS += $(LIBSWOCCLIENT_CFLAGS) $(LIBMGEC_CFLAGS)

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif


sbin_PROGRAMS = swocclient
swocclient_SOURCES = main.c version.c cmdlineargs.c signalhandle.c


AM_LDFLAGS = $(LIBSWOCCLIENT_LIBS) $(LIBMGEC_LIBS)


LDADD = $(tmplibraries)
