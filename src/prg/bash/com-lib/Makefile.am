#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate swoc.conf setup scripts from	#
# source.								#
#									#
#########################################################################


swoc-setup.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in >$@.tmp
	mv $@.tmp $@
	chmod a+x $@
swoc-setup.sh: $(srcdir)/swoc-setup.sh.in

pkglibexec_SCRIPTS = swoc-setup.sh

EXTRA_DIST = $(srcdir)/swoc-setup.sh.in

CLEANFILES = *.tmp swoc-setup.sh

