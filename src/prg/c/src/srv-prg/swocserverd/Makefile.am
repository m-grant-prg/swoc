#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021-2024  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserverd daemon from	#
# source.								#
#									#
#########################################################################


# Please note: systemd requires some ordering of it's actions. Please refer to
# the comment in the top-level Makefile.am


fix-AT-LD-LIBRARY-PATH-bug.sh: swocserverd Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
	chmod a+x $@
	./$@ "swocserverd"
fix-AT-LD-LIBRARY-PATH-bug.sh: $(srcdir)/fix-AT-LD-LIBRARY-PATH-bug.sh.in

noinst_SCRIPTS = fix-AT-LD-LIBRARY-PATH-bug.sh


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

noinst_SCRIPTS += .gdbinit
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
if HEADERCHECK
AM_CFLAGS += -H
endif
if SPARSE
AM_CFLAGS += -Wsparse-all -Wno-vla
endif
if ANALYZER
AM_CFLAGS += $(MG_ANALYZER_CFLAGS)
endif
AM_CFLAGS += $(LIBMGEC_CFLAGS) $(LIBMGESYSUTILS_CFLAGS)
AM_CFLAGS += $(LIBSWOCCOMMON_CFLAGS)


noinst_HEADERS = internal.h


sbin_PROGRAMS = swocserverd
swocserverd_SOURCES = cmdlineargs.c signalhandle.c validateconfig.c version.c
swocserverd_SOURCES += main.c comms.c request.c


AM_LDFLAGS = $(MG_LDFLAGS)
AM_LDFLAGS += $(LIBMGEC_LIBS) $(LIBMGESYSUTILS_LIBS)


LDADD = $(tmplibraries)
LDADD += ../../com-lib/libswoccommon.la


EXTRA_DIST = $(srcdir)/fix-AT-LD-LIBRARY-PATH-bug.sh.in \
	     $(srcdir)/gdbinit.in

CLEANFILES = *.tmp fix-AT-LD-LIBRARY-PATH-bug.sh \
	     .gdbinit

