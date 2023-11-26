#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021-2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate libswoccommon library from	#
# source.								#
#									#
#########################################################################


CC = $(PTHREAD_CC)


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += $(PTHREAD_CFLAGS)
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
AM_CFLAGS += $(PTHREAD_CFLAGS)
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSSH_CFLAGS)


lib_LTLIBRARIES = libswoccommon.la


libswoccommon_la_SOURCES = messages.c ssh.c tcp.c validateconfig.c
libswoccommon_la_SOURCES += version.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += $(PTHREAD_CFLAGS) $(LDFLAGS) $(PTHREAD_LIBS)
AM_LDFLAGS += $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)
AM_LDFLAGS += $(LIBSSH_LIBS)
AM_LDFLAGS += -version-info @common_lib_version_info@


libswoccommon_la_LIBADD = $(tmplibraries)

