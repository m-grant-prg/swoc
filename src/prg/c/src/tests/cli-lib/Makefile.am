#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021-2024  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate testoptionproc test program.	#
#									#
#########################################################################


fix-AT-LD-LIBRARY-PATH-bug.sh: optionproctest Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
	chmod a+x $@
	./$@ "optionproctest"
fix-AT-LD-LIBRARY-PATH-bug.sh: $(srcdir)/fix-AT-LD-LIBRARY-PATH-bug.sh.in

check_SCRIPTS = fix-AT-LD-LIBRARY-PATH-bug.sh


if DEBUG
.gdbinit: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	no_dot_target=`echo $@ | sed "s/^.//"`; \
		test -f ./$${no_dot_target}.in || srcdir=$(srcdir)/; \
		no_dot_file=`echo $(@F) | sed "s/^.//"`; \
		$(edit) $${srcdir}$${no_dot_file}.in > $@.tmp
	mv $@.tmp $@
.gdbinit: $(srcdir)/gdbinit.in

check_SCRIPTS += .gdbinit
endif


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
LDADD += ../../cli-lib/libswocclient.la


EXTRA_DIST = $(srcdir)/fix-AT-LD-LIBRARY-PATH-bug.sh.in \
	     $(srcdir)/gdbinit.in

CLEANFILES = *.tmp fix-AT-LD-LIBRARY-PATH-bug.sh \
	     .gdbinit

