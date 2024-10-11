#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021-2024  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate com-lib test programs.		#
#									#
#########################################################################


fix-AT-LD-LIBRARY-PATH-bug.sh: messagestest sshtest tcptest validatetest \
	Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || top_srcdir=$(top_srcdir)/; \
	$(edit) $${top_srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
	chmod a+x $@
	./$@ "messagestest sshtest tcptest validatetest"
fix-AT-LD-LIBRARY-PATH-bug.sh: $(top_srcdir)/fix-AT-LD-LIBRARY-PATH-bug.sh.in

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


check_PROGRAMS = messagestest sshtest tcptest validatetest
messagestest_SOURCES = test-messages.c
sshtest_SOURCES = test-ssh.c
tcptest_SOURCES = test-tcp.c
validatetest_SOURCES = test-validate.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += $(LIBMGEC_LIBS)


LDADD = $(tmplibraries)
LDADD += ../../com-lib/libswoccommon.la


EXTRA_DIST = $(srcdir)/gdbinit.in

CLEANFILES = *.tmp fix-AT-LD-LIBRARY-PATH-bug.sh \
	     .gdbinit

