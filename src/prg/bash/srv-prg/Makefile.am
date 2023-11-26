#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to generate swocserverd scripts from	#
# source.								#
#									#
#########################################################################


swocserverd-setup.sh: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in >$@.tmp
	mv $@.tmp $@
	chmod a+x $@
swocserverd-setup.sh: $(srcdir)/swocserverd-setup.sh.in

pkglibexec_SCRIPTS = swocserverd-setup.sh

EXTRA_DIST = $(srcdir)/swocserverd-setup.sh.in

CLEANFILES = *.tmp swocserverd-setup.sh

