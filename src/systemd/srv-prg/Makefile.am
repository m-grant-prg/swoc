#########################################################################
#									#
# Author: Copyright (C) 2017-2019, 2021, 2023  Mark Grant		#
#									#
# Released under the GPLv3 only.					#
# SPDX-License-Identifier: GPL-3.0-only					#
#									#
# Purpose:								#
#	AutoMake script file to install systemd service file.		#
#									#
#########################################################################


systemddir = $(libdir)/systemd/system

swocserverd.service: Makefile
	mkdir -p $(@D)
	rm -f $@ $@.tmp
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(edit) $${srcdir}$(@F).in > $@.tmp
	mv $@.tmp $@
swocserverd.service: $(srcdir)/swocserverd.service.in

systemd_DATA = swocserverd.service

# systemd requires some ordering of it's actions. Please refer to the comment in
# the top-level Makefile.am
install-data-hook:
	systemctl enable swocserverd.service
	systemctl start swocserverd.service

uninstall-hook:
	systemctl daemon-reload

EXTRA_DIST = $(srcdir)/swocserverd.service.in

CLEANFILES = *.tmp swocserverd.service

