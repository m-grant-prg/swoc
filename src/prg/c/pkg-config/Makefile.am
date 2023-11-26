#########################################################################
#									#
# Author: Copyright (C) 2016-2019, 2021-2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate pkgconfigs from source.	#
#									#
#########################################################################


libswocclient.pc libswoccommon.pc libswocserver.pc: Makefile
	mkdir -p $(@D)
	rm -f $@
	$(edit) $(srcdir)/$(@F).in > $@

libswocclient.pc: $(srcdir)/libswocclient.pc.in
libswoccommon.pc: $(srcdir)/libswoccommon.pc.in
libswocserver.pc: $(srcdir)/libswocserver.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libswocclient.pc libswoccommon.pc libswocserver.pc

EXTRA_DIST = $(srcdir)/libswocclient.pc.in $(srcdir)/libswoccommon.pc.in \
	     $(srcdir)/libswocserver.pc.in

CLEANFILES = libswocclient.pc libswoccommon.pc libswocserver.pc

