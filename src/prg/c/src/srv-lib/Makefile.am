#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021-2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswocserver library from	#
# source.								#
#									#
#########################################################################


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../inc
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
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBSWOCCOMMON_CFLAGS)


lib_LTLIBRARIES = libswocserver.la

libswocserver_la_SOURCES = optionproc.c
libswocserver_la_SOURCES += version.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += -version-info @server_lib_version_info@
AM_LDFLAGS += $(LIBMGEC_LIBS)


libswocserver_la_LIBADD = $(tmplibraries)
libswocserver_la_LIBADD += ../com-lib/libswoccommon.la

