#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021-2024 Mark Grant			#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate optionproctest test program.	#
#									#
#########################################################################


if DEBUG
.gdbinit: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
.gdbinit: $(srcdir)/.gdbinit.in

noinst_SCRIPTS = .gdbinit
endif


fix-optionproctest.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
	chmod a+x $@
fix-optionproctest.sh: $(srcdir)/fix-optionproctest.sh.in

check_SCRIPTS = fix-optionproctest.sh


if DEBUG
AM_CPPFLAGS = $(MG_DEBUG_CPPFLAGS)
else
AM_CPPFLAGS = $(MG_CPPFLAGS)
endif
AM_CPPFLAGS += -I$(srcdir)/../../../inc-tmp
AM_CPPFLAGS += -I$(srcdir)/../../../inc
AM_CPPFLAGS += -I../../../gen/inc -I../../../gen/lib


if DEBUG
AM_CFLAGS = $(MG_DEBUG_CFLAGS)
else
AM_CFLAGS = $(MG_CFLAGS)
endif


check_PROGRAMS = optionproctest
optionproctest_SOURCES = test-optionproc.c


AM_LDFLAGS = $(MG_LDFLAGS)


LDADD = $(tmplibraries)
LDADD += ../../srv-lib/libswocserver.la


EXTRA_DIST = $(srcdir)/.gdbinit.in $(srcdir)/fix-optionproctest.sh.in


CLEANFILES = *.tmp .gdbinit fix-optionproctest.sh

