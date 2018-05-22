#! /usr/bin/env bash
#########################################################################
#									#
# Script ID: srv-prg/src/prg/c/src/swocserver/Makefile.am		#
# Author: Copyright (C) 2015-2018  Mark Grant				#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserver program from	#
# source.								#
#									#
#########################################################################

#########################################################################
#									#
# Changelog								#
#									#
# Date		Author	Version	Description				#
#									#
# 22/06/2015	MG	1.0.1	First release.				#
# 06/07/2015	MG	1.0.2	Make libconffile pkg-config check	#
#				display name more meaningful.		#
# 12/07/2015	MG	1.0.3	Change to use libmgesysutils.		#
# 24/10/2015	MG	1.0.4	Restructure and change to conform to	#
#				AutoTools General Template v1.0.6.	#
# 10/05/2016	MG	1.0.4	Move header files to include directory.	#
# 28/05/2016	MG	1.0.5	Add flags for libswocserver following	#
#				it's introduction.			#
# 04/10/2016	MG	1.0.6	Add support for configure debug option.	#
# 05/10/2016	MG	1.0.7	Change compiler optimisation flag to	#
#				-Og for debugging.			#
# 13/01/2017	MG	1.1.0	Add uninstall-local target to get	#
#				systemd to shutdown the swocserver	#
#				daemon service before we uninstall	#
#				swocserver. This is because the service	#
#				ExecStop uses swocserver -e. This only	#
#				gets executed if configure is run with	#
#				--enable-systemd=yes.			#
#				Change to eliminate -g compiler option	#
#				if not debugging.			#
# 27/05/2017	MG	1.1.1	Add support for temporary header files.	#
#				Add support for temporary libraries.	#
# 20/10/2017	MG	1.1.2	Change to use _LDADD for temporary	#
#				libraries as passing -L and the temp	#
#				directory does not necessarily leave	#
#				LD_LIBRARY_PATH in the correct order.	#
# 12/01/2018	MG	1.1.3	Add SPDX license tags to source files.	#
# 16/01/2018	MG	1.1.4	Correct usage of config.h		#
#				Change to include headers from relevant	#
#				-dev projects.				#
# 24/03/2018	MG	1.1.5	Add support for sparse.			#
# 22/05/2018	MG	1.1.6	Change from swocserver.h to internal.h	#
#									#
#########################################################################


AM_CPPFLAGS = -I$(srcdir)/../../inc-tmp -I$(srcdir)/../../inc
AM_CPPFLAGS += -I$(top_srcdir)/../com-dev/src/prg/c/inc
AM_CPPFLAGS += -I$(top_srcdir)/../srv-dev/src/prg/c/inc
AM_CPPFLAGS += -I../../gen/inc -I../../gen/lib


AM_CFLAGS = -Wall
AM_CFLAGS += $(LIBSWOCSERVER_CFLAGS) $(LIBMGEC_CFLAGS)

if DEBUG
AM_CFLAGS += -g -Og
else
AM_CFLAGS += -O2
endif

if SPARSE
AM_CFLAGS += -Wsparse-all
endif


noinst_HEADERS = internal.h


sbin_PROGRAMS = swocserver
swocserver_SOURCES = main.c version.c cmdlineargs.c signalhandle.c


AM_LDFLAGS = $(LIBSWOCSERVER_LIBS) $(LIBMGEC_LIBS)


LDADD = $(tmplibraries)


if SYSD
uninstall-local:
	systemctl stop swocserverd.service
	systemctl disable swocserverd.service
endif
